#include "UDPListener.h"
#include <boost/log/trivial.hpp>

UDPListener::UDPListener(int port, std::atomic<bool>& shouldQuit)
	: port(port), startTime(std::chrono::steady_clock::now()), threadPool(1), messageSize(0), totalBytesReceived(0), isSizeSet(false)
	, shouldQuit(shouldQuit)
	// it accepts every ip in given port
	, endpoint(boost::asio::ip::udp::v4(), port), socket(ioService, endpoint)
{
	threadPool.start();
}

void UDPListener::handleIncomingMessages() {
	this->totalBytesReceived = 0;
	this->messageSize = 0;
	this->isSizeSet = false;
	while (true) {
		if (!this->isSizeSet) {
			messageSize = 100;
		}
		std::vector<char> receivedData(this->messageSize);
		boost::system::error_code error;

		// Read the specified number of bytes
		std::size_t bytesRead = socket.receive_from(boost::asio::buffer(receivedData), endpoint, 0, error);
		std::string message(receivedData.begin(), receivedData.end());
		std::size_t pos = message.find("SIZE:");
		if (pos != std::string::npos) {
			//if has size
			std::string numStr = message.substr(pos + std::string("SIZE:").length());
			try {
				int num = std::stoi(numStr);
				if (!isSizeSet) {
					this->messageSize = num;
					BOOST_LOG_TRIVIAL(info) << "UDP extracted size: " << this->messageSize;
					this->isSizeSet = true;
					this->startTime = std::chrono::steady_clock::now();
				}
				else if (num != this->messageSize) {
					BOOST_LOG_TRIVIAL(info) << "UDP extracted size: " << this->messageSize << " is wrong. Sending disconnecting message";
					socket.send_to(boost::asio::buffer("DISCONNECT"), this->endpoint);
				}
			}
			catch (const std::exception& e) {
				BOOST_LOG_TRIVIAL(error) << "UDP error converting string to number: " << e.what() << std::endl;
			}
		} else if (this->isSizeSet) {
			//if just random packet
			if (error) {
				// Handle other errors
				BOOST_LOG_TRIVIAL(error) << "UDP error during read: " << error.message();
				break;
			}
			else {
				// Process received data
				auto currentTime = std::chrono::steady_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0;
				std::string receivedData(message.data(), bytesRead);
				BOOST_LOG_TRIVIAL(info) << "UDP bytesRead: " << bytesRead;
				this->totalBytesReceived += bytesRead;
				double speedKbPerSec = totalBytesReceived / elapsedTime / 1024.0;
				BOOST_LOG_TRIVIAL(info) << "UDP Received " << totalBytesReceived / 1024 << " KB in "
					<< elapsedTime << " seconds with the speed of " << speedKbPerSec << " KB/sec" << std::endl;
			}
		}
	}
	this->socket.close();
}

void UDPListener::run() {
	try {
		while (true) {
			this->handleIncomingMessages();
		}
	}
	catch (const boost::system::system_error& e) {
		BOOST_LOG_TRIVIAL(error) << "UDP error during UDP accept: " << e.what() << std::endl;
	}
}