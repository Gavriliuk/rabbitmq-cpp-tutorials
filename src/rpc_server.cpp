#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

#include "SimplePocoHandler.h"

int fib(int n)
{
    if (n < 2)
    {
        return n ? 1 : 0;
    }

    return fib(n - 1) + fib(n - 2);
}

int main(void)
{
    SimplePocoHandler handler("localhost", 5672);

    AMQP::Connection connection(&handler, AMQP::Login("guest", "guest"), "/");

    AMQP::Channel channel(&connection);

    channel.setQos(1);

    channel.declareQueue("rpc_queue");

    AMQP::MessageCallback onMessageReceived =
            [&channel](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
            {
                const std::string request(message.message());
                std::cout << " [x] Request: " << request << std::endl << std::flush;

                const std::string response(std::to_string(fib(std::stoi(request))));
                std::cout << " [x] Response: " << response << std::endl << std::flush;

                AMQP::Envelope env(response);
                env.setCorrelationID(message.correlationID());

                channel.publish("", message.replyTo(), env);
                channel.ack(deliveryTag);
            };

    channel.consume("").onReceived(onMessageReceived);

    std::cout << " [x] Awaiting RPC requests" << std::endl << std::flush;

    handler.loop();

    return 0;
}
