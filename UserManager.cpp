#include "UserManager.h"
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

namespace fs = filesystem;

UserManager::UserManager()
{
    if (!fs::exists("Users"))
    {
        fs::create_directory("Users");
    }

    if (!fs::exists("Users/LastID.txt"))
    {
        fstream file;
        file.open("Users/LastID.txt", ios::out | ios::trunc);
        file << 0;
        file.close();
    }
}

void UserManager::SaveLastID(int id)
{
    fstream file;
    file.open("Users/LastID.txt", ios::out | ios::trunc);
    file << ++id;
    file.close();
}

int UserManager::LoadLastID()
{
    int id;
    fstream file;
    file.open("Users/LastID.txt", ios::in);
    file >> id;
    file.close();

    return id;
}

void UserManager::SaveUser(User user)
{
    fstream file;

    if (!fs::exists("Users/" + to_string(user.id)))
    {
        fs::create_directory("Users/" + to_string(user.id));
    }

    file.open("Users/" + to_string(user.id) + "/name.txt", ios::out | ios::trunc);
    file << user.name;
    file.close();

    file.open("Users/" + to_string(user.id) + "/password.txt", ios::out | ios::trunc);
    file << user.password;
    file.close();
}

UserManager::User UserManager::LoadUser(string name)
{
    User user;
    fstream file;
    int id = LoadLastID();

    for (int i = 0; i < id; i++)
    {
        file.open("Users/" + to_string(i) + "/name.txt", ios::in);
        file >> user.name;
        file.close();

        if (user.name == name)
        {
            file.open("Users/" + to_string(i) + "/password.txt", ios::in);
            file >> user.password;

            user.id = i;

            file.close();

            return user;
        }
    }

    return User();
}

int UserManager::Register(string name, string password)
{
    fstream file;
    int id = LoadLastID();

    for (int i = 0; i < id; i++)
    {
        string _name;
        file.open("Users/" + to_string(i) + "/name.txt", ios::in);
        file >> _name;

        if (name == _name)
        {
            file.close();
            return 1;
        }

        file.close();
    }

    SaveUser({id, name, password});

    SaveLastID(id);

    return 0;
}

int UserManager::Authoriz(string name, string password)
{
    User user = LoadUser(name);
    
    if (user.id == -1)
    {
        return -1;
    }
    else
    {
        if (user.password == password)
        {
            return user.id;
        }
        else
        {
            return -2;
        }
    }
}

int UserManager::Deleteac(string name, string password)
{
    User user = LoadUser(name);

    if (user.id == -1)
    {
        return 1;
    }
    else
    {
        if (user.password == password)
        {
            fs::remove_all("Users/" + to_string(user.id));
            return 0;
        }
        else
        {
            return 1;
        }
    }
}
