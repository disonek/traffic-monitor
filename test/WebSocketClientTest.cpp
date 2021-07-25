#include <gtest/gtest.h>

#include <boost/asio.hpp>
#include <filesystem>
#include <iostream>
#include <string>

#include "WebSocketClient.hpp"

using namespace testing;
using NetworkMonitor::WebSocketClient;

TEST(NetworkMonitorTest, basicTest)
{
    // Connection targets
    const std::string url{"echo.websocket.org"};
    const std::string port{"443"};
    const std::string message{"Hello WebSocket"};

    boost::asio::ssl::context ctx{boost::asio::ssl::context::tlsv12_client};
    ctx.load_verify_file(TESTS_CACERT_PEM);

    // Always start with an I/O context object.
    boost::asio::io_context ioc{};

    // The class under test
    WebSocketClient client{url, port, ioc, ctx};

    // We use these flags to check that the connection, send, receive functions
    // work as expected.
    bool connected{false};
    bool messageSent{false};
    bool messageReceived{false};
    bool disconnected{false};
    std::string echo{};

    // Our own callbacks
    auto onSend{[&messageSent](auto ec) { messageSent = !ec; }};

    auto onConnect{[&client, &connected, &onSend, &message](auto ec) {
        connected = !ec;
        if(!ec)
        {
            client.Send(message, onSend);
        }
    }};

    auto onClose{[&disconnected](auto ec) { disconnected = !ec; }};

    auto onReceive{[&client, &onClose, &messageReceived, &echo, &message](auto ec, auto received) {
        messageReceived = !ec;
        echo = message;
        client.Close(onClose);
    }};

    // We must call io_context::run for asynchronous callbacks to run.
    client.Connect(onConnect, onReceive);
    ioc.run();

    EXPECT_TRUE(connected);
    EXPECT_TRUE(messageSent);
    EXPECT_TRUE(messageReceived);
    EXPECT_TRUE(disconnected);
    EXPECT_EQ(echo, message);
}

TEST(NetworkMonitorTest, cert)
{
    EXPECT_TRUE(std::filesystem::exists(TESTS_CACERT_PEM));
}