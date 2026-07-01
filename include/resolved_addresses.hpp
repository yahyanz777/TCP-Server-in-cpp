#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <vector>

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
        static uint16_t Port(const sockaddr* addr);
        static std::string IP(const sockaddr* addr);
        std::vector<std::string> IPs();

        
    private:
        addrinfo* addresses_;
};