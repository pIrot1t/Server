#include "UserManager.h"

using namespace std;

UserManager::UserManager()
{
    const char* db_password = getenv("MYSQL_PASSWORD");

	if (mysql_init(&mysql) == nullptr)
    {
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
 
	if (!mysql_real_connect(&mysql, "localhost", "root", db_password, "testdb", NULL, NULL, 0)) 
    {
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else 
    {
		cout << "Success!" << endl;
	}
 
	mysql_set_character_set(&mysql, "utf8mb4");
	cout << "connection characterset: " << mysql_character_set_name(&mysql) << endl;
}

UserManager::~UserManager()
{
    mysql_close(&mysql);
}

int UserManager::Register(string name, string pass_hash)
{
    const char* sql = "INSERT INTO users (name, pass_hash) VALUES (?, ?)";
    MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
    if (!stmt)
    {
        return 1;
    }

    if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
    {
        cerr << "Prepare error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return 1;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    unsigned long name_len = name.length();
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)name.c_str();
    bind[0].buffer_length = name_len;
    bind[0].length = &name_len;

    unsigned long pass_len = pass_hash.length();
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)pass_hash.c_str();
    bind[1].buffer_length = pass_len;
    bind[1].length = &pass_len;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        cout << "Bind error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return 1;
    }
    
    if (mysql_stmt_execute(stmt) != 0)
    {
        cout << "Execute error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return 1;
    }

    mysql_stmt_close(stmt);
    return 0;
}

int UserManager::Authoriz(string name, string pass_hash)
{
    const char* sql = "SELECT id FROM users WHERE name = ? AND pass_hash = ?";

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

    MYSQL_BIND input_bind[2];
    memset(input_bind, 0, sizeof(bind));

    unsigned long name_len = name.length();
    input_bind[0].buffer_type = MYSQL_TYPE_STRING;
    input_bind[0].buffer = (void*)name.c_str();
    input_bind[0].buffer_length = name_len;
    input_bind[0].length = &name_len;

    unsigned long pass_len = pass_hash.length();
    input_bind[1].buffer_type = MYSQL_TYPE_STRING;
    input_bind[1].buffer = (void*)pass_hash.c_str();
    input_bind[1].buffer_length = pass_len;
    input_bind[1].length = &pass_len;

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

    if (mysql_stmt_store_result(stmt) != 0)
    {
        cerr << "Store result error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    int user_id = -1;

    MYSQL_BIND result_bind[1];
    memset(result_bind, 0, sizeof(result_bind));

    result_bind[0].buffer_type = MYSQL_TYPE_LONG;
    result_bind[0].buffer = &user_id;

    if (mysql_stmt_bind_result(stmt, result_bind) != 0)
    {
        cerr << "Bind result error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    if (mysql_stmt_fetch(stmt) != 0)
    {
        user_id = -1;
    }

    mysql_stmt_close(stmt);
    return user_id;
}

int UserManager::Delteac(string name, string pass_hash)
{
    const char* sql = "DELETE FROM users WHERE name = ? AND pass_hash = ?";
    MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
    if (!stmt)
    {
        return 1;
    }

    if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
    {
        cerr << "Prepare error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return 1;
    }

    MYSQL_BIND bind[2];
	memset(bind, 0, sizeof(bind));

	unsigned long name_len = name.length();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = (void*)name.c_str();
	bind[0].buffer_length = name_len;
	bind[0].length = &name_len;

	unsigned long pass_len = pass_hash.length();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (void*)pass_hash.c_str();
	bind[1].buffer_length = pass_len;
	bind[1].length = &pass_len;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		cout << "Bind error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return 1;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		cout << "Execute error: " << mysql_stmt_error(stmt) << endl;
		mysql_stmt_close(stmt);
		return 1;
	}

    mysql_stmt_close(stmt);
    return 0;
}
