#pragma once
#include "TCPListener.h"
#include "UDPListener.h"
#include "../ThreadPool/ThreadPool.h"

#define THREAD_NUM 2

class Server {
	public:
		Server(int port);
	private:
		int port;
		TCPListener tcpListener;
		UDPListener udpListener;
		ThreadPool threadPool;
};