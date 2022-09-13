#ifndef PUSH_H
#define DEVICE_H

#include <string>

struct Push
{
        std::string iden;
        bool active;
        float created;
        float modified;
        std::string type;
        bool dismissed;      
};
        
