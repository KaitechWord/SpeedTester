#include "Server.h"
#include <iostream>

Server::Server(int port)
	: port(port)
	, threadPool(THREAD_NUM)
	, tcpListener(port)
{
	this->threadPool.queueJob([this]() { this->tcpListener.run(); });
	this->threadPool.start();
	while (this->threadPool.busy()) { std::cout << "busy" << std::endl; };
	this->threadPool.stop();
	std::cout << "dupa";
}