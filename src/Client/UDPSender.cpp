#include "UDPSender.h"

UDPSender::UDPSender(const std::string& ip, int port, int dataSize, bool isNagleUsed)
	: ip(ip), port(port), dataSize(dataSize), isNagleUsed(isNagleUsed)
{}