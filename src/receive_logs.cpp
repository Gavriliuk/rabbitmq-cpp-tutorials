#include <iostream>

#include "SimplePocoHandler.h"

int main(void)
{
    SimplePocoHandler handler("localhost", 5672);

    AMQP::Connection connection(&handler, AMQP::Login("guest", "guest"), "/");

    AMQP::Channel channel(&connection);

    AMQP::MessageCallback onMessageReceived =
            [](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
            {
                std::cout << " [x] " << message.message() << std::endl << std::flush;
            };

    AMQP::QueueCallback onQueueDeclared =
            [&](const std::string &name, int msgcount, int consumercount)
            {
                channel.bindQueue("logs", name,"");
                channel.consume(name, AMQP::noack).onReceived(onMessageReceived);
            };

    AMQP::SuccessCallback onExchangeDeclared =
            [&]()
            {
                channel.declareQueue(AMQP::exclusive).onSuccess(onQueueDeclared);
            };

    channel.declareExchange("logs", AMQP::fanout).onSuccess(onExchangeDeclared);

    std::cout << " [*] Waiting for messages. To exit press CTRL-C" << std::endl << std::flush;

    handler.loop();

    return 0;
}
