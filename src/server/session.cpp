#include "server/session.h"

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "utility/logger.h"

namespace th_valley {

boost::uuids::uuid Session::GetUUID() const { return uuid_; }

boost::asio::ip::tcp::socket &Session::GetSocket() { return socket_; }

void Session::SetSessionManager(
    const std::shared_ptr<SessionManager> &session_manager) {
    session_manager_ = session_manager;
}

Session::Session(boost::asio::ip::tcp::socket socket)
    : socket_(std::move(socket)), uuid_(boost::uuids::random_generator()()) {}

void Session::Start() {
    Logger::GetInstance().LogInfo("Session: Starting a new session.");
    DoRead();  // Start reading from the client socket.
    Logger::GetInstance().LogInfo("Session: Started a new session.");
}

void Session::Terminate() {
    Logger::GetInstance().LogInfo("Session: Terminating with UUID: {}.",
                                  boost::uuids::to_string(uuid_));
    boost::system::error_code error_code;

    if (socket_.is_open()) {
        // Shutdown the socket
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                         error_code);
        if (error_code) {
            Logger::GetInstance().LogError(
                "Session: Socket shutdown failed with error: {}.",
                error_code.message());
        }

        // Close the socket
        socket_.close(error_code);
        if (error_code) {
            Logger::GetInstance().LogError(
                "Session: Socket close failed with error: {}.",
                error_code.message());
        }
    } else {
        Logger::GetInstance().LogError("Session: Socket already closed.");
    }

    Logger::GetInstance().LogInfo("Session: Terminated with UUID: {}.",
                                  boost::uuids::to_string(uuid_));
}

void Session::DoRead() {
    Logger::GetInstance().LogInfo("Session: Reading from client.");
    auto self(shared_from_this());
    // Start an asynchronous read operation.
    boost::asio::async_read_until(
        socket_, buffer_, '\n',
        [this, self](const boost::system::error_code &error_code,
                     const std::size_t length) {
            if (!error_code) {
                // Extract the message from the buffer.
                std::istream input_stream(&buffer_);
                std::string message;
                std::getline(input_stream, message);
                self->HandleMessage(message);
            } else if (error_code == boost::asio::error::eof) {
                // Handle the end of file error gracefully.
                Logger::GetInstance().LogInfo(
                    "Session: Client disconnected (EOF).");
                self->Terminate();
            } else {
                Logger::GetInstance().LogError(
                    "Session: Error reading from socket: {}.",
                    error_code.message());
                self->Terminate();
            }
        });

    Logger::GetInstance().LogInfo("Session: Reading from client initiated.");
}

void Session::DoWrite(const std::string_view message_sv) {
    // Convert the message to a string and append a newline for the protocol.
    std::string message(message_sv);
    std::string sent_message = message + "\n";
    Logger::GetInstance().LogInfo("Session: Writing messageL {} to client.",
                                  message);

    auto self(shared_from_this());
    // Start an asynchronous write operation.
    boost::asio::async_write(
        socket_, boost::asio::buffer(sent_message, sent_message.size()),
        [self, message](const boost::system::error_code &error_code,
                        const std::size_t /* length */) {
            if (!error_code) {
                Logger::GetInstance().LogInfo(
                    "Session: Sent message: {} to client.", message);
            } else {
                Logger::GetInstance().LogError(
                    "Session: Error writing to socket: {}.",
                    error_code.message());
                self->Terminate();
            }
        });

    Logger::GetInstance().LogInfo("Session: Writing to client initiated.");
}

void Session::HandleMessage(const std::string_view message_sv) {
    Logger::GetInstance().LogInfo("Session: Handling message.");
    // Process the message and send a response.
    if (IsUUIDValid(message_sv)) {
        Logger::GetInstance().LogInfo(
            "Session: Received valid UUID, updating.");
        auto old_uuid = uuid_;
        uuid_ = boost::uuids::string_generator()(std::string(message_sv));
        session_manager_.lock()->UpdateSessionUUID(old_uuid, uuid_);
        Logger::GetInstance().LogInfo("Session: Updated UUID to {}.",
                                      boost::uuids::to_string(uuid_));
    } else {
        Logger::GetInstance().LogInfo(
            "Session: Received invalid UUID, treating as message.");
        DoWrite(message_sv);
    }
    Logger::GetInstance().LogInfo("Session: Handled message.");

    DoRead();  // Continue reading from the client socket.
}

bool Session::IsUUIDValid(const std::string_view uuid_sv) {
    try {
        boost::uuids::string_generator()(std::string(uuid_sv));
        return true;
    } catch (const std::exception &exception) {
        return false;
    }
}

}  // namespace th_valley
