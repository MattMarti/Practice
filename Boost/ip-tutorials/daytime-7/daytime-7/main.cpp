/*
Daytime 7 - A combined TCP/UDP asynchronous server

This tutorial shows how to combine the UDP Async Server and the
TCP Async Server into a single server application. The classes
developed in the previous tutorials are pretty much the same.
*/

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio::ip;

std::string make_daytime_string() {
    return "Hello client!";
}

class My_TCP_Connection : public boost::enable_shared_from_this<My_TCP_Connection> {
public:
    typedef boost::shared_ptr<My_TCP_Connection> pointer;

    static pointer create(boost::asio::io_context& io) {
        return pointer(new My_TCP_Connection(io));
    }

    tcp::socket& socket() {
        return socket_;
    }

    void start() {
        message_ = make_daytime_string();
        boost::asio::async_write(socket_,
            boost::asio::buffer(message_),
            boost::bind(&My_TCP_Connection::handle_write,
                shared_from_this()));
    }
private:
    My_TCP_Connection(boost::asio::io_context& io) : socket_(io) {}
    void handle_write() {}
    tcp::socket socket_;
    std::string message_;
};

class My_TCP_Server {
public:
    My_TCP_Server(boost::asio::io_context& io) :
            io_(io),
            acceptor_(io, tcp::endpoint(tcp::v4(), 13)) {
        start_accept();
    }
private:
    void start_accept() {
        My_TCP_Connection::pointer new_connection = My_TCP_Connection::create(io_);
        acceptor_.async_accept(new_connection->socket(),
            boost::bind(&My_TCP_Server::handle_accept,
                this,
                new_connection,
                boost::asio::placeholders::error));
    }
    
    void handle_accept(My_TCP_Connection::pointer new_connection, 
            const boost::system::error_code& error) {
        if (!error) {
            new_connection->start();
        }
        start_accept();
    }

    boost::asio::io_context& io_;
    tcp::acceptor acceptor_;
};

class My_UDP_Server {
public:
    My_UDP_Server(boost::asio::io_context& io) :
            socket_(io, udp::endpoint(udp::v4(), 13)) {
        start_receive();
    }
private:
    void start_receive() {
        socket_.async_receive_from(boost::asio::buffer(recv_buffer_),
            remote_endpoint_,
            boost::bind(&My_UDP_Server::handle_receive,
                this,
                boost::asio::placeholders::error));
    }

    void handle_receive(const boost::system::error_code& error) {
        if (!error) {
            boost::shared_ptr<std::string> message(new std::string(make_daytime_string()));
            socket_.async_send_to(boost::asio::buffer(*message),
                remote_endpoint_,
                boost::bind(&My_UDP_Server::handle_send,
                    this,
                    message));
            start_receive();
        }
    }

    void handle_send(boost::shared_ptr<std::string> /*message*/) {}

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1> recv_buffer_;
};

int main() {
    try {
        boost::asio::io_context io;

        My_TCP_Server tcp_server(io);
        My_UDP_Server udp_server(io);

        io.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}