#pragma once
#include "../ThreadPool/ThreadPool.h"
#include <boost/asio.hpp>
#include <chrono>
#include <atomic>

class TCPListener {
	public:
		TCPListener(int port, std::atomic<bool>& shouldQuit);
		void run();
	private:
		void handleAccept(const boost::system::error_code& error); //test
		int port;
		std::atomic<int> messageSize;
		ThreadPool threadPool;
		boost::asio::io_service ioService;
		boost::asio::ip::tcp::acceptor acceptor;
		boost::asio::ip::tcp::socket socket;
		double totalBytesReceived;
		std::atomic<bool> &shouldQuit;
		std::atomic<bool> isConnected;
		std::atomic<bool> isRunning = false;
		bool newConnection = false;
		std::atomic<bool> isConnectionWaiting = false;
		std::chrono::steady_clock::time_point startTime;
		void establishConnection();
		void handleMessageSize();
		void handleIncomingMessages(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
};