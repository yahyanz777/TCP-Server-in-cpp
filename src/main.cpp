#include "address_resolver.h"
#include <netdb.h>
#include <iostream>
#include "resolved_addresses.h"

int main(){
    address_resolver resolver;
    resolved_addresses address = resolver.resolve("googl.co","80");
    if (address.get() == nullptr){
        std::cout<< "failed\n";
    }else{
        std::cout<<"successed\n";
    }
}