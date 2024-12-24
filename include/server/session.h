#ifndef SESSION_H_
#define SESSION_H_

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <string>
#include <string_view>

#include "server/session_manager.h"

namespace th_valley {

class SessionManager;

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(boost::asio::ip::tcp::socket socket);

    void DoRead();
    void DoWrite(std::string_view message_sv);
    boost::asio::ip::tcp::socket &GetSocket();
    boost::uuids::uuid GetUUID() const;
    void SetSessionManager(
        const std::shared_ptr<SessionManager> &session_manager);
    void Start();
    void Terminate();

private:
    void HandleMessage(std::string_view message_sv);
    static bool IsUUIDValid(std::string_view uuid_sv);

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buffer_;
    boost::uuids::uuid uuid_;

    std::string received_message_;
    std::string sent_message_;
    std::weak_ptr<SessionManager> session_manager_;
};

}  // namespace th_valley

#endif  // SESSION_H_
