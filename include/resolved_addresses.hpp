#pragma once

#include<netdb.h>
#include <string>
#include <arpa/inet.h>
#include <vector>
#include <sys/socket.h>
class resolved_addresses{
    public: 
        explicit resolved_addresses(addrinfo* addresses);
        resolved_addresses(const resolved_addresses&) = delete;
        resolved_addresses& operator=(const resolved_addresses&) = delete;
        resolved_addresses(resolved_addresses&& other) noexcept;
        resolved_addresses& operator=(resolved_addresses&& other) noexcept;
    
        ~resolved_addresses();

        addrinfo* get() ;
        std::string IP();
        std::vector<std::string> IPs();

        
    private:
        addrinfo* addresses_;
        std::string IP(addrinfo* addr);
};