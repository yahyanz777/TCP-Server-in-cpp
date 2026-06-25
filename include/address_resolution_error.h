#pragma once
#include <stdexcept>


class resolution_error:public std:: runtime_error{
    public: 
    explicit resolution_error(const char* message);
};
