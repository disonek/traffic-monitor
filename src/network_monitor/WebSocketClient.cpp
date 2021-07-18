#include "WebSocketClient.hpp"

#include <chrono>

#include "Log.hpp"

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

namespace NetworkMonitor {

WebSocketClient::WebSocketClient(const std::string& url, const std::string& port, boost::asio::io_context& ioc)
    : url_{url}
    , port_{port}
    , resolver_{boost::asio::make_strand(ioc)}
    , ws_{boost::asio::make_strand(ioc)}
{
}

void WebSocketClient::Connect(std::function<void(boost::system::error_code)> onConnect,
                              std::function<void(boost::system::error_code, std::string&&)> onMessage,
                              std::function<void(boost::system::error_code)> onDisconnect)
{
    onConnect_ = onConnect;
    onMessage_ = onMessage;
    onDisconnect_ = onDisconnect;

    closed_ = false;

    resolver_.async_resolve(url_, port_, [this](auto ec, auto resolverIt) { OnResolve(ec, resolverIt); });
}

void WebSocketClient::Close(std::function<void(boost::system::error_code)> onClose)
{
    closed_ = true;
    ws_.async_close(websocket::close_code::none, [onClose](auto ec) {
        if(onClose)
        {
            onClose(ec);
        }
    });
}

void WebSocketClient::Send(const std::string& message, std::function<void(boost::system::error_code)> onSend)
{
    ws_.async_write(boost::asio::buffer(message), [onSend](auto ec, auto) {
        if(onSend)
        {
            onSend(ec);
        }
    });
}

void WebSocketClient::OnResolve(const boost::system::error_code& ec,
                                boost::asio::ip::tcp::resolver::iterator resolverIt)
{
    if(ec)
    {
        Log(__func__, ec);
        if(onConnect_)
        {
            onConnect_(ec);
        }
        return;
    }

    ws_.next_layer().expires_after(std::chrono::seconds(5));

    ws_.next_layer().async_connect(*resolverIt, [this](auto ec) { OnConnect(ec); });
}

void WebSocketClient::OnConnect(const boost::system::error_code& ec)
{
    if(ec)
    {
        Log(__func__, ec);
        if(onConnect_)
        {
            onConnect_(ec);
        }
        return;
    }

    ws_.next_layer().expires_never();
    ws_.set_option(websocket::stream_base::timeout::suggested(boost::beast::role_type::client));

    ws_.async_handshake(url_, "/", [this](auto ec) { OnHandshake(ec); });
}

void WebSocketClient::OnHandshake(const boost::system::error_code& ec)
{
    if(ec)
    {
        Log(__func__, ec);
        if(onConnect_)
        {
            onConnect_(ec);
        }
        return;
    }

    ws_.text(true);
    ListenToIncomingMessage(ec);

    if(onConnect_)
    {
        onConnect_(ec);
    }
}

void WebSocketClient::ListenToIncomingMessage(const boost::system::error_code& ec)
{
    if(ec == boost::asio::error::operation_aborted)
    {
        if(onDisconnect_ && !closed_)
        {
            onDisconnect_(ec);
        }
        return;
    }

    ws_.async_read(rBuffer_, [this](auto ec, auto nBytes) {
        OnRead(ec, nBytes);
        ListenToIncomingMessage(ec);
    });
}
void WebSocketClient::OnRead(const boost::system::error_code& ec, size_t nBytes)
{
    // We just ignore messages that failed to read.
    if(ec)
    {
        return;
    }

    // Parse the message and forward it to the user callback.
    // Note: This call is synchronous and will block the WebSocket strand.
    std::string message{boost::beast::buffers_to_string(rBuffer_.data())};
    rBuffer_.consume(nBytes);
    if(onMessage_)
    {
        onMessage_(ec, std::move(message));
    }
}
} // namespace NetworkMonitor