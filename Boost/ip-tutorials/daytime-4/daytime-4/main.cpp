/*
Daytime 4 - A synchronous UDP daytime client

This shows how to use asio to implement a client with UDP
*/

#include <iostream>
#include <array>
#include <boost/asio.hpp>

int main(int argc, char* argv[]) {
    using boost::asio::ip::udp;

    try {
        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
        }
        boost::asio::io_context io;

        // Use a UDP Resolver object to find the correct remote endpoint based on host
        // and service names. Returns only IPv4 endpoints by our argument
        udp::resolver resolver(io);
        udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), argv[1], "daytime").begin();

        // The resolve function is guaranteed to return at least one endpoint if it doesn't fail.
        // This means it's safe to dereference the return value directly.
        udp::socket socket(io);
        socket.open(udp::v4());

        std::array<char, 1> send_buf = { {0} };
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        std::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;

        /*
        Since UDP is datagram-oriented, we will not be using a stream socket. Create an
        ip::udp::socket and initiate contact with the remote endpoint.

        Now we need to be ready to accept whatever the server sends back to us. The endpoint 
        on our side that receives the server's response will be initialised by
        ip::udp::socket::receive_from().
        */
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        std::cout.write(recv_buf.data(), len);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}