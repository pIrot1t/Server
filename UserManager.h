#pragma once
#include <iostream>
#include <string>
#include <mysql/mysql.h>

class UserManager
{
private:

    MYSQL mysql;
    MYSQL_RES* res;
    MYSQL_ROW row;
    bool connected = false;

public:

    UserManager();

    ~UserManager();

    int Register(std::string name, std::string pass_hash);

    int Authoriz(std::string name, std::string pass_hash);

    int Deleteac(std::string name, std::string pass_hash);

};
