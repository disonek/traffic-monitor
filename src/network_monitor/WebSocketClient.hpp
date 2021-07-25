#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/system/error_code.hpp>
#include <functional>
#include <string>

namespace NetworkMonitor {

class WebSocketClient
{
public:
    WebSocketClient(const std::string& url,
                    const std::string& port,
                    boost::asio::io_context& ioc,
                    boost::asio::ssl::context& ctx);

    void Connect(std::function<void(boost::system::error_code)> onConnect = nullptr,
                 std::function<void(boost::system::error_code, std::string&&)> onMessage = nullptr,
                 std::function<void(boost::system::error_code)> onDisconnect = nullptr);

    void Send(const std::string& message, std::function<void(boost::system::error_code)> onSend = nullptr);
    void Close(std::function<void(boost::system::error_code)> onClose = nullptr);

private:
    void OnResolve(const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator resolverIt);
    void OnConnect(const boost::system::error_code& ec);
    void OnHandshake(const boost::system::error_code& ec);
    void OnTlsHandshake(const boost::system::error_code& ec);
    void ListenToIncomingMessage(const boost::system::error_code& ec);
    void OnRead(const boost::system::error_code& ec, size_t nBytes);

    std::string url_{};
    std::string port_{};

    boost::asio::ip::tcp::resolver resolver_;
    boost::beast::websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream> > ws_;

    boost::beast::flat_buffer rBuffer_{};
    bool closed_{true};

    std::function<void(boost::system::error_code)> onConnect_{nullptr};
    std::function<void(boost::system::error_code, std::string&&)> onMessage_{nullptr};
    std::function<void(boost::system::error_code)> onDisconnect_{nullptr};
};

} // namespace NetworkMonitor