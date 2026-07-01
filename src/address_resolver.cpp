#include "address_resolver.hpp"
#include <stdexcept>
#include "resolved_addresses.hpp"
#include <netdb.h>

namespace address_resolver{
resolved_addresses resolve(const std::string& host, const std::string& service, int socktype) {

    addrinfo* addresses =nullptr;
    addrinfo metadata {};

    metadata.ai_flags=AI_PASSIVE;
    metadata.ai_socktype=socktype;
    metadata.ai_family=AF_UNSPEC;

    int status = getaddrinfo(host.c_str(),service.c_str(),&metadata,&addresses);

    if(status != 0 ){
        throw std::runtime_error(gai_strerror(status));
    }
       
    return resolved_addresses(addresses);

}
}


