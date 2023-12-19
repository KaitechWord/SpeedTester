#include "UDPSender.h"
#include <boost/log/trivial.hpp>

UDPSender::UDPSender(const std::string& ip, int port, int dataSize, bool isNagleUsed, std::atomic<bool>& isConnectionActive, std::atomic<bool>& shouldQuit)
	: ip(ip), port(port), dataSize(dataSize), isNagleUsed(isNagleUsed), isConnectionActive(isConnectionActive), shouldQuit(shouldQuit)
	, endpoint(boost::asio::ip::address::from_string(this->ip), this->port)
	, socket(this->ioService)
{
	socket.open(boost::asio::ip::udp::v4());
}

void UDPSender::run() {
	this->sendMessages();
}

void UDPSender::sendMessages() {
	while (true) {
		while (this->isConnectionActive.load()) {
			boost::system::error_code error;
			BOOST_LOG_TRIVIAL(info) << "UDP client connected.";
			socket.send_to(boost::asio::buffer("SIZE:" + std::to_string(this->dataSize)), endpoint, 0, error);
			std::vector<char> message(dataSize, '\a');
			while (this->isConnectionActive.load()) {
				try {
					socket.send_to(boost::asio::buffer(message), endpoint, 0, error);
					//boost::asio::write(this->socket, boost::asio::buffer(message), error);
					if (error == boost::asio::error::eof) {
						BOOST_LOG_TRIVIAL(info) << "UDP server disconnected.";
						break;
					}
					else if (error) {
						BOOST_LOG_TRIVIAL(error) << "UDP error during write: " << error.message();
						break;
					}
					if (this->shouldQuit.load()) {
						break;
					}
				}
				catch (const boost::system::system_error& e) {
					BOOST_LOG_TRIVIAL(error) << "UDP error during write: " << e.what();
				}
			}
			if (this->shouldQuit.load()) {
				break;
			}
		}
		if (this->shouldQuit.load()) {
			break;
		}
	}
	BOOST_LOG_TRIVIAL(info) << "UDP exited.";
}