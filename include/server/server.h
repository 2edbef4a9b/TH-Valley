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

    void StartUp();
    void ShutDown();

private:
    void DoAccept();

    boost::asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<SessionManager> session_manager_;
};

}  // namespace th_valley

#endif  // SERVER_H_
