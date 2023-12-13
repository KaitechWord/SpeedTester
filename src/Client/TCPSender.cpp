#include "TCPSender.h"

TCPSender::TCPSender(const std::string& ip, int port, int dataSize, bool isNagleUsed)
	: ip(ip), port(port), dataSize(dataSize), isNagleUsed(isNagleUsed), socket(ioService), isServerBusy(false), threadPool(2)
{}

void TCPSender::run() {
	threadPool.queueJob([this]() { this->sendMessages();});
	threadPool.queueJob([this]() { this->listenToMessages();});
}

void TCPSender::sendMessages() {
	socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345));
}

void TCPSender::listenToMessages() {

}

