#pragma once
#include <sys/epoll.h>

class epoll_handler
{

public:
    epoll_handler();
    epoll_handler(const epoll_handler &) = delete;
    epoll_handler &operator=(const epoll_handler &) = delete;
    epoll_handler(epoll_handler &&other) noexcept;
    epoll_handler &operator=(epoll_handler &&other) noexcept;

     ~epoll_handler();

    int add (int fd,struct epoll_event* events);
    int modify(int fd, struct epoll_event* events);
    int remove (int fd,struct epoll_event* events);

    int wait (int epfd, struct epoll_event* events);


private:
    int epfd_{-1};
    static constexpr int MAX_EVENTS = 64;
};
