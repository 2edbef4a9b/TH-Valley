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

void Server::StartUp() {
    Logger::GetInstance().LogInfo("Server starting up.");
    is_running_ = true;
    DoAccept();
    main_loop_thread_ = std::thread([this]() -> void { RunMainLoop(); });
    Logger::GetInstance().LogInfo("Server started up.");
}

void Server::ShutDown() {
    Logger::GetInstance().LogInfo("Server shutting down.");
    is_running_.store(false, std::memory_order_release);
    acceptor_.close();
    if (main_loop_thread_.joinable()) {
        main_loop_thread_.join();
    }
    Logger::GetInstance().LogInfo("Server shut down.");
}

bool Server::IsRunning() const {
    return is_running_.load(std::memory_order_acquire);
}

void Server::RunMainLoop() {
    Logger::GetInstance().LogInfo("Server running main loop.");
    while (IsRunning()) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(kServerTickInterval));
    }
    Logger::GetInstance().LogInfo("Server main loop ended.");
}

void Server::DoAccept() const {
    Logger::GetInstance().LogInfo("Server accepting connections.");
    session_manager_->StartAccept();
    Logger::GetInstance().LogInfo("Server accepted connections.");
}

}  // namespace th_valley
