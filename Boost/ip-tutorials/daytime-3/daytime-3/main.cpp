/*
Daytime 3 - An Asynchronous TCP Daytime Server
*/

#include <iostream>
#include <string>

#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

std::string make_daytime_string() {
    return "Hello client!";
}

// This is defined with a shared pointer because we want to keep the object alive as long
// as there is an operation that refers to it
class My_TCP_Connection : public boost::enable_shared_from_this<My_TCP_Connection> {
    boost::asio::ip::tcp::socket socket_;
    std::string message_;
public:
    typedef boost::shared_ptr<My_TCP_Connection> pointer;

    static pointer create(boost::asio::io_context& io) {
        return pointer(new My_TCP_Connection(io));
    }

    boost::asio::ip::tcp::socket& socket() {
        return socket_;
    }

    void start() {
        // Store the message  and keep data valid until the async operation is done
        message_ = make_daytime_string();
        
        // You must specify only the args that match the handler's parameter list.
        boost::asio::async_write(socket_,
            boost::asio::buffer(message_),
            boost::bind(&My_TCP_Connection::handle_write,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
private:
    My_TCP_Connection(boost::asio::io_context& io) : socket_(io) {}

    void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transfered*/) {}
};

class My_TCP_Server {
    boost::asio::io_context& io_;
    boost::asio::ip::tcp::acceptor acceptor_;
public:
    My_TCP_Server(boost::asio::io_context& io) : 
            io_(io), 
            acceptor_(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 13)) {
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

    void handle_accept(My_TCP_Connection::pointer new_connection, const boost::system::error_code& error) {
        if (!error) {
            new_connection->start();
        }
        start_accept();
    }
};

int main() {

    try {
        boost::asio::io_context io;
        My_TCP_Server server(io);
        io.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}