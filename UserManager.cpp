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

void UserManager::SaveUser(User user, int id)
{
    fstream file;

    if (!fs::exists("Users/" + to_string(id)))
    {
        fs::create_directory("Users/" + to_string(id));
    }

    file.open("Users/" + to_string(id) + "name.txt", ios::out | ios::trunc);
    file << user.name;
    file.close();

    file.open("Users/" + to_string(id) + "password.txt");
    file << user.password;
    file.close();
}

UserManager::User* UserManager::LoadUser(string name)
{
    User user;
    fstream file;
    int id = LoadLastID();

    for (int i = 0; i <= id; i++)
    {
        file.open("Users/" + to_string(i) + "/name.txt", ios::in);
        file >> user.name;

        if (user.name == name)
        {
            file.open("Users/" + to_string(i) + "/password.txt", ios::in);
            file >> user.password;

            user.id = i;

            return &user;
        }
    }

    return nullptr;
}

int UserManager::Register(User user)
{
    fstream file;
    int id = LoadLastID();

    for (int i = 0; i < id; i++)
    {
        string name;
        file.open("Users/" + to_string(id) + "name.txt", ios::in);
        file >> name;

        if (user.name == name)
        {
            file.close();
            return 1;
        }

        file.close();
    }

    SaveUser(user, id);

    SaveLastID(id);

    return 0;
}
