/*
Daytime 5 - A synchronous UDP daytime server
*/

#include <iostream>
#include <string>
#include <array>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string() {
    return "Hello client!";
}

int main() {
    try {
        boost::asio::io_context io;
        udp::socket socket(io, udp::endpoint(udp::v4(), 13));

        for (;;) {
            std::array<char, 1> recv_buf;
            udp::endpoint remote_endpoint;
            socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

            // Determine what to send back to the client
            std::string message = make_daytime_string();

            // Send response to remote endpoint
            boost::system::error_code ignored_error;
            socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, ignored_error);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}