#include "address_resolver.hpp"
#include "socket.hpp"

#include <atomic>
#include <iostream>
#include <string>
#include <system_error>
#include <thread>

std::atomic<bool> running{true};

void receive_messages(socket_handler& sock) {
    char buffer[1024];

    while (running) {
        try {
            std::size_t bytes = sock.recv_data(buffer, sizeof(buffer) - 1);

            if (bytes == 0) {
                std::cout << "\n[Server disconnected]" << std::endl;
                running = false;
                break;
            }

            buffer[bytes] = '\0';
            std::cout << buffer << std::flush;
        } catch (const std::system_error& e) {
            if (running) {
                std::cerr << "\n[Connection error: " << e.what() << "]" << std::endl;
                running = false;
            }
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    const std::string host = (argc > 1) ? argv[1] : "127.0.0.1";
    const std::string port = (argc > 2) ? argv[2] : "3000";

    try {
        std::cout << "Connecting to " << host << ":" << port << "..." << std::endl;

        auto address = address_resolver::resolve(host, port);

        socket_handler client_socket = socket_handler::create_socket(address.get());
        client_socket.connect_socket(*address.get());

        std::cout << "Connected! Enter your name: ";
        std::string name;
        std::getline(std::cin, name);

        std::cout << "Type your messages below:" << std::endl;

        std::thread receiver(receive_messages, std::ref(client_socket));

        std::string input;
        while (running && std::getline(std::cin, input)){
            if (input == "/quit") {
                std::string message = name + " left the chat\n";
                 client_socket.send_data(message);
                running = false;
                break;
            }

            if (input.empty() || input.find_first_not_of(" \t\r\n") == std::string::npos) {
                continue;
            }

            std::string message = name + ": " + input + "\n";
            client_socket.send_data(message);
        }

        running = false;
        shutdown(client_socket.get_fd(), SHUT_RDWR);

        if (receiver.joinable()) {
            receiver.join();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}