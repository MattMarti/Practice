/*
Timer 5 - Synchronising handlers in multithreaded programs

This tutorial demonstrates the strand class template to sync callback handlers
in a multithreaded program. Basically we're going to pool threads in io_context::run()
*/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>

class My_Printer {
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
public:
    My_Printer(boost::asio::io_context& io) :
            strand_(boost::asio::make_strand(io)),
            timer1_(io, boost::asio::chrono::seconds(1)),
            timer2_(io, boost::asio::chrono::seconds(2)),
            count_(0) {

        // Each callback handler is bound to a strand object. The bind_executor function
        // returns a new handler that automatically dispatches its contained handler through
        // the strand object. By binding to the same strand, we ensure that they cannot 
        // execute concurrently
        timer1_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&My_Printer::print1, this)));
        timer2_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&My_Printer::print2, this)));
    }

    ~My_Printer() {
        std::cout << "Final count is " << count_ << std::endl;
    }

    /*
    In a multithreaded program, the handlers for asynchronous operations should be synced if
    they access shared resources. In this tutorial, the shared resource is cout and count_.
    */

    void print1() {
        if (count_ < 10) {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));
            timer1_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&My_Printer::print1, this)));
        }
    }

    void print2() {
        if (count_ < 10) {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(2));
            timer2_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&My_Printer::print2, this)));
        }
    }
};

int main() {
    boost::asio::io_context io;
    My_Printer p(io);
    boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
    io.run();
    t.join();

    return 0;
}