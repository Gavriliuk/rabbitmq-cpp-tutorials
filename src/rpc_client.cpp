#include <iostream>

#include "SimplePocoHandler.h"

#include "tools.h"

int main(int argc, const char* argv[])
{
    const std::string correlation(uuid());

    SimplePocoHandler handler("localhost", 5672);

    AMQP::Connection connection(&handler, AMQP::Login("guest", "guest"), "/");

    AMQP::Channel channel(&connection);

    AMQP::QueueCallback onQueueDeclared =
            [&](const std::string &name, int msgCount, int consumerCount)
            {
                AMQP::Envelope env("30");
                env.setCorrelationID(correlation);
                env.setReplyTo(name);
                channel.publish("", "rpc_queue", env);
                std::cout << " [*] Requesting fib(30)" << std::endl << std::flush;
            };

    channel.declareQueue(AMQP::exclusive).onSuccess(onQueueDeclared);

    AMQP::MessageCallback onMessageReceived =
            [&](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
            {
                if (message.correlationID() == correlation)
                {
                    std::cout << " [*] Got " << message.message() << std::endl << std::flush;
                    handler.quit();
                }
            };

    channel.consume("", AMQP::noack).onReceived(onMessageReceived);

    handler.loop();

    return 0;
}
