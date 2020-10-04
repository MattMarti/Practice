/*
Daytime 1 - A synchronous TCP daytime client

How to use asio to implement a client application with TCP
*/

#include <iostream>
#include <array>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    using namespace std;

    // This application is used to access the server, so we need to specify the server
    try {
        boost::asio::io_context io;

        // Turn the specified server name into a TCP endpoint. A resolver takes a host
        // name and service name and turns them into a list of endpoints.
        tcp::resolver resolver(io);

        // Obtain the address for the server. This should be known. It can also be
        // found using the resolve function, but that take some time to run
        auto address = boost::asio::ip::address_v4::from_string("127.0.0.1");
        auto endpoint = tcp::endpoint(address, 5555);

        // Now create and connect the socket
        tcp::socket socket(io);
        socket.connect(endpoint);

        // Now that the connection is open, we need to read the response from the
        // daytime service
        for (;;) {
            std::array<char, 1024> buf;
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