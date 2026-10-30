#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <mysql/mysql.h>

class ChatManager
{
private:
    
    MYSQL mysql;
    bool connected = false;

public:

    ChatManager();

    ~ChatManager();

    int CreateChat(std::string chatName);

    std::vector<int> GetChats(int userID);

    std::string GetChatName(int chatID);

    std::vector<std::string> GetUsersChat(int chatID);

    int AddUserToChat(int chatID, int userID);

    int AddMessage(int chatID, int userID, std::string message);

    std::vector<std::string> GetMessages(int chatID);
};
