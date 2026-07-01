#include "epoll_handler.hpp"
#include <iostream>
#include <unistd.h>


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

int epoll_handler::add (int fd,struct epoll_event* events){
    epoll_event event{};
    int flag =epoll_ctl(epfd_,)

}

