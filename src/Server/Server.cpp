#include "Server.h"
#include <iostream>
#include <boost/log/trivial.hpp>

Server::Server(int port)
	: port(port)
	, threadPool(4)
	, shouldQuit(false)
	, discoveryListener("225.1.1.1", 8888, port)
	, tcpListener(port, this->shouldQuit)
	, udpListener(port, this->shouldQuit)
{
	try {
		//this->threadPool.queueJob([this]() { this->discoveryListener.listenForDiscoveryRequests(); });
		this->threadPool.queueJob([this]() { this->tcpListener.run(); });
		this->threadPool.queueJob([this]() { this->udpListener.run(); });
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