/*
Daytime 6 - An asynchronous UDP daytime server
*/

#include <iostream>
#include <string>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string() {
    return "Hello client!";
}

class My_UDP_Server {
public:
    My_UDP_Server(boost::asio::io_context& io) : socket_(io, udp::endpoint(udp::v4(), 13)) {
        start_receive();
    }
private:
    void start_receive() {
        socket_.async_receive_from(boost::asio::buffer(recv_buffer_),
            remote_endpoint_,
            boost::bind(&My_UDP_Server::handle_receive,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error, std::size_t /*bytes_transferred*/) {
        if (!error) {
            boost::shared_ptr<std::string> message(new std::string(make_daytime_string()));

            // Now call the send function to serve data to the client
            socket_.async_send_to(boost::asio::buffer(*message),
                remote_endpoint_,
                boost::bind(&My_UDP_Server::handle_send,
                    this,
                    message,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            start_receive();
        }
    }

    void handle_send(boost::shared_ptr<std::string> /*message*/,
        const boost::system::error_code& /*error*/,
        std::size_t /*bytes_transferred*/) {}

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1> recv_buffer_;
};

int main() {
    try {
        boost::asio::io_context io;
        My_UDP_Server server(io);
        io.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}