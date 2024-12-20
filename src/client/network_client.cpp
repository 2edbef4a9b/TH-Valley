#include "client/network_client.h"

#include <boost/uuid/uuid_io.hpp>
#include <string>
#include <thread>
#include <utility>

#include "utility/logger.h"

namespace th_valley {

void NetworkClient::Connect(const std::string_view host,
                            const std::string_view port) {
    Logger::GetInstance().LogInfo("Client: Connecting to server at {}:{}.",
                                  host, port);

    // Resolve the host name into an IP address.
    const boost::asio::ip::tcp::resolver::results_type endpoints =
        resolver_.resolve(host, port);
    endpoints_ = endpoints;

    // Connect to the server asynchronously.
    boost::asio::async_connect(
        socket_, endpoints_,
        [this](const boost::system::error_code& error_code,
               const boost::asio::ip::tcp::endpoint& endpoint) -> void {
            if (error_code) {
                Logger::GetInstance().LogError(
                    "Client: Connection failed with error: {}.",
                    error_code.message());
            } else {
                Logger::GetInstance().LogInfo(
                    "Client: Connected to server at {}:{}.",
                    endpoint.address().to_string(), endpoint.port());
                connected_ = true;
                ReceiveMessages();
            }
        });

    // Run the io_context to start the asynchronous operations.
    io_context_thread_ = std::thread([this]() { io_context_.run(); });

    Logger::GetInstance().LogInfo("Client: Connection established.");
}

void NetworkClient::Disconnect() {
    Logger::GetInstance().LogInfo("Client: Disconnecting from server.");

    boost::system::error_code error_code;
    if (socket_.is_open()) {
        // Shutdown the socket
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                         error_code);
        if (error_code) {
            Logger::GetInstance().LogError(
                "Client: Socket shutdown failed with error: {}.",
                error_code.message());
        }

        // Close the socket
        socket_.close(error_code);
        if (error_code) {
            Logger::GetInstance().LogError(
                "Client: Socket close failed with error: {}.",
                error_code.message());
        }
    } else {
        Logger::GetInstance().LogError("Client: Socket already closed.");
    }

    // Stop the io_context to stop the asynchronous operations.
    io_context_.stop();
    if (io_context_thread_.joinable()) {
        io_context_thread_.join();
    }

    connected_ = false;
    Logger::GetInstance().LogInfo("Client: Disconnected from server.");
}

void NetworkClient::SendMessages(const std::string_view message_sv) {
    sent_message_ = std::string(message_sv) + "\n";
    Logger::GetInstance().LogInfo("Client: Sending message: {}.", message_sv);
    boost::asio::async_write(
        socket_, boost::asio::buffer(sent_message_, sent_message_.size()),
        [this](const boost::system::error_code& error_code,
               std::size_t /*length*/) -> void {
            if (error_code) {
                Logger::GetInstance().LogError(
                    "Client: Send failed with error: {}.",
                    error_code.message());
            } else {
                Logger::GetInstance().LogInfo(
                    "Client: Sent message: {} to server.",
                    sent_message_.substr(0, sent_message_.size() - 1));
            }
        });

    Logger::GetInstance().LogInfo("Client: Message sent initiated.");
}

void NetworkClient::ReceiveMessages() {
    Logger::GetInstance().LogInfo("Client: Receiving messages.");
    boost::asio::async_read_until(
        socket_, buffer_, '\n',
        [this](const boost::system::error_code& error_code,
               std::size_t /*length*/) -> void {
            if (error_code) {
                Logger::GetInstance().LogError(
                    "Client: Receive failed with error: {}.",
                    error_code.message());
            } else {
                std::istream input_stream(&buffer_);
                std::getline(input_stream, received_message_);
                Logger::GetInstance().LogInfo("Client: Received: {}.",
                                              received_message_);
                callback_(received_message_);
                ReceiveMessages();
            }
        });

    Logger::GetInstance().LogInfo("Client: Receive initiated.");
}

void NetworkClient::SetCallback(
    std::function<void(std::string_view)> callback) {
    Logger::GetInstance().LogInfo("Client: Setting callback.");
    callback_ = std::move(callback);
    Logger::GetInstance().LogInfo("Client: Callback set.");
}

void NetworkClient::SetUUID(boost::uuids::uuid uuid) { uuid_ = uuid; }

boost::uuids::uuid NetworkClient::GetUUID() const { return uuid_; }

NetworkClient& NetworkClient::GetInstance() {
    static NetworkClient instance;
    return instance;
}

NetworkClient::NetworkClient() : socket_(io_context_), resolver_(io_context_) {
    Logger::GetInstance().LogInfo("Client: NetworkClient constructor.");
}

NetworkClient::~NetworkClient() {
    Logger::GetInstance().LogInfo("Client: NetworkClient destructor.");
    if (connected_) {
        Disconnect();
    }
    Logger::GetInstance().LogInfo("Client: NetworkClient destructed.");
}

}  // namespace th_valley
