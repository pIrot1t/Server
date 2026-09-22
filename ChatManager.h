#pragma once
#include <string>
#include <vector>

class ChatManager
{
public:

    ChatManager();

    int CreateChat(std::string chatName);

    std::vector<int> GetChats(int userID);

    std::vector<int> GetUsersChat(int chatID);

    int AddUserToChat(int chatID, int userID);

    void AddMessage(int chatID, int userID, std::string message);

    std::vector<std::string> GetMessages(int chatID);
};
