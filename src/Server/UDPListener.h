#pragma once
#include <boost/asio.hpp>

class UDPListener {
	public:
		UDPListener(int port, std::atomic<bool>& shouldQuit);
		void run();
	private:
		int port;
		std::atomic<int> messageSize;
		boost::asio::io_service ioService;
		boost::asio::ip::udp::endpoint endpoint;
		boost::asio::ip::udp::socket socket;
		double totalBytesReceived;
		bool isSizeSet;
		std::chrono::steady_clock::time_point startTime;
		std::atomic<bool> &shouldQuit;
		void handleMessageSize();
		void handleIncomingMessages();
};