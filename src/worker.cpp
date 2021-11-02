#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

#include "SimplePocoHandler.h"

int main(void)
{
    SimplePocoHandler handler("localhost", 5672);

    AMQP::Connection connection(&handler, AMQP::Login("guest", "guest"), "/");

    AMQP::Channel channel(&connection);

    channel.setQos(1);

    channel.declareQueue("task_queue", AMQP::durable);

    AMQP::MessageCallback onMessageReceived =
            [&channel](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
            {
                const std::string body = message.message();
                std::cout << " [x] Received " << body << std::endl << std::flush;

                size_t count = std::count(body.cbegin(), body.cend(), '.');
                std::this_thread::sleep_for(std::chrono::seconds(count));

                std::cout << " [x] Done" << std::endl << std::flush;
                channel.ack(deliveryTag);
            };

    channel.consume("task_queue").onReceived(onMessageReceived);

    std::cout << " [*] Waiting for messages. To exit press CTRL-C" << std::endl << std::flush;

    handler.loop();

    return 0;
}
