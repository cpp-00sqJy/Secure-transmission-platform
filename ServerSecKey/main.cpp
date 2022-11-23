#include <cstdio>
#include "ServerOP.h"




int main()
{
    ServerOP serOP("server.json");
    
    serOP.startServer();



    return 0;
}