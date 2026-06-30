#pragma once
#include <sys/epoll.h>
#include <connection.hpp>

class epoll_handler
{

public:
    epoll_handler();
    epoll_handler(const epoll_handler &) = delete;
    epoll_handler &operator=(const epoll_handler &) = delete;
    epoll_handler(epoll_handler &&other) noexcept;
    epoll_handler &operator=(epoll_handler &&other) noexcept;

    ~epoll_handler();

    void add (uint32_t events,connection* conn);
    void modify(uint32_t events,connection* conn);
    void remove (uint32_t events,connection* conn);

    int wait (uint32_t events,connection* conn);


private:
    int epfd_{-1};
    static constexpr int MAX_EVENTS = 64;
};
