#include "server/server.h"

#include <boost/asio.hpp>

#include "utility/logger.h"

namespace th_valley {

Server::Server(boost::asio::io_context& io_context, const std::string_view port)
    : acceptor_(io_context,
                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                               std::stoi(std::string(port)))),
      session_manager_(std::make_shared<SessionManager>(acceptor_)) {}

Server::~Server() {
    if (is_running_) {
        ShutDown();
    }
}

void Server::BroadcastMessage(const std::string_view message) const {
    session_manager_->BroadcastMessage(message);
}

void Server::SendMessage(const boost::uuids::uuid session_id,
                         const std::string_view message) const {
    session_manager_->SendMessage(session_id, message);
}

void Server::StartUp() {
    Logger::GetInstance().LogInfo("{}: Starting up.", server_name_);
    is_running_ = true;
    DoAccept();
    main_loop_thread_ = std::thread([this]() -> void { RunMainLoop(); });
    Logger::GetInstance().LogInfo("{}: Started up.", server_name_);
}

void Server::ShutDown() {
    Logger::GetInstance().LogInfo("{}: Shutting down.", server_name_);
    is_running_.store(false, std::memory_order_release);
    acceptor_.close();
    if (main_loop_thread_.joinable()) {
        main_loop_thread_.join();
    }
    Logger::GetInstance().LogInfo("{}: Shut down.", server_name_);
}

bool Server::IsRunning() const {
    return is_running_.load(std::memory_order_acquire);
}

void Server::RunMainLoop() {
    Logger::GetInstance().LogInfo("{}: Main loop starting.", server_name_);
    while (IsRunning()) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(kServerTickInterval));
    }
    Logger::GetInstance().LogInfo("{}: Main loop ended.", server_name_);
}

std::string_view Server::GetServerName() const { return server_name_; }

void Server::SetServerName(const std::string_view server_name) {
    server_name_ = server_name;
}

void Server::DoAccept() const {
    Logger::GetInstance().LogInfo("{}: Starting to accept connections.",
                                  server_name_);
    session_manager_->StartAccept();
    Logger::GetInstance().LogInfo("{}: Accepting connections initiated.",
                                  server_name_);
}

}  // namespace th_valley
