#ifndef NETWORK_CLIENT_H_
#define NETWORK_CLIENT_H_

#include <sys/stat.h>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>

namespace th_valley {

class NetworkClient {
public:
    NetworkClient(NetworkClient const& other) = delete;
    NetworkClient& operator=(NetworkClient const& other) = delete;
    NetworkClient(NetworkClient&& other) = delete;
    NetworkClient& operator=(NetworkClient&& other) = delete;

    void Connect(std::string_view host, std::string_view port);
    void Disconnect();
    void SendMessages(std::string_view message_sv);
    void SetUUID(boost::uuids::uuid uuid);

    static NetworkClient& GetInstance();

private:
    NetworkClient();
    ~NetworkClient() = default;

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::resolver::results_type endpoints_;
    boost::uuids::uuid uuid_;
};

}  // namespace th_valley

#endif  // NETWORK_CLIENT_H_
