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

    if (mysql_stmt_prepare(stmt, sql, srtlen(sql)) != 0)
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
