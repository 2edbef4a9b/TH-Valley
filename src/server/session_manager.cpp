#include "server/session_manager.h"

#include <boost/uuid/uuid_io.hpp>

#include "server/session.h"
#include "utility/logger.h"

namespace th_valley {

SessionManager::SessionManager(boost::asio::io_context& io_context,
                               const std::string_view port)
    : acceptor_(io_context,
                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                               std::stoi(std::string(port)))) {
    StartAccept();
}

void SessionManager::StartAccept() {
    acceptor_.async_accept([this](const boost::system::error_code& error_code,
                                  boost::asio::ip::tcp::socket socket) {
        std::make_shared<Session>(std::move(socket))->Start();
        HandleAccept(std::make_shared<Session>(std::move(socket)), error_code);
    });
}

void SessionManager::HandleAccept(const std::shared_ptr<Session>& session,
                                  const boost::system::error_code& error_code) {
    if (!error_code) {
        boost::uuids::uuid const uuid = session->GetUUID();
        std::string uuid_str = boost::uuids::to_string(uuid);
        std::string remote_endpoint_str =
            session->GetSocket().remote_endpoint().address().to_string();

        Logger::GetInstance().LogInfo(
            "Client connected with UUID: {} and IP: {}", uuid_str,
            remote_endpoint_str);
        AddSession(uuid, session);
    } else {
        Logger::GetInstance().LogError(
            "Error occurred while accepting connection: {}",
            error_code.message());
    }
    StartAccept();
}

void SessionManager::AddSession(const boost::uuids::uuid& uuid,
                                const std::shared_ptr<Session>& session) {
    sessions_.emplace(uuid, session);
}

void SessionManager::RemoveSession(const boost::uuids::uuid& uuid) {
    sessions_.erase(uuid);
}

}  // namespace th_valley
