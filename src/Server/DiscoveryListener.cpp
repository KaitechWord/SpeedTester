#include "DiscoveryListener.h"

DiscoveryListener::DiscoveryListener(const std::string& multicastAddress, int multicastPort, int listeningPort)
    :
    multicastEndpoint(boost::asio::ip::make_address(multicastAddress), multicastPort),
    listeningPort(listeningPort),
    socket(ioContext, multicastEndpoint.protocol())
{
    // Join the multicast group
    socket.set_option(boost::asio::ip::multicast::join_group(multicastEndpoint.address()));
    ioContext.run();
}

void DiscoveryListener::listenForDiscoveryRequests() {
    while (true) {
        boost::system::error_code error;
        boost::asio::ip::udp::endpoint senderEndpoint;

        // Synchronously receive multicast DISCOVERY requests
        std::array<char, 1024> receiveBuffer;
        std::size_t bytesRead = socket.receive_from(boost::asio::buffer(receiveBuffer), senderEndpoint, 0, error);

        if (!error) {
            std::string receivedMessage(receiveBuffer.data(), bytesRead);

            // Process the received message (assuming it's a DISCOVERY request)
            if (receivedMessage == "DISCOVERY") {
                std::cout << "Received DISCOVERY request from: " << senderEndpoint.address().to_string() << std::endl;

                // Synchronously send OFFER response
                std::string offerMessage = "OFFER:" + std::to_string(listeningPort);
                socket.send_to(boost::asio::buffer(offerMessage), senderEndpoint, 0, error);
                if (!error) {
                    std::cout << "Sent OFFER response to: " << senderEndpoint.address().to_string() << std::endl;
                }
                else {
                    std::cerr << "Error sending OFFER response: " << error.message() << std::endl;
                }
            }
        }
        else {
            std::cerr << "Error receiving DISCOVERY request: " << error.message() << std::endl;
        }
    }
}