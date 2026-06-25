#pragma once

#include <string>
#include "resolved_addresses.h"

struct addrinfo;

 

class address_resolver
{
    public: 
    resolved_addresses resolve(const std:: string& host, const std::string& service)const;
};