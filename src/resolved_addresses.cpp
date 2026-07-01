#include "resolved_addresses.hpp"

resolved_addresses::resolved_addresses(addrinfo* addresses):addresses_(addresses){}

resolved_addresses::resolved_addresses(resolved_addresses&& other) noexcept : addresses_(other.addresses_)
{
    other.addresses_ = nullptr;
}

resolved_addresses& resolved_addresses::operator=(resolved_addresses&& other) noexcept
{
    if (this != &other)
    {
        if (addresses_)
            freeaddrinfo(addresses_);

        addresses_ = other.addresses_;
        other.addresses_ = nullptr;
    }

    return *this;
}

resolved_addresses::~resolved_addresses(){
    if(addresses_)
        freeaddrinfo(addresses_);
}

addrinfo* resolved_addresses::get() {
    return addresses_;
}

std::string resolved_addresses::IP(const sockaddr* addresses_){

    char buffer[INET6_ADDRSTRLEN];
    if(addresses_->sa_family == AF_INET){
        auto* ipv4 = reinterpret_cast<const sockaddr_in*>(addresses_);
        inet_ntop(AF_INET,&ipv4->sin_addr,buffer,sizeof(buffer));
    }else{
        auto* ipv6 = reinterpret_cast<const sockaddr_in6*>(addresses_);
        inet_ntop(AF_INET6,&ipv6->sin6_addr,buffer,sizeof(buffer));
    }
    return buffer;
}

uint16_t resolved_addresses::Port(const sockaddr* addr){
    uint16_t port{};
    if(addr->sa_family == AF_INET){
        auto* ipv4 = reinterpret_cast<const sockaddr_in*>(addr);
        port = ntohs(ipv4->sin_port);
    }else{
        auto* ipv6 = reinterpret_cast<const sockaddr_in6*>(addr);
        port = ntohs(ipv6->sin6_port);
    }
    return port;
}
 

std::string resolved_addresses::IP(){
    return resolved_addresses::IP(addresses_->ai_addr);
}


std::vector<std::string> resolved_addresses::IPs(){
    std::vector<std::string> result;
    for(addrinfo* p = addresses_; p != nullptr; p = p->ai_next){
        result.push_back(IP(p->ai_addr));
    }
    return result;
}

