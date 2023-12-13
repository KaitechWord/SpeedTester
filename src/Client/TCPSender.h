#pragma once
#include <string>
#include <atomic>
#include <boost/asio.hpp>
#include "../ThreadPool/ThreadPool.h"

class TCPSender {
	public:
		TCPSender(const std::string& ip, int port, int dataSize, bool isNagleUsed);
		void run();
	private:
		std::string ip;
		int port;
		int dataSize;
		bool isNagleUsed;
		std::atomic<bool> isServerBusy;
		boost::asio::io_service ioService;
		boost::asio::ip::tcp::socket socket;
		boost::asio::ip::tcp::endpoint endpoint;
		ThreadPool threadPool;
		void sendMessages();
		void listenToMessages();
};