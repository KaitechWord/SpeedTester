#include "Server.h"
#include <iostream>

Server::Server(int port)
	: port(port), threadPool(THREAD_NUM)
{
}