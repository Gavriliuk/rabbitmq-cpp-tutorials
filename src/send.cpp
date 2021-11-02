#include <iostream>

#include "SimplePocoHandler.h"

int main(void)
{
    SimplePocoHandler handler("localhost", 5672);

    AMQP::Connection connection(&handler, AMQP::Login("guest", "guest"), "/");

    AMQP::Channel channel(&connection);

    AMQP::SuccessCallback onChannelReady =
            [&]()
            {
                if (handler.connected())
                {
                    channel.publish("", "hello", "Hello World!");
                    std::cout << " [x] Sent 'Hello World!'" << std::endl << std::flush;
                    handler.quit();
                }
            };

    channel.onReady(onChannelReady);

    handler.loop();

    return 0;
}
