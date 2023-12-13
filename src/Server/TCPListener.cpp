#include "TCPListener.h"
#include <boost/log/trivial.hpp>

//https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

TCPListener::TCPListener(int port) 
	: port(port), startTime(std::chrono::steady_clock::now()), threadPool(1), isConnected(false), isMultipleConnected(false), messageSize(0), totalBytesReceived(0)
	// it accepts every ip in given port
	, acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	, socket(ioService)
{
	threadPool.start();
}

void TCPListener::establishConnection(){
	this->acceptor.accept(this->socket);
	if (this->isConnected) {
		try {
			boost::asio::write(this->socket, boost::asio::buffer("BUSY"));
			BOOST_LOG_TRIVIAL(info) << "TCP sent BUSY";
			this->socket.close();
			this->isMultipleConnected = true;
		}
		catch (const boost::system::system_error& e) {
			BOOST_LOG_TRIVIAL(error) << "TCP error sending busy message: " << e.what();
		}
		BOOST_LOG_TRIVIAL(info) << "TCP connection already established.";
		return;
	}
	this->isConnected.store(true);
	BOOST_LOG_TRIVIAL(info) << "TCP Connection established.";
}

void TCPListener::handleMessageSize(){
	boost::asio::streambuf receiveBuffer;
	boost::system::error_code error;
	read_until(socket, receiveBuffer, '\a', error);
	std::string receivedData(boost::asio::buffers_begin(receiveBuffer.data()),
		boost::asio::buffers_begin(receiveBuffer.data()) + receiveBuffer.size());
	std::size_t pos = receivedData.find(':');
	if (pos != std::string::npos) {
		std::string numStr = receivedData.substr(pos + 1);
		try {
			this->messageSize.store(std::stoi(numStr));
			BOOST_LOG_TRIVIAL(info) << "TCP extracted size: " << messageSize;
		}
		catch (const std::exception& e) {
			BOOST_LOG_TRIVIAL(error) << "TCP error converting string to number: " << e.what() << std::endl;
		}
	}
	else {
		BOOST_LOG_TRIVIAL(error) << "TCP invalid format. Missing ':' in the received string." << std::endl;
	}
	receiveBuffer.consume(receiveBuffer.size());
}


void TCPListener::handleIncomingMessages(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
	this->totalBytesReceived = 0;
	this->startTime = std::chrono::steady_clock::now();
	while (true) {
		std::vector<char> message(this->messageSize);
		boost::system::error_code error;

		// Read the specified number of bytes
		std::size_t bytesRead = boost::asio::read(*socket, boost::asio::buffer(message), error);
		if (error == boost::asio::error::eof) {
			// Client disconnected
			BOOST_LOG_TRIVIAL(info) << "TCP client disconnected.";
			break;
		}
		else if (error) {
			// Handle other errors
			BOOST_LOG_TRIVIAL(error) << "TCP error during read: " << error.message();
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
			BOOST_LOG_TRIVIAL(info) << "TCP Received " << totalBytesReceived / 1024 << " KB in "
				<< elapsedTime << " seconds with the speed of " << speedKbPerSec << " KB/sec" << std::endl;
		}
	}
	socket->close();
	this->isConnected.store(false);
}

void TCPListener::run(){
	try {
		while (true) {
			this->establishConnection();
			if (!this->isMultipleConnected) {
				this->handleMessageSize();
				std::shared_ptr<boost::asio::ip::tcp::socket> threadSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(this->socket));
				this->threadPool.queueJob([this, threadSocket]() { this->handleIncomingMessages(threadSocket); });
			} else {
				this->isMultipleConnected = false;
			}
		}
	}
	catch (const boost::system::system_error& e) {
		BOOST_LOG_TRIVIAL(error) << "TCP error during TCP accept: " << e.what() << std::endl;
	}
}