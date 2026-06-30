#pragma once
#include<unistd.h>

class connection{
    public:
        connection();
        ~connection(){
            close(cfd_);
        }
        int get_epfd(){
            return epfd_;
        }
        int get_cfd(){
            return cfd_;
        }
    private:
        int cfd_;
        int epfd_;
};