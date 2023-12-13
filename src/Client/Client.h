#pragma once
#include "TCPSender.h"
#include "UDPSender.h"
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
		TCPSender tcpSender;
		UDPSender udpSender;
};