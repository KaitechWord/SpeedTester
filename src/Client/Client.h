#pragma once
#include "TCPSender.h"
#include "UDPSender.h"
#include <string>

class Client {
	public:
		Client(const std::string &ip, int port);
	private:
		std::string ip;
		int port;
		int messageSize;
		bool shouldNagleBeUsed;
};