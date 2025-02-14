#include <iostream>

#include "SimplePocoHandler.h"

#include "tools.h"

int main(int argc, const char* argv[])
{
    const std::string msg =
            argc > 1 ? join(&argv[1], &argv[argc], " ") : "info: Hello World!";

    SimplePocoHandler handler("localhost", 5672);

    AMQP::Connection connection(&handler, AMQP::Login("guest", "guest"), "/");

    AMQP::Channel channel(&connection);

    AMQP::SuccessCallback onExchangeDeclared =
            [&]()
            {
                channel.publish("logs", "", msg);
                std::cout << " [x] Sent " << msg << std::endl << std::flush;
                handler.quit();
            };

    channel.declareExchange("logs", AMQP::fanout).onSuccess(onExchangeDeclared);

    handler.loop();

    return 0;
}
