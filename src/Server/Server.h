#pragma once
#include "TCPListener.h"
#include "../ThreadPool/ThreadPool.h"

#define THREAD_NUM 2

class Server {
	public:
		Server(int port);
	private:
		int port;
		TCPListener tcpListener;
		ThreadPool threadPool;
};