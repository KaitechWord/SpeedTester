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
		boost::asio::ip::udp::endpoint endpoint;
		boost::asio::ip::udp::socket socket;
		double totalBytesReceived;
		bool isSizeSet;
		std::chrono::steady_clock::time_point startTime;
		void handleMessageSize();
		void handleIncomingMessages();
};