#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

class UserManager
{
    UserManager();

    int Register(string name, string password);

    int Authoriz(string name, string password);

    int Deleteac(string name, string password);
};
