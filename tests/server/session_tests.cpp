#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/asio.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "server/session.h"

class MockSocket : public boost::asio::ip::tcp::socket {
public:
    explicit MockSocket(boost::asio::io_context& io_context)
        : boost::asio::ip::tcp::socket(io_context) {}

    MOCK_METHOD4(async_read_until,
                 void(boost::asio::ip::tcp::socket&, boost::asio::streambuf&,
                      char,
                      std::function<void(const boost::system::error_code&,
                                         std::size_t)>));
    MOCK_METHOD2(async_write_some,
                 void(const boost::asio::const_buffer&,
                      std::function<void(const boost::system::error_code&,
                                         std::size_t)>));
    MOCK_METHOD2(shutdown, void(boost::asio::ip::tcp::socket::shutdown_type,
                                boost::system::error_code&));
    MOCK_METHOD1(close, void(boost::system::error_code&));
};

class SessionTest : public testing::Test {
protected:
    boost::asio::io_context io_context_;
    boost::uuids::uuid uuid_;
    std::shared_ptr<MockSocket> mock_socket_;
    std::shared_ptr<th_valley::Session> session_;

    void SetUp() override {
        boost::uuids::random_generator generator;
        uuid_ = generator();
        mock_socket_ = std::make_shared<MockSocket>(io_context_);
        session_ = std::make_shared<th_valley::Session>(
            std::move(*mock_socket_), uuid_);
    }
};

TEST_F(SessionTest, StartSession) {
    EXPECT_CALL(*mock_socket_, async_read_until(testing::_, testing::_,
                                                testing::_, testing::_))
        .Times(testing::AtLeast(1));
    session_->Start();
}

TEST_F(SessionTest, TerminateSession) {
    constexpr boost::system::error_code kErrorCode;
    EXPECT_CALL(
        *mock_socket_,
        shutdown(boost::asio::ip::tcp::socket::shutdown_both, testing::_))
        .WillOnce(testing::SetArgReferee<1>(kErrorCode));
    EXPECT_CALL(*mock_socket_, close(testing::_))
        .WillOnce(testing::SetArgReferee<0>(kErrorCode));
    session_->Terminate();
}

TEST_F(SessionTest, WriteMessage) {
    const std::string message = "Hello, World!\n";
    session_->Write(message);
}
