#include "server/session_manager.h"

#include <boost/uuid/uuid_io.hpp>
#include <memory>

#include "server/session.h"
#include "utility/logger.h"

namespace th_valley {

SessionManager::SessionManager(boost::asio::ip::tcp::acceptor& acceptor)
    : acceptor_(std::move(acceptor)) {}

void SessionManager::AddSession(const boost::uuids::uuid& uuid,
                                const std::shared_ptr<Session>& session) {
    sessions_.emplace(uuid, session);
}

void SessionManager::RemoveSession(const boost::uuids::uuid& uuid) {
    sessions_.erase(uuid);
}

void SessionManager::StartAccept() {
    // Start accepting new connections asynchronously.
    acceptor_.async_accept([this](const boost::system::error_code& error_code,
                                  boost::asio::ip::tcp::socket socket) {
        if (!error_code) {
            // Create a new session for the accepted connection.
            const auto session = std::make_shared<Session>(std::move(socket));

            // Bind the session manager to the session and start the session.
            session->SetSessionManager(shared_from_this());
            session->Start();

            const boost::uuids::uuid uuid = session->GetUUID();
            std::string remote_endpoint_str =
                session->GetSocket().remote_endpoint().address().to_string();
            auto remote_port = session->GetSocket().remote_endpoint().port();

            Logger::GetInstance().LogInfo(
                "Session Manager: Accepted client connection from {}:{}",
                remote_endpoint_str, remote_port);

            // Add the session to the session map.
            AddSession(uuid, session);
        } else {
            Logger::GetInstance().LogError(
                "Error occurred while accepting connection: " +
                error_code.message());
        }

        // Continue accepting new connections regardless of the error.
        StartAccept();
    });
}

void SessionManager::UpdateSessionUUID(const boost::uuids::uuid& old_uuid,
                                       const boost::uuids::uuid& new_uuid) {
    // Update the UUID of an existing session.
    if (sessions_.contains(old_uuid)) {
        const auto session = sessions_.at(old_uuid);
        RemoveSession(old_uuid);
        AddSession(new_uuid, session);
        Logger::GetInstance().LogInfo(
            "Session Manager: Updated UUID from {} to {}", to_string(old_uuid),
            to_string(new_uuid));
    } else {
        Logger::GetInstance().LogError(
            "Session Manager: Attempted to update UUID for non-existent "
            "session with UUID: {}",
            to_string(old_uuid));
    }
}

}  // namespace th_valley
