#include "SQL.h"
void SQL::finditem(SOCKET clntSock, std::string key, std::string& item) {
    if (key == "$name") {
        item = "�";
    } else {
        item = "1025";
    }
}