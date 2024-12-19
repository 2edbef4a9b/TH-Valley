#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <unordered_map>

#include "server/session.h"

namespace th_valley {

class Session;

class SessionManager : public std::enable_shared_from_this<SessionManager> {
public:
    explicit SessionManager(boost::asio::ip::tcp::acceptor& acceptor);

    void AddSession(const boost::uuids::uuid& uuid,
                    const std::shared_ptr<Session>& session);
    void RemoveSession(const boost::uuids::uuid& uuid);
    void StartAccept();
    void UpdateSessionUUID(const boost::uuids::uuid& old_uuid,
                           const boost::uuids::uuid& new_uuid);

private:
    boost::asio::ip::tcp::acceptor acceptor_;

    std::unordered_map<boost::uuids::uuid, std::shared_ptr<Session>> sessions_;
};

}  // namespace th_valley

#endif  // SESSION_MANAGER_H_
