#pragma once
#include <sys/epoll.h>
#include <connection.hpp>
#include <vector>

class epoll_handler
{

public:
    epoll_handler();
    epoll_handler(const epoll_handler &) = delete;
    epoll_handler &operator=(const epoll_handler &) = delete;
    epoll_handler(epoll_handler &&other) noexcept;
    epoll_handler &operator=(epoll_handler &&other) noexcept;

    ~epoll_handler();

    void add(uint32_t events, connection* conn);
    void modify(uint32_t events, connection* conn);
    void remove(connection* conn);

    int wait(std::vector<epoll_event>& active_events, int timeout_ms = -1);


private:
    int epfd_{-1};
    static constexpr int MAX_EVENTS = 64;
};
