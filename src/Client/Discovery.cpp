#include "Discovery.h"

Discovery::Discovery(const std::string& multicastAddress, unsigned short multicastPort)
    : multicastEndpoint(boost::asio::ip::address::from_string(multicastAddress), multicastPort)
    , socket(ioService, multicastEndpoint.protocol())
{
    // Join the multicast group
    socket.set_option(boost::asio::ip::multicast::join_group(multicastEndpoint.address()));
}

void Discovery::sendDiscover() {
    while (true) {
        try {
            // Synchronously send DISCOVER command
            std::string discoverMessage = "DISCOVER";
            socket.send_to(boost::asio::buffer(discoverMessage), multicastEndpoint);

            // Wait for OFFER:PORT response
            boost::system::error_code error;
            boost::asio::ip::udp::endpoint senderEndpoint;
            std::array<char, 1024> receiveBuffer;
            std::size_t bytesRead = socket.receive_from(boost::asio::buffer(receiveBuffer), senderEndpoint, 0, error);

            if (!error) {
                std::string receivedMessage(receiveBuffer.data(), bytesRead);

                // Process the received message (assuming it's an OFFER response)
                if (receivedMessage.find("OFFER:") == 0) {
                    std::string serverAddress = senderEndpoint.address().to_string();
                    std::string portStr = receivedMessage.substr(6);
                    unsigned short serverPort = std::stoi(portStr);

                    // Process the server information (e.g., add to a list)
                    std::cout << "Found server: " << serverAddress << ":" << serverPort << std::endl;
                }
            }
            else {
                std::cerr << "Error receiving OFFER response: " << error.message() << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Exception in discoverThread: " << e.what() << std::endl;
        }
    }
}