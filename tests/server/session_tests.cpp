#include <client/client_application.h>
#include <gtest/gtest.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <thread>

#include "client/network_client.h"
#include "server/server.h"
#include "utility/logger.h"

namespace th_valley {

class ServerClientTest : public ::testing::Test {
protected:
    ServerClientTest() : server_(server_io_context_, "65472") {}

    void SetUp() override {
        // Start the server in server_thread_.
        server_thread_ = std::thread([this]() {
            server_.StartUp();
            server_io_context_.run();
        });
    }

    void TearDown() override {
        // Stop the server and join the server_thread_.
        server_.ShutDown();
        server_io_context_.stop();
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }

private:
    boost::asio::io_context server_io_context_;
    Server server_;
    std::thread server_thread_;
};

TEST_F(ServerClientTest, ClientServerCommunication) {
    NetworkClient& client = NetworkClient::GetInstance();

    // Set the UUID for the client.
    client.SetUUID(boost::uuids::random_generator()());

    // Set a callback to handle received messages.
    client.SetCallback([](std::string_view message) -> void {
        Logger::GetInstance().LogInfo("Client Callback: Received: {}.",
                                      message);
    });

    // Connect the client to the server.
    client.Connect("127.0.0.1", "65472");

    // Wait for the client to connect.
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Send the UUID to the server first
    client.SendMessages(boost::uuids::to_string(client.GetUUID()));
    client.SendMessages("Hello, server!");
    client.SendMessages("Goodbye, server!");

    // Wait for the server to process the messages.
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Disconnect the client from the server.
    client.Disconnect();
}

}  // namespace th_valley

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
