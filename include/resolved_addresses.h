#pragma once

#include<netdb.h>
#include <string>
#include <arpa/inet.h>
class resolved_addresses{
    public: 
        explicit resolved_addresses(addrinfo* addresses);
    
        ~resolved_addresses();

        addrinfo* get() const;
       
    private:
        addrinfo* addresses_;
};