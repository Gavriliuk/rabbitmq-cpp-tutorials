#include <iostream>

#include "SimplePocoHandler.h"

int main(void)
{
    SimplePocoHandler handler("localhost", 5672);

    AMQP::Connection connection(&handler, AMQP::Login("guest", "guest"), "/");

    AMQP::Channel channel(&connection);

    channel.declareQueue("hello");

    AMQP::MessageCallback onMessageReceived =
            [](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
            {
                std::cout << " [x] Received " << message.message() << std::endl << std::flush;
            };

    channel.consume("hello", AMQP::noack).onReceived(onMessageReceived);

    std::cout << " [*] Waiting for messages. To exit press CTRL-C" << std::endl << std::flush;

    handler.loop();

    return 0;
}
