#include "resolved_addresses.hpp"

resolved_addresses::resolved_addresses(addrinfo* addresses):addresses_(addresses){}

resolved_addresses::~resolved_addresses(){
    if(addresses_)
        freeaddrinfo(addresses_);
}

addrinfo* resolved_addresses::get() const{
    return addresses_;
}

std::string resolved_addresses::IP(addrinfo* addresses_){

    char buffer[INET6_ADDRSTRLEN];
    if(addresses_->ai_family == AF_INET){
        auto* ipv4 = reinterpret_cast<sockaddr_in*>(addresses_->ai_addr);
        inet_ntop(AF_INET,&ipv4->sin_addr,buffer,sizeof(buffer));
    }else{
        sockaddr_in6* ipv6 = reinterpret_cast<sockaddr_in6*>(addresses_->ai_addr);
        inet_ntop(AF_INET6,&ipv6->sin6_addr,buffer,sizeof(buffer));
    }
    return buffer;
}

std::string resolved_addresses::IP(){
    return resolved_addresses::IP(addresses_);
}


std::vector<std::string> resolved_addresses::IPs(){
    std::vector<std::string> result;
    for(addrinfo* p = addresses_; p != nullptr; p = p->ai_next){
        result.push_back(IP(p));
    }
    return result;
}

