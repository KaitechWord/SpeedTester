#pragma once
#include <string>

class UDPSender{
	public:
		UDPSender(const std::string &ip, int port, int dataSize, bool isNagleUsed);
	private:
		std::string ip;
		int port; 
		int dataSize; 
		bool isNagleUsed;
};