#pragma once
#include "TCPSender.h"
#include "UDPSender.h"
#include "Discovery.h"
#include "../ThreadPool/ThreadPool.h"
#include <string>

class Client {
	public:
		Client(const std::string &ip, int port, int dataSize, bool isNagleUsed);
	private:
		std::string ip;
		int port;
		int dataSize;
		bool isNagleUsed;
		ThreadPool threadPool;
		std::atomic<bool> shouldQuit;
		std::atomic<bool> isConnectionActive;
		Discovery discovery;
		TCPSender tcpSender;
		UDPSender udpSender;
};