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
    const std::string endpoint{"/"};
    const std::string port{"443"};
    const std::string message{"Hello WebSocket"};

    boost::asio::ssl::context ctx{boost::asio::ssl::context::tlsv12_client};
    ctx.load_verify_file(TESTS_CACERT_PEM);

    // Always start with an I/O context object.
    boost::asio::io_context ioc{};

    // The class under test
    WebSocketClient client{url, endpoint, port, ioc, ctx};

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

bool CheckResponse(const std::string& response)
{
    // We do not parse the whole message. We only check that it contains some
    // expected items.
    bool ok{true};
    ok &= response.find("ERROR") != std::string::npos;
    ok &= response.find("ValidationInvalidAuth") != std::string::npos;
    return ok;
}

TEST(NetworkMonitorTest, stomp)
{
    // Connection targets
    const std::string url{"ltnm.learncppthroughprojects.com"};
    const std::string endpoint{"/network-events"};
    const std::string port{"443"};

    // STOMP frame
    const std::string username{"fake_username"};
    const std::string password{"fake_password"};
    // 3b7cf9ecf4eaa7a5359cb203675f1e7ba2e36e39
    std::stringstream ss{};
    ss << "STOMP" << std::endl
       << "accept-version:1.2" << std::endl
       << "host:transportforlondon.com" << std::endl
       << "login:" << username << std::endl
       << "passcode:" << password << std::endl
       << std::endl // Headers need to be followed by a blank line.
       << '\0';     // The body (even if absent) must be followed by a NULL octet.
    const std::string message{ss.str()};

    // TLS context
    boost::asio::ssl::context ctx{boost::asio::ssl::context::tlsv12_client};
    ctx.load_verify_file(TESTS_CACERT_PEM);

    // Always start with an I/O context object.
    boost::asio::io_context ioc{};

    // The class under test
    WebSocketClient client{url, endpoint, port, ioc, ctx};

    // We use these flags to check that the connection, send, receive functions
    // work as expected.
    bool connected{false};
    bool messageSent{false};
    bool messageReceived{false};
    bool disconnected{false};
    std::string response{};

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
    auto onReceive{[&client, &onClose, &messageReceived, &response](auto ec, auto received) {
        messageReceived = !ec;
        response = std::move(received);
        client.Close(onClose);
    }};

    // We must call io_context::run for asynchronous callbacks to run.
    client.Connect(onConnect, onReceive);
    ioc.run();

    EXPECT_TRUE(connected);
    EXPECT_TRUE(messageSent);
    EXPECT_TRUE(messageReceived);
    EXPECT_TRUE(disconnected);
    EXPECT_TRUE(CheckResponse(response));
}
