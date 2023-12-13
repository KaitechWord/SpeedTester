#include "Client.h"
#include <iostream>

Client::Client(const std::string& ip, int port, int dataSize, bool isNagleUsed)
	: ip(ip), port(port), dataSize(dataSize), isNagleUsed(isNagleUsed), threadPool(2)
{}