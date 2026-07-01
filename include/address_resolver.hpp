#pragma once

#include <string>
#include <sys/socket.h>
#include "resolved_addresses.hpp"

struct addrinfo;

 

namespace address_resolver
{
    resolved_addresses resolve(const std::string& host, const std::string& service, int socktype = SOCK_STREAM);
}