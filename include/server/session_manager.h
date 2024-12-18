#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "server/session.h"

namespace th_valley {

class SessionManager {
public:
    SessionManager(boost::asio::io_context& io_context, std::string_view port);

    void StartAccept();
    void HandleAccept(const std::shared_ptr<Session>& session,
                      const boost::system::error_code& error_code);

    void AddSession(const boost::uuids::uuid& uuid,
                    const std::shared_ptr<Session>& session);
    void RemoveSession(const boost::uuids::uuid& uuid);

private:
    boost::asio::ip::tcp::acceptor acceptor_;

    std::unordered_map<boost::uuids::uuid, std::shared_ptr<Session>> sessions_;
};

}  // namespace th_valley

#endif  // SESSION_MANAGER_H_
