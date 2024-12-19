#ifndef NETWORK_CLIENT_H_
#define NETWORK_CLIENT_H_

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <functional>
#include <string_view>
#include <thread>

namespace th_valley {

class NetworkClient {
public:
    NetworkClient(NetworkClient const& other) = delete;
    NetworkClient& operator=(NetworkClient const& other) = delete;
    NetworkClient(NetworkClient&& other) = delete;
    NetworkClient& operator=(NetworkClient&& other) = delete;

    void Connect(std::string_view host, std::string_view port);
    void Disconnect();
    void ReceiveMessages();
    void SendMessages(std::string_view message_sv);
    void SetCallback(std::function<void(std::string_view)> callback);
    void SetUUID(boost::uuids::uuid uuid);
    boost::uuids::uuid GetUUID() const;

    static NetworkClient& GetInstance();

private:
    NetworkClient();
    ~NetworkClient();

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::resolver::results_type endpoints_;
    boost::asio::streambuf buffer_;
    boost::uuids::uuid uuid_;

    std::function<void(std::string_view)> callback_;
    std::thread io_context_thread_;
};

}  // namespace th_valley

#endif  // NETWORK_CLIENT_H_
