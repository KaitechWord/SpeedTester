#pragma once
#include <string>
#include <atomic>
#include <boost/asio.hpp>

class UDPSender{
	public:
		UDPSender(const std::string &ip, int port, int dataSize, bool isNagleUsed, std::atomic<bool> &isConnectionActive, std::atomic<bool>& shouldQuit);
		void run();
	private:
		std::string ip;
		int port; 
		int dataSize; 
		bool isNagleUsed;
		boost::asio::io_service ioService;
		boost::asio::ip::udp::endpoint endpoint;
		boost::asio::ip::udp::socket socket;
		std::atomic<bool> &shouldQuit;
		std::atomic<bool> &isConnectionActive;
		void sendMessages();
};