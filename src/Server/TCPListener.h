#pragma once
#include <boost/asio.hpp>

class TCPListener {
	public:
		TCPListener(int port);
		void run();
	private:
		int port;
		boost::asio::io_service ioService;
		boost::asio::ip::tcp::acceptor acceptor;
		boost::asio::ip::tcp::socket socket;
};