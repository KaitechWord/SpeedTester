#include <iostream>
#include "./Server/Server.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>

int main(){
    //boost::log::add_file_log("ServerLogs.log");
    int port;
    std::cout << "Provide the port number: " << std::endl;
    std::cin >> port;
    Server server(port);
    return 0;
}