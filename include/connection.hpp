#pragma once
#include "socket.hpp"
#include <utility>

class connection
{
public:
    connection() = default;
    
    // Constructor matching the user's calling order (epfd first, socket second)
    connection(int epfd, socket_handler&& socket) : epfd_(epfd), socket_(std::move(socket)) {}
    connection(int epfd, int cfd) : epfd_(epfd), socket_(cfd) {}

    // Rule of 5: Handle copying and moving safely
    connection(const connection &) = delete;
    connection &operator=(const connection &) = delete;

    connection(connection &&other) noexcept 
        : epfd_(other.epfd_), socket_(std::move(other.socket_))
    {
        other.epfd_ = -1;
    }

    connection &operator=(connection &&other) noexcept
    {
        if (this != &other)
        {
            epfd_ = other.epfd_;
            socket_ = std::move(other.socket_);
            other.epfd_ = -1;
        }
        return *this;
    }

    ~connection() = default; // socket_ destructor automatically closes the file descriptor

    int get_epfd() const
    {
        return epfd_;
    }

    int get_cfd()
    {
        return socket_.get_fd();
    }

    socket_handler& get_socket()
    {
        return socket_;
    }

private:
    int epfd_{-1};
    socket_handler socket_{-1};
};