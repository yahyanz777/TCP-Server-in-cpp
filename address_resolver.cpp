#include "address_resolver.h"


addrinfo* address_resolver::resolve(const std::string& host,const std::string& port){
    addrinfo* addr_list =nullptr;
    addrinfo hints {} ;
    
}



//getaddrinfo (ip,port,hints<metadata>,result_addr)