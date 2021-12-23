#pragma once
#include<WinSock2.h>
#include<string>
class SQL {
public:
    static void finditem(SOCKET clntSock, std::string key, std::string& item);
};

