#include "address_resolver.hpp"
#include "resolved_addresses.hpp"
#include "socket.hpp"
#include "epoll_handler.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

int main(int argc, char* argv[]) {

    const std::string host = (argc > 1) ? argv[1] : "0.0.0.0";
    const std::string port = (argc > 2) ? argv[2] : "3000";

    try {
        auto address = address_resolver::resolve("", port);

        socket_handler listener_socket = socket_handler::create_socket(address.get());
        listener_socket.bind_socket(*address.get());
        listener_socket.socket_listen(15);

        std::cout << "[Server] Listening on " << host << ":" << port << std::endl;

        epoll_handler epoll;
        auto* listener_conn = new connection(epoll.get_epfd(), listener_socket.get_fd());
        uint32_t events = EPOLLIN | EPOLLET;
        epoll.add(events, listener_conn);

        std::vector<epoll_event> active_events{};
        std::vector<connection*> clients{};

        while (true) {
            int active = epoll.wait(active_events);

            for (int i = 0; i < active; i++) {
                connection* conn = static_cast<connection*>(active_events[i].data.ptr);

                if (conn->get_cfd() == listener_socket.get_fd()) {
                    auto conn_socket = listener_socket.socket_accept();
                    conn_socket.set_nonblocking();

                    auto* client_conn = new connection(epoll.get_epfd(), std::move(conn_socket));
                    epoll.add(EPOLLIN | EPOLLET, client_conn);
                    clients.push_back(client_conn);

                    std::cout << "[Server] New client connected. Total clients: "
                              << clients.size() << std::endl;
                } else {
                    char buffer[1024];

                    while (true) {
                        try {
                            std::size_t count = conn->get_socket().recv_data(buffer, sizeof(buffer));

                            if (count == 0) {
                                epoll.remove(conn);
                                clients.erase(std::remove(clients.begin(), clients.end(), conn), clients.end());
                                delete conn;

                                std::cout << "[Server] Client disconnected. Total clients: "
                                          << clients.size() << std::endl;
                                break;
                            }

                            for (auto* client : clients) {
                                if (client != conn) {
                                    try {
                                        client->get_socket().send_data(buffer, count);
                                    } catch (...) {
                                        // Ignore send failures to other clients for now.
                                    }
                                }
                            }
                        } catch (const std::system_error& e) {
                            if (epoll.handle_read_error(e, conn)) {
                                if (e.code().value() != EAGAIN && e.code().value() != EWOULDBLOCK) {
                                    clients.erase(std::remove(clients.begin(), clients.end(), conn), clients.end());
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}