#pragma once
#include <string>
#include <atomic>
#include <boost/asio.hpp>

class TCPSender {
	public:
		TCPSender(const std::string& ip, int port, int dataSize, bool isNagleUsed, std::atomic<bool> &isConnectionActive, std::atomic<bool>& shouldQuit);
		void run();
	private:
		std::string ip;
		int port;
		int dataSize;
		bool isNagleUsed;
		std::atomic<bool> &shouldQuit;
		std::atomic<bool> &isConnectionActive;
		boost::asio::io_service ioService;
		boost::asio::ip::tcp::socket socket;
		boost::asio::ip::tcp::endpoint endpoint;
		void sendMessages();
		void listenToMessages();
};