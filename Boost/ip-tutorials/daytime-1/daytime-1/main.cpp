/*
Daytime 1 - A synchronous TCP daytime client

How to use asio to implement a client application with TCP
*/

#include <iostream>
#include <array>
#include <boost/asio.hpp>

int main(int argc, char* argv[]) {
    using boost::asio::ip::tcp;

    // This application is used to access a daytime service, so we need to specify the server
    try {
        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        boost::asio::io_context io;

        // Turn the specified server name into a TCP endpoint. A resolver takes a host
        // name and service name and turns them into a list of endpoints.
        tcp::resolver resolver(io);

        // Perform a resolve call using the name of the server, specified in argv[1], and
        // the name of the service, in this case "daytime"
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");

        // Now create and connect the socket. The list of endpoints obtained above 
        // may contain both IPv4 and IPv6 endpoints, so we need to try each until we
        // find one that works. This keeps the client independent of a specific
        // IP version
        tcp::socket socket(io);
        boost::asio::connect(socket, endpoints);

        // Now that the connection is open, we need to read the response from the
        // daytime service
        for (;;) {
            std::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            // When the server closes connection, the read_some() function will
            // exit with the asio::error, which is how we know to exit the loop
            if (error == boost::asio::error::eof) {
                break;
            }
            else if (error) {
                throw boost::system::system_error(error);
            }

            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}