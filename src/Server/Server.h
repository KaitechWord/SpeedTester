#pragma once
#include <boost/asio.hpp>
#include "../ThreadPool/ThreadPool.h"

#define THREAD_NUM 2

class Server {
	public:
		Server();
	private:
		int port;
		ThreadPool threadPool;
};