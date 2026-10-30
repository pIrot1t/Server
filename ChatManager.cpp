#include "ChatManager.h"

using namespace std;

ChatManager::ChatManager()
{
    const char* db_password = getenv("MYSQL_PASSWORD");

    if (db_password == nullptr)
    {
        cout << "Error: MYSQL_PASSWORD is not set" << endl;
        return;
    }

	if (mysql_init(&mysql) == nullptr)
    {
		cout << "Error: can't create MySQL-descriptor" << endl;
        return;
	}
 
	if (!mysql_real_connect(&mysql, "localhost", "root", db_password, "myMessenger", NULL, NULL, 0)) 
    {
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
        return;
	}
	else 
    {
		cout << "Success!" << endl;
        connected = true;
	}
 
	mysql_set_character_set(&mysql, "utf8mb4");
	cout << "connection characterset: " << mysql_character_set_name(&mysql) << endl;
}

ChatManager::~ChatManager()
{
    if (connected)
    {
        mysql_close(&mysql);
    }
}

int ChatManager::CreateChat(string name)
{
    const char* sql = "INSERT INTO chats (name) VALUES (?)";
    MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
    if (!stmt)
    {
        return -1;
    }

    if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
    {
        cerr << "Prepare error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    unsigned long name_len = name.length();
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)name.c_str();
    bind[0].buffer_length = name_len;
    bind[0].length = &name_len;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cerr << "Bind error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    if (mysql_stmt_execute(stmt) != 0)
    {
        cerr << "Execute error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    int chat_id = (int)mysql_stmt_insert_id(stmt);

    mysql_stmt_close(stmt);
    return chat_id;
}

int ChatManager::AddUserToChat(int chatID, int userID)
{
    const char* sql = "INSERT INTO chat_users (chat_id, user_id) VALUES (?, ?)";
    MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
    if (!stmt)
    {
        return -1;
    }

    if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
    {
        cerr << "Prepare error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &chatID;

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &userID;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
    {
        cerr << "Bind error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    if (mysql_stmt_execute(stmt) != 0)
    {
        if (mysql_stmt_errno(stmt) == 1062)
        {
            mysql_stmt_close(stmt);
            return 1;
        }

        cerr << "Execute error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    mysql_stmt_close(stmt);
    return 0;
}

vector<int> ChatManager::GetChats(int userID)
{
    vector<int> chats;

    const char* sql = "SELECT chat_id FROM chat_users WHERE user_id = ?";
	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
	if (!stmt)
	{
		return chats;
	}

	if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
	{
		cerr << "Prepare error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return chats;
	}

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &userID;

    if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		cerr << "Bind error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return chats;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		cerr << "Execute error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return chats;
	}

	if (mysql_stmt_store_result(stmt) != 0)
	{
		cerr << "Store result error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return chats;
	}

    int chat_id = 0;

    MYSQL_BIND result_bind[1];
    memset(result_bind, 0, sizeof(result_bind));

    result_bind[0].buffer_type = MYSQL_TYPE_LONG;
    result_bind[0].buffer = &chat_id;

    if (mysql_stmt_bind_result(stmt, result_bind) != 0)
    {
        cerr << "Bind result error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return chats;
    }

    while (mysql_stmt_fetch(stmt) == 0)
    {
        chats.push_back(chat_id);
    }

    mysql_stmt_close(stmt);
    return chats;
}

string ChatManager::GetChatName(int chatID)
{
	string chat_name = "";

	const char* sql = "SELECT name FROM chats WHERE id = ?";
	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
	if (!stmt)
	{
		return chat_name;
	}

	if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
	{
		cerr << "Prepare error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return chat_name;
	}

	MYSQL_BIND bind[1];
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &chatID;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		cerr << "Bind error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return chat_name;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		cerr << "Execute error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return chat_name;
	}

	if (mysql_stmt_store_result(stmt) != 0)
	{
		cerr << "Store result error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return chat_name;
	}

    char nameBuffer[129];
    unsigned long nameLength = 0;

    MYSQL_BIND result_bind[1];
    memset(result_bind, 0, sizeof(result_bind));

    result_bind[0].buffer_type = MYSQL_TYPE_STRING;
    result_bind[0].buffer = nameBuffer;
    result_bind[0].buffer_length = sizeof(nameBuffer) - 1;
    result_bind[0].length = &nameLength;

    if (mysql_stmt_bind_result(stmt, result_bind) != 0)
    {
        cerr << "Bind result error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return chat_name;
    }

    if (mysql_stmt_fetch(stmt) == 0)
    {
        nameBuffer[nameLength] = '\0';
        chat_name = nameBuffer;
    }

    mysql_stmt_close(stmt);
    return chat_name;
}

vector<string> ChatManager::GetUsersChat(int chatID)
{
    vector<string> users;

    const char* sql = "SELECT u.name FROM chat_users cu "
        "JOIN users u ON u.id = cu.user_id WHERE cu.chat_id = ?";

    MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
    if (!stmt)
	{
		return users;
	}

	if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
	{
		cerr << "Prepare error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return users;
	}

    MYSQL_BIND bind[1];
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &chatID;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		cerr << "Bind error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return users;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		cerr << "Execute error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return users;
	}

	if (mysql_stmt_store_result(stmt) != 0)
	{
		cerr << "Store result error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return users;
	}

    char nameBuffer[129];
	unsigned long nameLength = 0;

	MYSQL_BIND result_bind[1];
	memset(result_bind, 0, sizeof(result_bind));

	result_bind[0].buffer_type = MYSQL_TYPE_STRING;
	result_bind[0].buffer = nameBuffer;
	result_bind[0].buffer_length = sizeof(nameBuffer) - 1;
	result_bind[0].length = &nameLength;

	if (mysql_stmt_bind_result(stmt, result_bind) != 0)
	{
		cerr << "Bind result error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return users;
	}

	while (mysql_stmt_fetch(stmt) == 0)
	{
		nameBuffer[nameLength] = '\0';
		users.push_back(nameBuffer);
	}

	mysql_stmt_close(stmt);
	return users;
}

int ChatManager::AddMessage(int chatID, int userID, string message)
{
	const char* sql = "INSERT INTO messages (chat_id, user_id, message) VALUES (?, ?, ?)";

	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
	if (!stmt)
	{
		return -1;
	}

	if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
	{
		cerr << "Prepare error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return -1;
	}

	MYSQL_BIND bind[3];
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &chatID;

	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &userID;

	unsigned long message_len = message.length();
	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer = (void*)message.c_str();
	bind[2].buffer_length = message_len;
	bind[2].length = &message_len;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		cerr << "Bind error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return -1;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		cerr << "Execute error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return -1;
	}

	mysql_stmt_close(stmt);
	return 0;
}

vector<string> ChatManager::GetMessages(int chatID)
{
	vector<string> messages;

	const char* sql = "SELECT u.name, m.message FROM messages m "
		"JOIN users u ON u.id = m.user_id WHERE m.chat_id = ? "
		"ORDER BY m.created_at ASC";

	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
	if (!stmt)
	{
		return messages;
	}

	if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
	{
		cerr << "Prepare error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return messages;
	}

	MYSQL_BIND bind[1];
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = &chatID;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		cerr << "Bind error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return messages;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		cerr << "Execute error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return messages;
	}

	if (mysql_stmt_store_result(stmt) != 0)
	{
		cerr << "Store result error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return messages;
	}

	char nameBuffer[129];
	unsigned long nameLength = 0;

	char messageBuffer[1025];
	unsigned long messageLength = 0;

	MYSQL_BIND result_bind[2];
	memset(result_bind, 0, sizeof(result_bind));

	result_bind[0].buffer_type = MYSQL_TYPE_STRING;
	result_bind[0].buffer = nameBuffer;
	result_bind[0].buffer_length = sizeof(nameBuffer) - 1;
	result_bind[0].length = &nameLength;

	result_bind[1].buffer_type = MYSQL_TYPE_STRING;
	result_bind[1].buffer = messageBuffer;
	result_bind[1].buffer_length = sizeof(messageBuffer) - 1;
	result_bind[1].length = &messageLength;

	if (mysql_stmt_bind_result(stmt, result_bind) != 0)
	{
		cerr << "Bind result error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return messages;
	}

	while (mysql_stmt_fetch(stmt) == 0)
	{
		nameBuffer[nameLength] = '\0';
		messageBuffer[messageLength] = '\0';

		messages.push_back(string(nameBuffer) + " > " + messageBuffer);
	}

	mysql_stmt_close(stmt);
	return messages;
}
