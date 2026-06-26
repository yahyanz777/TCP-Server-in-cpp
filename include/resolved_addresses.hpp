#pragma once

#include<netdb.h>
#include <string>
#include <arpa/inet.h>
#include <vector>
#include <sys/socket.h>
class resolved_addresses{
    public: 
        explicit resolved_addresses(addrinfo* addresses);
    
        ~resolved_addresses();

        addrinfo* get() const;
        std::string IP();
        std::vector<std::string> IPs();

        
    private:
        addrinfo* addresses_;
        std::string IP(addrinfo* addr);
};