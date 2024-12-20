#include <client/client_application.h>
#include <gtest/gtest.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <thread>

#include "client/network_client.h"
#include "server/server.h"
#include "server/universe_server.h"
#include "utility/logger.h"

namespace th_valley {

class ServerClientTest : public ::testing::Test {
protected:
    ServerClientTest()
        : server_(server_io_context_, "65472"),
          universe_server_(universe_io_context_, "65473") {}

    void SetUp() override {
        // No server is started here. Each test will start the required server.
    }

    void TearDown() override {
        // Stop the server and join the server_thread_ if it was started.
        server_io_context_.stop();
        if (server_thread_.joinable()) {
            server_thread_.join();
        }

        // Stop the universe server and join the universe_server_thread_ if it
        // was started.
        universe_io_context_.stop();
        if (universe_server_thread_.joinable()) {
            universe_server_thread_.join();
        }
    }

    void StartServer() {
        server_thread_ = std::thread([this]() {
            server_.StartUp();
            server_io_context_.run();
        });
    }

    void StartUniverseServer() {
        universe_server_thread_ = std::thread([this]() {
            universe_server_.StartUp();
            universe_io_context_.run();
        });
    }

private:
    boost::asio::io_context server_io_context_;
    Server server_;
    boost::asio::io_context universe_io_context_;
    UniverseServer universe_server_;
    std::thread server_thread_;
    std::thread universe_server_thread_;
};

TEST_F(ServerClientTest, ClientServerCommunication) {
    StartServer();

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

TEST_F(ServerClientTest, ClientUniverseServerCommunication) {
    StartUniverseServer();

    NetworkClient& client = NetworkClient::GetInstance();

    // Set the UUID for the client.
    client.SetUUID(boost::uuids::random_generator()());

    // Set a callback to handle received messages.
    client.SetCallback([](std::string_view message) -> void {
        Logger::GetInstance().LogInfo("Client Callback: Received: {}.",
                                      message);
    });

    // Connect the client to the universe server.
    client.Connect("127.0.0.1", "65473");

    // Wait for the client to connect.
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Send the UUID to the universe server first
    client.SendMessages(boost::uuids::to_string(client.GetUUID()));
    client.SendMessages("Hello, universe server!");
    client.SendMessages("Goodbye, universe server!");

    // Wait for the universe server to process the messages.
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Disconnect the client from the universe server.
    client.Disconnect();
}

}  // namespace th_valley

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
