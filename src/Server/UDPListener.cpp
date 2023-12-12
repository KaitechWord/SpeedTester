#include "UDPListener.h"
#include <boost/log/trivial.hpp>

UDPListener::UDPListener(int port)
	: port(port), startTime(std::chrono::steady_clock::now()), threadPool(1), messageSize(0), totalBytesReceived(0), isSizeSet(false)
	// it accepts every ip in given port
	, endpoint(boost::asio::ip::udp::v4(), port), socket(ioService, endpoint)
{
	threadPool.start();
}

void UDPListener::handleMessageSize() {
	boost::asio::streambuf receiveBuffer;
	boost::system::error_code error;
	std::size_t bytesRead = socket.receive_from(receiveBuffer.prepare(100), endpoint, 0, error);
	receiveBuffer.commit(bytesRead);
	std::istream is(&receiveBuffer);
	std::string receivedData;
	std::getline(is, receivedData);
	BOOST_LOG_TRIVIAL(info) << "Received udp: " << receivedData;
	std::size_t pos = receivedData.find(':');
	if (pos != std::string::npos) {
		std::string numStr = receivedData.substr(pos + 1);
		try {
			int num = std::stoi(numStr);
			if (!isSizeSet) {
				this->messageSize = num;
				BOOST_LOG_TRIVIAL(info) << "Extracted size: " << this->messageSize;
				this->isSizeSet = true;
			} else if (num != this->messageSize){
				BOOST_LOG_TRIVIAL(info) << "Extracted size: " << this->messageSize << " is wrong. Sending disconnecting message";
				socket.send_to(boost::asio::buffer("DISCONNECT"), this->endpoint);
			}
		}
		catch (const std::exception& e) {
			BOOST_LOG_TRIVIAL(error) << "Error converting string to number: " << e.what() << std::endl;
		}
	}
	receiveBuffer.consume(receiveBuffer.size());
}

void UDPListener::handleIncomingMessages() {
	this->totalBytesReceived = 0;
	this->startTime = std::chrono::steady_clock::now();
	while (true) {
		this->handleMessageSize();
		if (!this->isSizeSet) {
			continue;
		}
		std::vector<char> message(this->messageSize);
		boost::system::error_code error;

		// Read the specified number of bytes
		std::size_t bytesRead = socket.receive_from(boost::asio::buffer(message), endpoint, 0, error);
		if (error == boost::asio::error::eof) {
			// Client disconnected
			BOOST_LOG_TRIVIAL(info) << "Client disconnected.";
			break;
		}
		else if (error) {
			// Handle other errors
			BOOST_LOG_TRIVIAL(error) << "Error during read: " << error.message();
			break;
		}
		else {
			// Process received data
			auto currentTime = std::chrono::steady_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0;
			std::string receivedData(message.data(), bytesRead);
			//BOOST_LOG_TRIVIAL(info) << "Message: " << receivedData;
			this->totalBytesReceived += bytesRead;
			double speedKbPerSec = totalBytesReceived / elapsedTime / 1024.0;
			BOOST_LOG_TRIVIAL(info) << "UDP Received " << totalBytesReceived / 1024 << " KB in "
				<< elapsedTime << " seconds with the speed of " << speedKbPerSec << " KB/sec" << std::endl;
		}
	}
	this->socket.close();
}

void UDPListener::run() {
	try {
		this->handleMessageSize();
		this->handleIncomingMessages();
	}
	catch (const boost::system::system_error& e) {
		BOOST_LOG_TRIVIAL(error) << "Error during TCP accept: " << e.what() << std::endl;
	}
}