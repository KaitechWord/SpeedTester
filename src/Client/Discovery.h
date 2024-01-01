#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <boost/asio.hpp>

class Discovery {
    public:
        Discovery(const std::string& multicastAddress, unsigned short multicastPort);
        void sendDiscover();
    private:

        boost::asio::io_service ioService;
        boost::asio::ip::udp::endpoint multicastEndpoint;
        boost::asio::ip::udp::socket socket;
        std::thread discoverThread;
};