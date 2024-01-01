#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <string>

class DiscoveryListener {
    public:
        DiscoveryListener(const std::string& multicastAddress, int multicastPort, int listeningPort);

        void listenForDiscoveryRequests();

    private:
        boost::asio::io_context ioContext;
        boost::asio::ip::udp::endpoint multicastEndpoint;
        int listeningPort;
        boost::asio::ip::udp::socket socket;
};