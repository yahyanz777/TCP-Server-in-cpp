#include "address_resolver.hpp"
#include "resolved_addresses.hpp"
#include "socket.hpp"
#include "epoll_handler.hpp"
#include <system_error>
#include <iostream>
#include <algorithm>
#include <vector>

int main(){
    auto address = address_resolver::resolve("127.0.0.1","3000");
    socket_handler listener_socket = socket_handler::create_socket(address.get());
    listener_socket.bind_socket(*address.get());
    listener_socket.socket_listen(15);
    epoll_handler* epoll = new epoll_handler();
    connection* conn= new connection(epoll->get_epfd(),listener_socket.get_fd());
    uint32_t events= EPOLLIN | EPOLLET;
    epoll->add(events,conn);
    std::vector<epoll_event>active_events{};
    std::vector<connection*> clients{};

    while(true){
        int active = epoll->wait(active_events);
        for(int i=0; i<active; i++){
            connection* conn=static_cast<connection*>(active_events[i].data.ptr);
            if (conn->get_cfd() == listener_socket.get_fd()){
                auto conn_socket =listener_socket.socket_accept();
                conn_socket.set_nonblocking();
                events=EPOLLIN | EPOLLET ;
                auto* con= new connection(epoll->get_epfd(),std::move(conn_socket));
                epoll->add(events,con);
                clients.push_back(con);
                std::cout << "[Server] New client connected. Total clients: " << clients.size() << std::endl;
            }else{
                char buffer[1024];
                while(true){
                    try {
                        std::size_t count = conn->get_socket().recv_data(buffer, sizeof(buffer));
                        if (count == 0) {
                            // Client closed connection
                            epoll->remove(conn);
                            clients.erase(std::remove(clients.begin(), clients.end(), conn), clients.end());
                            delete conn;
                            std::cout << "[Server] Client disconnected. Total clients: " << clients.size() << std::endl;
                            break;
                        }
                        
                        // Broadcast data to all clients except sender
                        for (auto* client : clients) {
                            if (client != conn) {
                                try {
                                    client->get_socket().send_data(buffer, count);
                                } catch (...) {
                                    // Ignore send errors to other clients for now
                                }
                            }
                        }
                    } catch (const std::system_error& e) {
                        if (epoll->handle_read_error(e, conn)) {
                            // If handle_read_error cleans up the connection (fatal error)
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
}

