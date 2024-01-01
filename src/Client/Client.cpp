#include "Client.h"
#include <iostream>
#include <boost/log/trivial.hpp>

Client::Client(const std::string& ip, int port, int dataSize, bool isNagleUsed)
	: ip(ip), port(port), dataSize(dataSize), isNagleUsed(isNagleUsed), threadPool(4), isConnectionActive(false)
	, discovery("225.1.1.1", 8888)
	, tcpSender(ip, port, dataSize, isNagleUsed, isConnectionActive, shouldQuit)
	, udpSender(ip, port, dataSize, isConnectionActive, shouldQuit)
{
	try {
		//this->threadPool.queueJob([this]() { this->discovery.sendDiscover(); });
		this->threadPool.queueJob([this]() { this->tcpSender.run(); });
		this->threadPool.queueJob([this]() { this->udpSender.run(); });
		this->threadPool.queueJob([this]() { while (true) {
			char a = getchar();
			if (a == 'q') {
				this->shouldQuit.store(true);
				break;
			}
		}});
		this->threadPool.start();
		while (this->threadPool.busy()) {};
		this->threadPool.stop();
	}
	catch (std::exception& e) {
		std::cout << "e: " << e.what() << std::endl;
	}
}