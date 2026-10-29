#include "UserManager.h"

using namespace std;

UserManager::UserManager()
{
    mysql_init(&mysql);
	if (&mysql == nullptr)
    {
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
 
	if (!mysql_real_connect(&mysql, "localhost", "root", "a657833zb", "testdb", NULL, NULL, 0)) 
    {
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else 
    {
		cout << "Success!" << endl;
	}
 
	mysql_set_character_set(&mysql, "utf8");
	cout << "connection characterset: " << mysql_character_set_name(&mysql) << endl;
}

UserManager::~UserManager()
{
    mysql_close(&mysql);
}

int UserManager::Register(string name, string pass_hash)
{
    const char* sql = "INSERT INTO users (name, age) VALUES (?, ?)";
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    if (!stmt)
    {
        return 1;
    }

    if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
    {
        cout << "Prepare error: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return 1;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    unsigned long name_len = strlen(name);
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void*)name;
    bind[0].buffer_length = name_len;
    bind[0].length = &name_len;

    unsigned long pass_len = strlen(pass_hash);
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void*)pass_hash;
    bind[1].buffer_length = pass_len;
    bind[1].length = &pass_len;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        cout << "Bind error: " << mysql_stmt_error(stmt) << endl;
    }
    else
    {
        if (mysql_stmt_execute(stmt) != 0)
        {
            cout << "Execute error: " << mysql_stmt_error(stmt) << endl;
        }
    }

    mysql_stmt_close(stmt);

    return 0;
}
