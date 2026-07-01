#include "udp_datagram.hpp"


void udp_datagram::recvd_info(std::ostream &os) const {
    os << sender.IP() << ':' << sender.Port();
}
