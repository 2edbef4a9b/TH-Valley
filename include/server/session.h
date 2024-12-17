#ifndef SESSION_H_
#define SESSION_H_

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <string_view>

namespace th_valley {

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket socket, boost::uuids::uuid uuid);

    void Start();
    void Terminate();
    void Read();
    void Write(std::string_view message);

private:
    boost::asio::ip::tcp::socket socket_;
    boost::uuids::uuid uuid_;
    boost::asio::streambuf buffer_;
};

}  // namespace th_valley

#endif  // SESSION_H_
