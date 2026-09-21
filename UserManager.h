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
        User(int id, std::string name, std::string password) : id(id), name(name), password(password) {};
        User(std::string name, std::string password) : name(name), password(password) {};
        User(std::string name, std::string password, std::vector<std::string> contacts) :
            name(name), password(password), contacts(contacts) {}; 
    };

public:

    UserManager();

    void SaveLastID(int id);

    int LoadLastID();

    User LoadUser(std::string name);

    void SaveUser(User user);

    int Register(std::string name, std::string password);

    int Authoriz(std::string name, std::string password);

    int Deleteac(std::string name, std::string password);

};
