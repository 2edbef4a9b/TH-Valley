#include "server/session.h"

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "utility/logger.h"

namespace th_valley {

boost::uuids::uuid Session::GetUUID() const { return uuid_; }

boost::asio::ip::tcp::socket &Session::GetSocket() { return socket_; }

Session::Session(boost::asio::ip::tcp::socket socket)
    : socket_(std::move(socket)) {}

void Session::Start() {
    Logger::GetInstance().LogInfo("Session started with UUID: " +
                                  boost::uuids::to_string(uuid_));
    DoRead();  // Start reading from the client socket.
}

void Session::Terminate() {
    Logger::GetInstance().LogInfo("Session terminated with UUID: " +
                                  boost::uuids::to_string(uuid_));
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
}

void Session::DoRead() {
    // Start an asynchronous read operation.
    boost::asio::async_read_until(
        socket_, buffer_, '\n',
        [self = shared_from_this()](const boost::system::error_code &error_code,
                                    std::size_t /*length*/) -> void {
            if (!error_code) {
                // Successfully read data, process it.
                std::istream input_stream(&self->buffer_);
                std::string line;
                std::getline(input_stream, line);
                Logger::GetInstance().LogInfo("Received: " + line);

                // After processing, continue reading.
                self->DoRead();
            } else {
                Logger::GetInstance().LogError("Error reading from socket: " +
                                               error_code.message());
                self->Terminate();
            }
        });
}

void Session::DoWrite(const std::string_view message_sv) {
    // Convert the message to a string and append a newline for the protocol.
    std::string message(message_sv);
    message += '\n';

    // Start an asynchronous write operation.
    boost::asio::async_write(
        socket_, boost::asio::buffer(message),
        [self = shared_from_this()](const boost::system::error_code &error_code,
                                    const std::size_t length) {
            if (!error_code) {
                Logger::GetInstance().LogInfo("Sent " + std::to_string(length) +
                                              " bytes to client.");
            } else {
                Logger::GetInstance().LogError("Error writing to socket: " +
                                               error_code.message());
                self->Terminate();
            }
        });
}

void Session::HandleInitMessage(const std::string_view message_sv) {
    // Process the message and send a response.
    try {
        uuid_ = boost::uuids::string_generator()(std::string(message_sv));
        Logger::GetInstance().LogInfo("Received UUID: " +
                                      boost::uuids::to_string(uuid_));
        DoRead();  // Start reading from the client socket.
        DoWrite("UUID received.");
    } catch (const std::exception &exception) {
        Logger::GetInstance().LogError("Error parsing UUID: " +
                                       std::string(exception.what()));
        Terminate();
    }
}

}  // namespace th_valley
