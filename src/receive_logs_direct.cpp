#include <iostream>
#include <algorithm>

#include "SimplePocoHandler.h"

#include "tools.h"

int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " [info] [warning] [error]" << std::endl << std::flush;
        return 1;
    }

    SimplePocoHandler handler("localhost", 5672);

    AMQP::Connection connection(&handler, AMQP::Login("guest", "guest"), "/");

    AMQP::Channel channel(&connection);

    channel.declareExchange("direct_logs", AMQP::direct);

    AMQP::MessageCallback onMessageReceived =
            [&](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
            {
                std::cout << " [" << argc - 1 << "] " << message.routingKey() << ": " << message.message() << std::endl << std::flush;
            };

    AMQP::QueueCallback onQueueDeclared =
            [&](const std::string &name, int msgCount, int consumerCount)
            {
                std::for_each(&argv[1], &argv[argc],
                        [&](const char* severity)
                        {
                            channel.bindQueue("direct_logs", "", severity);
                            channel.consume(name, AMQP::noack).onReceived(onMessageReceived);
                        });
            };

    channel.declareQueue(AMQP::exclusive).onSuccess(onQueueDeclared);

    std::cout << " [" << argc - 1 << "] Waiting for " << join(&argv[1], &argv[argc], ", ") << " messages. To exit press CTRL-C" << std::endl << std::flush;

    handler.loop();

    return 0;
}
