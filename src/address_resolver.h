#pragma once

#include <netdb.h>
#include <string>

class address_resolver
{
public:
    static addrinfo *resolve(const std::string &host, const std::string &port);
};