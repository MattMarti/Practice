/*
Daytime 2 - A synchronous TCP Daytime server

How to implement a server application in TCP
*/

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

// Define make_daytime_string() to create the string to send back to the client
std::string make_daytime_string() {
    using namespace std;
    time_t now = time(0);
    return "Hello client!";
}

int main() {
    using boost::asio::ip::tcp;

    try {
        boost::asio::io_context io;

        // An acceptor object is created to listen for new connections
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));

        // This is an iterative server, so it handles only one connection at time
        for (;;) {
            tcp::socket socket(io);
            acceptor.accept(socket);

            // Now a client is listening to our service. Determine the current time and
            // transfer it to the client
            std::string message = make_daytime_string();
            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}