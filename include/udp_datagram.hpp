#pragma once
#include <vector>
#include<endpoint.hpp>
#include <iostream>
#include <cstddef>
#include <utility>
class udp_datagram
{
public:
    udp_datagram() = default;
    udp_datagram(std::vector<std::byte>&& data, endpoint&& sender)
        : data(std::move(data)), size(this->data.size()), sender(std::move(sender)) {}
    udp_datagram(std::size_t size, endpoint&& sender)
        : size(size), sender(std::move(sender)) {}
    std::vector<std::byte> data{};
    std::size_t size{};
    endpoint sender;

    std::string_view text() const
    {
        return {reinterpret_cast<const char *>(data.data()), data.size()};
    }

    void recvd_info(std::ostream &os = std::cout) const;
};