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

    boost::uuids::uuid GetUUID() const;
    void Start();
    void Terminate();

private:
    void DoRead();
    void DoWrite(std::string_view message_sv);
    boost::asio::ip::tcp::socket socket_;
    boost::uuids::uuid uuid_;
    boost::asio::streambuf buffer_;
};

}  // namespace th_valley

#endif  // SESSION_H_
