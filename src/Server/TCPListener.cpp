#include "TCPListener.h"
#include "iostream"
#include <boost/log/trivial.hpp>

//https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

TCPListener::TCPListener(int port) 
	: port(port)
	// it accepts every ip in given port
	, acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	, socket(ioService)
{}

void TCPListener::run() {
	try {
		this->acceptor.accept(this->socket);
		BOOST_LOG_TRIVIAL(info) << "TCP Connection established.";
		//Handle incoming connections
		boost::asio::streambuf receiveBuffer;
		boost::system::error_code error;
		read_until(socket, receiveBuffer, '\a', error);
		std::string receivedData(boost::asio::buffers_begin(receiveBuffer.data()),
		boost::asio::buffers_begin(receiveBuffer.data()) + receiveBuffer.size());
		BOOST_LOG_TRIVIAL(info) << "Received: " << receivedData;
		std::size_t pos = receivedData.find(':');
		int size;
		if (pos != std::string::npos) {
			std::string numStr = receivedData.substr(pos + 1);
			try {
				size = std::stoi(numStr);
				BOOST_LOG_TRIVIAL(info) << "Extracted size: " << size;
			}
			catch (const std::exception& e) {
				std::cerr << "Error converting string to number: " << e.what() << std::endl;
			}
		}
		else {
			std::cerr << "Invalid format. Missing ':' in the received string." << std::endl;
		}
		receiveBuffer.consume(receiveBuffer.size());
		while (true) {
			
			// Keep reading until an error occurs or the client disconnects
			//while (read(socket, receiveBuffer, error) > 0) {
			//	// Process received data as needed// Process received data
			//	std::string receivedData(boost::asio::buffers_begin(receiveBuffer.data()),
			//		boost::asio::buffers_begin(receiveBuffer.data()) + receiveBuffer.size());
			//	std::cout << "Received: " << receivedData << std::endl;
			//	receiveBuffer.consume(receiveBuffer.size());
			//}
			std::vector<char> message(size);
			boost::system::error_code error;

			// Read the specified number of bytes
			std::size_t bytesRead = boost::asio::read(socket, boost::asio::buffer(message), error);
			if (error == boost::asio::error::eof) {
				// Client disconnected
				BOOST_LOG_TRIVIAL(info) << "Client disconnected." << std::endl;
			}
			else if (error) {
				// Handle other errors
				std::cerr << "Error during read: " << error.message() << std::endl;
			}
			else {
				// Process received data
				std::string receivedData(message.data(), bytesRead);
				BOOST_LOG_TRIVIAL(info) << "Received: " << bytesRead << std::endl;

				// Continue processing or close the connection as needed
			}
		}
		this->socket.close();
	}
	catch (const boost::system::system_error& e) {
		std::cerr << "Error during TCP accept: " << e.what() << std::endl;
	}
    
}