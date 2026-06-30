#include "epoll_handler.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <system_error>


namespace
{
std::runtime_error make_epoll_error(const char* operation)
{
    return std::system_error(errno,std::generic_category(),operation);
}
}

epoll_handler::epoll_handler(){
    epfd_ = epoll_create1(0);
    if(epfd_ ==-1)
    throw make_epoll_error("epoll_create1 failed");
}

epoll_handler::epoll_handler(epoll_handler &&other) noexcept : epfd_(other.epfd_){
    other.epfd_ =-1;
}

epoll_handler& epoll_handler::operator=( epoll_handler && other)noexcept{
    if( &other != this){
        if(epfd_ != -1 ){
            close(epfd_);
        }
        epfd_=other.epfd_;
        other.epfd_ =-1;
    }
    return *this;
}

epoll_handler::~epoll_handler() {
    if (epfd_ != -1) {
        close(epfd_);
    }
}

void epoll_handler::add(uint32_t events, connection* conn){
    epoll_event event{};
    event.events = events;
    event.data.ptr = conn;
    if(epoll_ctl(epfd_, EPOLL_CTL_ADD, conn->get_cfd(), &event) == -1){
        throw make_epoll_error("epoll_ctl : add failed");
    }
}

void epoll_handler::modify(uint32_t events, connection* conn){
    epoll_event event{};
    event.events = events;
    event.data.ptr = conn;
    if(epoll_ctl(epfd_, EPOLL_CTL_MOD, conn->get_cfd(), &event) == -1){
        throw make_epoll_error("epoll_ctl : mod failed");
    }
}

void epoll_handler::remove(connection* conn){
    if(epoll_ctl(epfd_, EPOLL_CTL_DEL, conn->get_cfd(), nullptr) == -1){
        throw make_epoll_error("epoll_ctl : del failed");
    }
}

int epoll_handler::wait(std::vector<epoll_event>& active_events, int timeout_ms){
    active_events.resize(MAX_EVENTS);
    int ready = epoll_wait(epfd_, active_events.data(), MAX_EVENTS, timeout_ms);
    if(ready == -1) {
        if (errno == EINTR) {
            active_events.clear();
            return 0;
        }
        throw make_epoll_error("epoll_wait failed");
    }
    active_events.resize(ready);
    return ready;
}




