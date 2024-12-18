#include "client/network_client.h"

#include <boost/uuid/uuid_io.hpp>
#include <string>
#include <utility>

#include "utility/logger.h"

namespace th_valley {

void NetworkClient::Connect(std::string_view host, std::string_view port) {
    // Restart the io_context to clear any previous state.
    io_context_.restart();

    // Resolve the host name into an IP address.
    boost::asio::ip::tcp::resolver::query query(host.data(), port.data());
    endpoints_ = resolver_.resolve(query);

    // Connect to the server asynchronously.
    boost::asio::async_connect(
        socket_, endpoints_,
        [this](const boost::system::error_code& error_code,
               const boost::asio::ip::tcp::endpoint& endpoint) -> void {
            if (error_code) {
                Logger::GetInstance().LogError("Connect failed: " +
                                               error_code.message());
            } else {
                Logger::GetInstance().LogInfo("Connected to: " +
                                              endpoint.address().to_string());
                SendMessages(boost::uuids::to_string(uuid_));
                ReceiveMessages();
            }
        });

    // Run the io_context to start the asynchronous operations.
    io_context_.run();
}

void NetworkClient::Disconnect() {
    boost::system::error_code error_code;
    if (socket_.is_open()) {
        // Shutdown the socket
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                         error_code);
        if (error_code) {
            Logger::GetInstance().LogError(
                "Socket shutdown failed with error: " + error_code.message());
        }

        // Close the socket
        socket_.close(error_code);
        if (error_code) {
            Logger::GetInstance().LogError("Socket close failed with error: " +
                                           error_code.message());
        }
    } else {
        Logger::GetInstance().LogError("Socket is already closed.");
    }

    // Stop the io_context to stop the asynchronous operations.
    io_context_.stop();
}

void NetworkClient::SendMessages(std::string_view message_sv) {
    std::string message = std::string(message_sv) + "\n";
    boost::asio::async_write(
        socket_, boost::asio::buffer(message, message.size()),
        [this, message](const boost::system::error_code& error_code,
                        std::size_t /*length*/) -> void {
            if (error_code) {
                Logger::GetInstance().LogError("Write failed: " +
                                               error_code.message());
            } else {
                Logger::GetInstance().LogInfo("Sent: " + message);
            }
        });
}

void NetworkClient::ReceiveMessages() {
    boost::asio::async_read_until(
        socket_, buffer_, '\n',
        [this](const boost::system::error_code& error_code,
               std::size_t /*length*/) -> void {
            if (error_code) {
                Logger::GetInstance().LogError("Read failed: " +
                                               error_code.message());
            } else {
                std::istream input_stream(&buffer_);
                std::string message;
                std::getline(input_stream, message);
                Logger::GetInstance().LogInfo("Received: " + message);
                callback_(message);
                ReceiveMessages();
            }
        });
}

void NetworkClient::SetCallback(
    std::function<void(std::string_view)> callback) {
    callback_ = std::move(callback);
}

void NetworkClient::SetUUID(boost::uuids::uuid uuid) { uuid_ = uuid; }

NetworkClient& NetworkClient::GetInstance() {
    static NetworkClient instance;
    return instance;
}

NetworkClient::NetworkClient() : socket_(io_context_), resolver_(io_context_) {}

}  // namespace th_valley
