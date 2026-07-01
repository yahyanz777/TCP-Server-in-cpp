#include "endpoint.hpp"
#include "resolved_addresses.hpp"

std::string endpoint::IP()const{
    return resolved_addresses::IP(reinterpret_cast<const sockaddr*>(&address_));
}

uint16_t endpoint::Port()const{
    return resolved_addresses::Port(reinterpret_cast<const sockaddr*>(&address_));
}

const sockaddr* endpoint::address() const noexcept
{
    return reinterpret_cast<const sockaddr*>(&address_);
}

socklen_t endpoint::length() const noexcept
{
    return length_;
}