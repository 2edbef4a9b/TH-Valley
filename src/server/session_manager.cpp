#include "server/session_manager.h"

#include <boost/uuid/uuid_io.hpp>
#include <memory>

#include "server/session.h"
#include "utility/logger.h"

namespace th_valley {

SessionManager::SessionManager(boost::asio::ip::tcp::acceptor& acceptor)
    : acceptor_(std::move(acceptor)) {}

void SessionManager::StartAccept() {
    acceptor_.async_accept([this](const boost::system::error_code& error_code,
                                  boost::asio::ip::tcp::socket socket) {
        if (!error_code) {
            auto session = std::make_shared<Session>(std::move(socket));
            session->Start();

            boost::uuids::uuid const uuid = session->GetUUID();
            std::string uuid_str = boost::uuids::to_string(uuid);
            std::string remote_endpoint_str =
                session->GetSocket().remote_endpoint().address().to_string();

            Logger::GetInstance().LogInfo(
                "Client connected with UUID: " + uuid_str +
                " and IP: " + remote_endpoint_str);
            AddSession(uuid, session);
        } else {
            Logger::GetInstance().LogError(
                "Error occurred while accepting connection: " +
                error_code.message());
        }

        // Continue accepting new connections
        StartAccept();
    });
}

void SessionManager::AddSession(const boost::uuids::uuid& uuid,
                                const std::shared_ptr<Session>& session) {
    sessions_.emplace(uuid, session);
}

void SessionManager::RemoveSession(const boost::uuids::uuid& uuid) {
    sessions_.erase(uuid);
}

}  // namespace th_valley
