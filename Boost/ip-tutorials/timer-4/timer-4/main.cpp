/*
Timer 4- Using a member function as a handler

How to use a class member function as a callback handler. This is identical in
function to timer-3 example.
*/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

class My_Printer {
    boost::asio::steady_timer timer_;
    int count_;
public:
    My_Printer(boost::asio::io_context& io) :
            timer_(io, boost::asio::chrono::seconds(1)),
            count_(0) {
        timer_.async_wait(boost::bind(&My_Printer::print, this));
    }

    ~My_Printer() {
        std::cout << "Final count is " << count_ << std::endl;
    }

    void print() {
        if (count_ < 5) {
            std::cout << count_ << std::endl;
            ++count_;

            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
            timer_.async_wait(boost::bind(&My_Printer::print, this));
        }
    }
};

int main() {
    boost::asio::io_context io;
    My_Printer p(io);
    io.run();

    return 0;
}