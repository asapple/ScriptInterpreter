#include "SQL.h"
void SQL::finditem(SOCKET clntSock, std::string key, std::string& item) {
    if (key == "$name") {
        item = "Àî»ª";
    } else {
        item = "1025";
    }
}