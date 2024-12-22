#include "client/network_client.h"

#include <boost/uuid/uuid_io.hpp>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include "utility/logger.h"

#include "game/animals.h"
#include "game/crops.h"
#include "game/crop_production.h"
#include "game/farm_house.h"
#include "game/worldtime.h"

WorldTime* GlobalTime = new WorldTime;
Weather* GlobalWeather = new Weather;
CropProduction* GlobalCropProduction = new CropProduction;
FarmHouse* GlobalFarmHouse = new FarmHouse;


namespace th_valley {

void NetworkClient::Connect(const std::string_view host,
                            const std::string_view port) {
    const std::lock_guard<std::mutex> lock(mutex_);
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
                ReceiveMessages();  // Start receiving messages from the server.
            }
        });

    // Run the io_context to start the asynchronous operations.
    // Run the io_context in a separate thread to prevent blocking main thread.
    io_context_thread_ = std::thread([this]() { io_context_.run(); });
    Logger::GetInstance().LogInfo("Client: Connection established.");
}

void NetworkClient::Disconnect() {
    const std::lock_guard<std::mutex> lock(mutex_);
    Logger::GetInstance().LogInfo("Client: Disconnecting from server.");

    if (socket_.is_open()) {
        boost::system::error_code error_code;
        // Shutdown the socket to stop the asynchronous operations.
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                         error_code);
        if (error_code) {
            Logger::GetInstance().LogError(
                "Client: Socket shutdown failed with error: {}.",
                error_code.message());
        }

        // Close the socket to release the resources.
        socket_.close(error_code);
        if (error_code) {
            Logger::GetInstance().LogError(
                "Client: Socket close failed with error: {}.",
                error_code.message());
        }
    } else {
        // Handle the case when the socket is already closed.
        Logger::GetInstance().LogError("Client: Socket already closed.");
    }

    // Stop the io_context to stop the asynchronous operations.
    io_context_.stop();
    if (io_context_thread_.joinable()) {
        io_context_thread_.join();
    }

    // Set the connected flag to false.
    connected_ = false;
    Logger::GetInstance().LogInfo("Client: Disconnected from server.");
}

void NetworkClient::SendMessages(const std::string_view message_sv) {
    const std::lock_guard<std::mutex> lock(mutex_);

    // Append a newline character to the message.
    sent_message_.clear();
    sent_message_ = std::string(message_sv) + "\n";
    Logger::GetInstance().LogInfo("Client: Sending message: {}.", message_sv);

    // Send the message asynchronously.
    boost::asio::async_write(
        socket_, boost::asio::buffer(sent_message_, sent_message_.size()),
        [this](const boost::system::error_code& error_code,
               std::size_t /* length */) -> void {
            if (!error_code) {
                Logger::GetInstance().LogInfo(
                    "Client: Sent message: {} to server.",
                    sent_message_.substr(0, sent_message_.size() - 1));
            } else {
                Logger::GetInstance().LogError(
                    "Client: Send failed with error: {}.",
                    error_code.message());
            }
        });

    Logger::GetInstance().LogInfo("Client: Message sent initiated.");
}

void NetworkClient::ReceiveMessages() {
    const std::lock_guard<std::mutex> lock(mutex_);
    Logger::GetInstance().LogInfo("Client: Receiving messages.");

    // Receive the message asynchronously until a newline character is found.
    boost::asio::async_read_until(
        socket_, buffer_, '\n',
        [this](const boost::system::error_code& error_code,
               std::size_t /* length */) -> void {
            if (!error_code) {
                std::istream input_stream(&buffer_);
                received_message_.clear();
                std::getline(input_stream, received_message_);
                Logger::GetInstance().LogInfo("Client: Received: {}.",
                                              received_message_);
                callback_(received_message_);
                ReceiveMessages();  // Continue receiving messages.
            } else if (error_code == boost::asio::error::eof) {
                Logger::GetInstance().LogInfo(
                    "Client: Server disconnected (EOF).");
                Disconnect();  // Disconnect from the server if EOF is received.
            } else {
                Logger::GetInstance().LogError(
                    "Client: Receive failed with error: {}.",
                    error_code.message());
            }
        });

    Logger::GetInstance().LogInfo("Client: Receive initiated.");
}

void NetworkClient::SetCallback(
    std::function<void(std::string_view)> callback) {
    const std::lock_guard<std::mutex> lock(mutex_);

    // Set the callback function to handle the received messages.
    Logger::GetInstance().LogInfo("Client: Setting callback.");
    callback_ = std::move(callback);
    Logger::GetInstance().LogInfo("Client: Callback set.");
}

void NetworkClient::SetUUID(boost::uuids::uuid uuid) {
    const std::lock_guard<std::mutex> lock(mutex_);
    uuid_ = uuid;
}

boost::uuids::uuid NetworkClient::GetUUID() const {
    const std::lock_guard<std::mutex> lock(mutex_);
    return uuid_;
}

NetworkClient& NetworkClient::GetInstance() {
    static NetworkClient instance;
    return instance;
}

NetworkClient::NetworkClient() : socket_(io_context_), resolver_(io_context_) {
    Logger::GetInstance().LogInfo("Client: NetworkClient constructor.");
}

NetworkClient::~NetworkClient() {
    const std::lock_guard<std::mutex> lock(mutex_);
    Logger::GetInstance().LogInfo("Client: NetworkClient destructor.");
    // Ensure the client is disconnected before destruction.
    if (connected_) {
        Disconnect();
    }
    Logger::GetInstance().LogInfo("Client: NetworkClient destructed.");
}

}  // namespace th_valley
