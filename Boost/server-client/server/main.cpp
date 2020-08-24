/*
Daytime 3 - An Asynchronous TCP Daytime Server
*/

#include <iostream>
#include <string>
#include <chrono>
#include <memory>
#include <thread>

#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
    return "Hello client!\n";
}

// This is defined with a shared pointer because we want to keep the object alive as long
// as there is an operation that refers to it
class My_TCP_Connection : public std::enable_shared_from_this<My_TCP_Connection> {
    tcp::socket _socket;
    std::string _message;
public:
    typedef std::shared_ptr<My_TCP_Connection> Pointer;

    static Pointer create(boost::asio::io_context& io) {
        return Pointer(new My_TCP_Connection(io));
    }

    tcp::socket& socket() {
        return _socket;
    }

    void start() {
        // Store the message  and keep data valid until the async operation is done
        _message = make_daytime_string();

        // You must specify only the args that match the handler's parameter list.
        for (size_t i = 0; i < 10; i++) {
            boost::asio::async_write(_socket,
                boost::asio::buffer(_message),
                boost::bind(&My_TCP_Connection::handle_write,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        _socket.close();
    }
private:
    My_TCP_Connection(boost::asio::io_context& io) : _socket(io) {}

    void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transfered*/) {}
};

class My_TCP_Server {
    boost::asio::io_context& _io;
    tcp::acceptor _acceptor;
public:
    My_TCP_Server(boost::asio::io_context& io) :
            _io(io),
            _acceptor(io, tcp::endpoint(tcp::v4(), 5555)) {
        start_accept();
    }
private:
    void start_accept() {
        for (;;) {
            My_TCP_Connection::Pointer new_connection = My_TCP_Connection::create(_io);
            boost::system::error_code ec;
            _acceptor.accept(new_connection->socket(), ec);
            handle_accept(new_connection, ec);
        }
    }

    void handle_accept(My_TCP_Connection::Pointer new_connection, const boost::system::error_code& error) {
        if (!error) {
            std::thread(&My_TCP_Connection::start, new_connection).detach();
        }
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