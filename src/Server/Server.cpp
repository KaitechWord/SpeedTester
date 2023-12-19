#include "Server.h"
#include <iostream>
#include <boost/log/trivial.hpp>

Server::Server(int port)
	: port(port)
	, threadPool(THREAD_NUM)
	, shouldQuit(false)
	, tcpListener(port, this->shouldQuit)
	, udpListener(port, this->shouldQuit)
{
	this->threadPool.queueJob([this]() { this->tcpListener.run(); });
	this->threadPool.queueJob([this]() { this->udpListener.run(); });
	this->threadPool.queueJob([this]() { while (true) {
		char a = getchar();
		if (a == 'q') {
			this->shouldQuit.store(true);
		}
	}});
	this->threadPool.start();
	while (this->threadPool.busy()) {};
	this->threadPool.stop();
}