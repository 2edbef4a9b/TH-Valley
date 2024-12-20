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
    virtual ~Server();

    Server(const Server& other) = delete;
    Server& operator=(const Server& other) = delete;
    Server(Server&& other) = delete;
    Server& operator=(Server&& other) = delete;

    void StartUp();
    void ShutDown();

protected:
    bool IsRunning() const;
    virtual void RunMainLoop();
    static constexpr double kServerTickRate = 60.0;
    static constexpr std::size_t kServerTickInterval =
        static_cast<std::size_t>(1000.0 / kServerTickRate);

private:
    void DoAccept() const;
    boost::asio::ip::tcp::acceptor acceptor_;
    mutable std::mutex mutex_;
    std::thread main_loop_thread_;
    std::shared_ptr<SessionManager> session_manager_;
    std::atomic<bool> is_running_{false};
};

}  // namespace th_valley

#endif  // SERVER_H_
