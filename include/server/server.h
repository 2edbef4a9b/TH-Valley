#ifndef SERVER_H_
#define SERVER_H_

#include <boost/asio.hpp>
#include <memory>
#include <string_view>

#include "server/session_manager.h"

namespace th_valley {

class Server {
public:
    Server(boost::asio::io_context& io_context, std::string_view port);
    ~Server();

    Server(const Server& other) = delete;
    Server& operator=(const Server& other) = delete;
    Server(Server&& other) = delete;
    Server& operator=(Server&& other) = delete;

    void StartUp();
    void ShutDown();

private:
    void DoAccept() const;

    boost::asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<SessionManager> session_manager_;
    bool is_running_{false};
};

}  // namespace th_valley

#endif  // SERVER_H_
