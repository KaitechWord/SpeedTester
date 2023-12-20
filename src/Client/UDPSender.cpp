#include "UDPSender.h"
#include <boost/log/trivial.hpp>

UDPSender::UDPSender(const std::string& ip, int port, int dataSize, std::atomic<bool>& isConnectionActive, std::atomic<bool>& shouldQuit)
	: ip(ip), port(port), dataSize(dataSize), isConnectionActive(isConnectionActive), shouldQuit(shouldQuit)
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
		int messageIndex = 1;
		while (this->isConnectionActive.load()) {
			boost::system::error_code error;
			BOOST_LOG_TRIVIAL(info) << "UDP client connected.";
			socket.send_to(boost::asio::buffer("SIZE:" + std::to_string(this->dataSize)), endpoint, 0, error);
			while (this->isConnectionActive.load()) {
				try {
					std::vector<char> message;
					std::string num = std::to_string(messageIndex);
					messageIndex += 1;
					for (int i = 0; i < num.size(); i++) {
						message.push_back(num[i]);
					}
					for (int i = num.size(); i < this->dataSize; i++) {
						message.push_back('a');
					}
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
			if (this->shouldQuit.load() || !this->isConnectionActive.load()) {
				Sleep(10);
				socket.send_to(boost::asio::buffer("FINE"), endpoint, 0, error);
				BOOST_LOG_TRIVIAL(info) << "UDP sending FINE";
				this->shouldQuit.store(true);
				break;
			}
		}
		if (this->shouldQuit.load()) {
			break;
		}
	}
	BOOST_LOG_TRIVIAL(info) << "UDP exited.";
}