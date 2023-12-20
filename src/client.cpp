#include <iostream>
#include "./ThreadPool/ThreadPool.h"
#include "./Client/Client.h"
#include "./Client/TCPSender.h"
#include "./Client/UDPSender.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>

int main() {
	//boost::log::add_file_log("ClientLogs.log");
	std::string ip;
	int port, dataSize;
	bool isNagleUsed = false;
	std::cout << "Provide ip address: ";
	std::cin >> ip;
	std::cout << "Provide port: ";
	std::cin >> port;
	std::cout << "Provide data size: ";
	std::cin >> dataSize;
	std::cout << "Should Nagle alg. be used(0/1)?: ";
	std::cin >> isNagleUsed;
	Client client(ip, port, dataSize, isNagleUsed);
	return 0;
}