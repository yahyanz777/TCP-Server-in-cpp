#pragma once
#include <sys/socket.h>
#include <string>
#include <cstdint>
#include <string_view>
class endpoint
{
public:
    endpoint(sockaddr_storage&& address) : address_(address), length_(sizeof(address_)) {}
    endpoint(sockaddr_storage&& address, socklen_t length) : address_(address), length_(length) {}
    endpoint(sockaddr_storage&) = delete;
    std::string IP() const;
    uint16_t Port() const;
    const sockaddr* address() const noexcept;
    socklen_t length() const noexcept;

private:
    sockaddr_storage address_;
    socklen_t length_;
};