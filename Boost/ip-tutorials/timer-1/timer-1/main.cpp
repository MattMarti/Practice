/*
Timer 1 - Using a timer synchornously

This tutorial introduces asio by showing how to perform a blocking
wait on a timer
*/

#include <iostream>
#include <boost/asio.hpp>

int main() {

    // All programs using asio must have at least one io_context object.
    boost::asio::io_context io;

    // Declare a steady timer which will expire in 5 seconds
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

    // Perform a blocking wait on the timer. Calling wait will not return until the
    // timer expires
    std::cout << "Waiting ..." << std::endl;
    t.wait();
    std::cout << "Finished!" << std::endl;

    return 0;
}