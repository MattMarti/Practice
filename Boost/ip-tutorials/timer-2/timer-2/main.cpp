/*
Timer 2 - Using a timer asynchronously

This demonstrates how to use the asynchronous callback functionality.

*/

#include <iostream>
#include <boost/asio.hpp>

// Define a callback function that gets called when the operation completes
void my_print(const boost::system::error_code& /*e*/) {
    std::cout << "Timer completed!" << std::endl;
}

int main() {
    using namespace std;

    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

    t.async_wait(&my_print);
    io.run(); // This makes the io object work. Without it, the timer would not wait


    return 0;
}