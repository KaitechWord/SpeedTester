#include <iostream>
#include "./Server/Server.h"

int main(){
    int port = 7777;
    /*std::cout << "Provide the port number: " << std::endl;
    std::cin >> port;*/
    Server server(port);
    return 0;
}