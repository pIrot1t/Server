#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <mysql/mysql.h>

class UserManager
{
private:

    MYSQL mysql;
    bool connected = false;

public:

    UserManager();

    ~UserManager();

    int Register(std::string name, std::string pass_hash);

    int Authoriz(std::string name, std::string pass_hash);

    int Deleteac(std::string name, std::string pass_hash);

};
