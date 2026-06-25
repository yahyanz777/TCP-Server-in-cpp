#include "resolved_addresses.h"

resolved_addresses::resolved_addresses(addrinfo* addresses):addresses_(addresses){}

resolved_addresses::~resolved_addresses(){
    if(addresses_)
        freeaddrinfo(addresses_);
}

addrinfo* resolved_addresses::get() const{
    return addresses_;
}



