#include "server/server.h"

#include <boost/asio.hpp>

#include "utility/logger.h"

namespace th_valley {

Server::Server(boost::asio::io_context& io_context, const std::string_view port)
    : acceptor_(io_context,
                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                               std::stoi(std::string(port)))),
      session_manager_(std::make_shared<SessionManager>(acceptor_)) {}

void Server::StartUp() const {
    Logger::GetInstance().LogInfo("Server starting up.");
    DoAccept();
    Logger::GetInstance().LogInfo("Server started up.");
}

void Server::ShutDown() {
    Logger::GetInstance().LogInfo("Server shutting down.");
    acceptor_.close();
    Logger::GetInstance().LogInfo("Server shut down.");
}

void Server::DoAccept() const { session_manager_->StartAccept(); }

}  // namespace th_valley
