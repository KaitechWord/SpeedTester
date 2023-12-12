#include "Client.h"
#include <iostream>

Client::Client(const std::string& ip, int port)
	: ip(ip), port(port)
{
	std::cout << "Provide message size: ";
	std::cin >> messageSize;
	std::string nagleDecision;
	bool nagleDecisionMade = false;
	while (!nagleDecisionMade) {
		std::cout << "Should Nagle algorithm be used?(yes/no): ";
		std::cin >> nagleDecision;
		if (nagleDecision == "yes" || nagleDecision == "no") {
			shouldNagleBeUsed = true;
		} else {
			std::cout << "Unrecognised command." << std::endl;
		}
	}
}