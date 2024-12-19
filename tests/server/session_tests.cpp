#include <gtest/gtest.h>

#include <boost/uuid/random_generator.hpp>
#include <chrono>
#include <thread>

#include "client/network_client.h"
#include "server/server.h"

namespace th_valley {

class ServerClientTest : public ::testing::Test {
protected:
    ServerClientTest() : server_(server_io_context_, "28080") {}

    void SetUp() override {
        server_thread_ = std::thread([this]() {
            server_.StartUp();
            server_io_context_.run();
        });

        // Give the server some time to start
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void TearDown() override {
        // Shut down the server
        server_.ShutDown();
        server_io_context_.stop();
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }

    boost::asio::io_context server_io_context_;
    Server server_;
    std::thread server_thread_;
};

TEST_F(ServerClientTest, ClientServerCommunication) {
    NetworkClient& client = NetworkClient::GetInstance();
    client.SetUUID(boost::uuids::random_generator()());
    client.Connect("127.0.0.1", "28080");

    // Set a callback to handle received messages
    client.SetCallback([](std::string_view message) {
        std::cout << "Received message: " << message << std::endl;
    });

    // Send a message to the server
    client.SendMessages("Hello, Server!");

    client.Disconnect();
}

}  // namespace th_valley

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
