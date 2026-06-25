#include "address_resolver.h"
#include "address_resolution_error.h"
#include "resolved_addresses.h"
#include <netdb.h>

resolved_addresses address_resolver::resolve(const std::string& host,const std::string& service)const{

    addrinfo* addresses =nullptr;
    addrinfo metadata {};

    metadata.ai_flags=AI_PASSIVE;
    metadata.ai_socktype=SOCK_STREAM;
    metadata.ai_family=AF_UNSPEC;

    int status = getaddrinfo(host.c_str(),service.c_str(),&metadata,&addresses);

    if(status != 0 ){
        throw resolution_error(gai_strerror(status));
    }
       
    return resolved_addresses(addresses);

}


//getaddrinfo (ip,port,hints<metadata>,result_addr)