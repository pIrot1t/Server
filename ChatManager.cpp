#include "ChatManager.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

using namespace std;

namespace fs = filesystem;

ChatManager::ChatManager()
{
    if (!fs::exists("Chats"))
    {
        fs::create_directory("Chats");
    }

    if (!fs::exists("Chats/LastID.txt"))
    {
        fstream file;
        file.open("Chats/LastID.txt", ios::out | ios::trunc);
        file << 0;
        file.close();
    }
}

int ChatManager::CreateChat(string chatName)
{
    fstream file;
    int id;

    file.open("Chats/LastID.txt", ios::in);
    file >> id;
    file.close();

    fs::create_directory("Chats/" + to_string(id));

    file.open("Chats/" + to_string(id) + "/chatname.txt", ios::out | ios::trunc);
    file << chatName;
    file.close();

    file.open("Chats/LastID.txt", ios::out | ios::trunc);
    file << id + 1;
    file.close();

    return id;
}

vector<int> ChatManager::GetChats(int userID)
{
    fstream file;
    int lastID;
    vector<int> chatsID;

    file.open("Chats/LastID.txt", ios::in);
    file >> lastID;
    file.close();

    for (int i = 0; i < lastID; i++)
    {
        file.open("Chats/" + to_string(i) + "/users.txt", ios::in);
        int id;
        while (file >> id)
        {
            if (id == userID)
            {
                chatsID.push_back(i);
            }
        }
        file.close();
    }

    return chatsID;
}

string ChatManager::GetChatName(int chatID)
{
    fstream file;
    string chatname;

    file.open("Chats/" + to_string(chatID) + "/chatname.txt", ios::in);
    getline(file, chatname);
    file.close();

    return chatname;
}

vector<string> ChatManager::GetUsersChat(int chatID)
{
    fstream file;
    vector<string> users;

    file.open("Chats/" + to_string(chatID) + "/users.txt", ios::in);
    int id;
    string name;
    while (file >> id)
    {
        file.close();
        file.open("Users/" + to_string(id) + "/name.txt", ios::in);
        file >> name;
        file.close();
        users.push_back(to_string(id) + "    " + name);
        file.open("Chats/" + to_string(chatID) + "/users.txt", ios::in);
    }
    file.close();

    return users;
}

int ChatManager::AddUserToChat(int chatID, int userID)
{
    if (!fs::exists("Users/" + to_string(userID)))
    {
        return 1;
    }

    fstream file;

    if (!fs::exists("Chats/" + to_string(chatID) + "/users.txt"))
    {
        file.open("Chats/" + to_string(chatID) + "/users.txt");
        file.close();
    }

    file.open("Chats/" + to_string(chatID) + "/users.txt", ios::out | ios::app);
    file << userID << endl;
    file.close();

    return 0;
}

void ChatManager::AddMessage(int chatID, int userID, string message)
{
    fstream file;
    string username;

    if (!fs::exists("Chats/" + to_string(chatID) + "/messages.txt"))
    {
        file.open("Chats/" + to_string(chatID) + "/messages.txt");
        file.close();
    }

    file.open("Users/" + to_string(userID) + "/name.txt", ios::in);
    file >> username;
    file.close();

    file.open("Chats/" + to_string(chatID) + "/messages.txt", ios::out | ios::app);
    file << username << " > " << message << endl;
    file.close();
}

vector<string> ChatManager::GetMessages(int chatID)
{
    fstream file;
    vector<string> messages;

    file.open("Chats/" + to_string(chatID) + "/messages.txt", ios::in);
    string message;
    while (getline(file, message))
    {
        messages.push_back(message);
    }
    file.close();

    return messages;
}
