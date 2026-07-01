#include "address_resolver.hpp"
#include "socket.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <system_error>
#include <atomic>

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

int main() {
    try {
        std::cout << "Connecting to 127.0.0.1:3000..." << std::endl;
        auto address = address_resolver::resolve("127.0.0.1", "3000");
        
        socket_handler client_socket = socket_handler::create_socket(address.get());
        client_socket.connect_socket(*address.get());
        
        std::cout << "Connected! Enter your name: ";
        std::string name;
        std::getline(std::cin, name);
        
        std::cout << "Type your messages below:" << std::endl;
        
        std::thread receiver(receive_messages, std::ref(client_socket));
        
        std::string input;
        while (running && std::getline(std::cin, input)) {
            if (input == "/quit") {
                running = false;
                break;
            }
            std::string message = name + ": " + input + "\n";
            client_socket.send_data(message);
        }
        
        running = false;
        
        // Wait for receiver thread to finish
        if (receiver.joinable()) {
            receiver.join();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
