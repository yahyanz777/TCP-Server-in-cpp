#include "address_resolution_error.h"

resolution_error::resolution_error(const char* message):std::runtime_error(message){
    
}