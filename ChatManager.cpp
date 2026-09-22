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

    file.open("Chat/LastID.txt", ios::in);
    file >> id;
    file.close();

    fs::create_directory("Chat/" + to_string(id));

    file.open("Chat/" + to_string(id) + "/chatname.txt", ios::out | ios::trunc);
    file << chatName;
    file.close();

    file.open("Chat/LastID.txt", ios::out | ios::trunc);
    file << id + 1;
    file.close();

    return id;
}

vector<int> ChatManager::GetChats(int userID)
{
    fstream file;
    int lastID;
    vector<int> chatsID;

    file.open("Chat/LastID.txt", ios::in);
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
                chatsID.push_back(id);
            }
        }
        file.close();
    }

    return chatsID;
}

vector<int> ChatManager::GetUsersChat(int chatID)
{
    fstream file;
    vector<int> usersID;

    file.open("Chats/" + to_string(chatID) + "/users.txt", ios::in);
    int id;
    while (file >> id)
    {
        usersID.push_back(id);
    }
    file.close();

    return usersID;
}

int ChatManager::AddUserToChat(int chatID, int userID)
{
    if (!fs::exists("Users/" + to_string(userID)))
    {
        return 1;
    }

    if (!fs::exists("Chats/" + to_string(chatID) + "/users.txt"))
    {
        ofstream("Chats/" + to_string(chatID) + "/users.txt");
    }

    fstream file;

    file.open("Chats/" + to_string(chatID) + "/users.txt", ios::out);
    file.seekp(0, ios_base::end);
    file << userID << endl;
    file.close();

    return 0;
}

void ChatManager::AddMessage(int chatID, int userID, string message)
{
    if (!fs::exists("Chats/" + to_string(chatID) + "/messages.txt"))
    {
        ofstream("Chats/" + to_string(chatID) + "/messages.txt");
    }

    fstream file;
    string username;

    file.open("Users/" + to_string(userID) + "/name.txt", ios::in);
    file >> username;
    file.close();

    file.open("Chats/" + to_string(chatID) + "/messages.txt", ios::out);
    file.seekp(0, ios_base::end);
    file << username << " > " << message << endl;
    file.close();
}

vector<string> ChatManager::GetMessages(int chatID)
{
    fstream file;
    vector<string> messages;

    file.open("Chats/" + to_string(chatID) + "messages.txt", ios::in);
    string message;
    while (file >> message)
    {
        messages.push_back(message);
    }
    file.close();

    return messages;
}
