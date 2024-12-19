#include <client/client_application.h>
#include <gtest/gtest.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <thread>

#include "client/network_client.h"
#include "server/server.h"

namespace th_valley {

class ServerClientTest : public ::testing::Test {
protected:
    ServerClientTest() : server_(server_io_context_, "65472") {}

    void SetUp() override {
        server_thread_ = std::thread([this]() {
            server_.StartUp();
            server_io_context_.run();
        });

        std::this_thread::sleep_for(std::chrono::seconds(1));

    }

    void TearDown() override {
        server_io_context_.stop();
        if (server_thread_.joinable()) {
            server_thread_.join();
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    boost::asio::io_context server_io_context_;
    Server server_;
    std::thread server_thread_;
};

TEST_F(ServerClientTest, ClientServerCommunication) {
    NetworkClient& client = NetworkClient::GetInstance();
    client.SetUUID(boost::uuids::random_generator()());
    // Set a callback to handle received messages
    client.SetCallback([](std::string_view message) {
        std::cout << "Client Callback: " << message << std::endl;
    });

    client.Connect("127.0.0.1", "65472");

    std::this_thread::sleep_for(std::chrono::seconds(3));


    std::cout << "Client connected" << std::endl;

    // Send the UUID to the server first
    client.SendMessages(boost::uuids::to_string(client.GetUUID()));
    client.SendMessages("Hello, server!");
    client.SendMessages("Goodbye, server!");
    client.Disconnect();
    server_.ShutDown();
}

}  // namespace th_valley

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
