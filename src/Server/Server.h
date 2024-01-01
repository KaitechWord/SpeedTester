#pragma once
#include "TCPListener.h"
#include "UDPListener.h"
#include "DiscoveryListener.h"
#include "../ThreadPool/ThreadPool.h"
#include <atomic>

class Server {
	public:
		Server(int port);
	private:
		int port;
		std::atomic<bool> shouldQuit;
		DiscoveryListener discoveryListener;
		TCPListener tcpListener;
		UDPListener udpListener;
		ThreadPool threadPool;
};