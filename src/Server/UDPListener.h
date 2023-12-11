#pragma once
#include <boost/asio.hpp>
#include "../ThreadPool/ThreadPool.h"

class UDPListener {
	public:
		UDPListener(int port);
		void run();
	private:
		int port;
		std::atomic<int> messageSize;
		ThreadPool threadPool;
		boost::asio::io_service ioService;
		boost::asio::ip::tcp::acceptor acceptor;
		boost::asio::ip::tcp::socket socket;
		double totalBytesReceived;
		bool isSizeSet;
		std::chrono::steady_clock::time_point startTime;
		void handleMessageSize();
		void handleIncomingMessages();
};