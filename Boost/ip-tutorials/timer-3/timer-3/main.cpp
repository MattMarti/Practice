/*
Timer 3 - Binding arguments to a handler

Modified from timer-2, where the timer fires once per second. Also, parameters
are passed to the handler function
*/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

void my_print(const boost::system::error_code& /*e*/,
    boost::asio::steady_timer* t, int* count) {

    // Count up to 5
    if (*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);

        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));

        // Add a second to the expiration timer. Since this adds a second to the previous
        // timer, it ensures that the timer lasts a whole second regardless of processing
        // time
        t->async_wait(boost::bind(my_print, boost::asio::placeholders::error, t, count));
    }
}

int main() {
    boost::asio::io_context io;

    int count = 0;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));

    t.async_wait(boost::bind(my_print, boost::asio::placeholders::error, &t, &count));
    io.run();

    std::cout << "Final count is " << count << std::endl;
    return 0;
}