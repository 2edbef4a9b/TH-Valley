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
    std::lock_guard<std::mutex> lock(mutex_);
    if (is_running_) {
        ShutDown();
    }
}

void Server::StartUp() {
    std::lock_guard<std::mutex> lock(mutex_);
    Logger::GetInstance().LogInfo("Server starting up.");
    is_running_ = true;
    main_loop_thread_ = std::thread([this]() -> void { RunMainLoop(); });
    DoAccept();
    Logger::GetInstance().LogInfo("Server started up.");
}

void Server::ShutDown() {
    std::lock_guard<std::mutex> lock(mutex_);
    Logger::GetInstance().LogInfo("Server shutting down.");
    acceptor_.close();
    if (main_loop_thread_.joinable()) {
        main_loop_thread_.join();
    }
    is_running_ = false;
    Logger::GetInstance().LogInfo("Server shut down.");
}

bool Server::IsRunning() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return is_running_;
}

void Server::RunMainLoop() {
    Logger::GetInstance().LogInfo("Server running main loop.");
    while (is_running_) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(kServerTickInterval));
    }
    Logger::GetInstance().LogInfo("Server main loop ended.");
}

void Server::DoAccept() const {
    std::lock_guard<std::mutex> lock(mutex_);
    Logger::GetInstance().LogInfo("Server accepting connections.");
    session_manager_->StartAccept();
    Logger::GetInstance().LogInfo("Server accepted connections.");
}

}  // namespace th_valley
