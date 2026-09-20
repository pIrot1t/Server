#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

class UserManager
{
private:
    struct User
    {
        int id;
        std::string name;
        std::string password;
        std::vector<std::string> contacts;

        User() : id(-1), name(""), password("") {};
        User(std::string name, std::string password) : name(name), password(password) {};
        User(std::string name, std::string password, std::vector<std::string> contacts) :
            name(name), password(password), contacts(contacts) {}; 
    };

public:

    UserManager();

    void SaveLastID(int id);

    int LoadLastID();

    User* LoadUser(std::string name);

    void SaveUser(User user, int id);

    int Register(User user);

    int Authoriz(User user);

    int Deleteac(User user);

};
