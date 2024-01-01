#include "TCPSender.h"
#include <boost/log/trivial.hpp>

TCPSender::TCPSender(const std::string& ip, int port, int dataSize, bool isNagleUsed, std::atomic<bool> &isConnectionActive, std::atomic<bool>& shouldQuit)
	: ip(ip), port(port), dataSize(dataSize), isNagleUsed(isNagleUsed), socket(ioService), isConnectionActive(isConnectionActive), shouldQuit(shouldQuit)
{}

void TCPSender::run() {
	this->sendMessages();
}

void TCPSender::sendMessages() {
	try {
		socket.open(boost::asio::ip::tcp::v4());
		if (!this->isNagleUsed) {
			//disabling nagle alg.
			socket.set_option(boost::asio::ip::tcp::no_delay(true));
		}
		socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(this->ip), this->port));
		boost::asio::write(this->socket, boost::asio::buffer("SIZE:" + std::to_string(this->dataSize) + '\a'));
		BOOST_LOG_TRIVIAL(trace) << "TCP sent SIZE:" << this->dataSize;
		boost::asio::streambuf receiveBuffer;
		boost::system::error_code error;
		read_until(socket, receiveBuffer, '\a', error);
		std::string receivedData(boost::asio::buffers_begin(receiveBuffer.data()),
			boost::asio::buffers_begin(receiveBuffer.data()) + receiveBuffer.size());
		std::size_t busyPos = receivedData.find("BUSY");
		if (busyPos != std::string::npos) {
			socket.close();
		} else if (receivedData.find("GO ON") != std::string::npos) {
			std::vector<char> message(dataSize, '\a');
			this->isConnectionActive.store(true);
			boost::system::error_code error;
			while (true) {
				try {
					boost::asio::write(this->socket, boost::asio::buffer(message), error);
					if (error == boost::asio::error::eof) {
						BOOST_LOG_TRIVIAL(trace) << "TCP server disconnected.";
						break;
					}
					else if (error) {
						BOOST_LOG_TRIVIAL(fatal) << "TCP error during write: " << error.message();
						break;
					}
					if (this->shouldQuit.load()) {
						break;
					}
				}
				catch (const boost::system::system_error& e) {
					BOOST_LOG_TRIVIAL(fatal) << "TCP exception during write: " << e.what();
				}
			}
			this->isConnectionActive.store(false);
			socket.close();
			socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(this->ip), this->port));
			boost::asio::write(this->socket, boost::asio::buffer("END"));
		}
	}
	catch (const boost::system::system_error& e) {
		BOOST_LOG_TRIVIAL(fatal) << "TCP error: " << e.what();
	}
	BOOST_LOG_TRIVIAL(trace) << "TCP exited.";
}