# Server

Создать базу данных
```sh
mysql -u root -p < schema.sql
```

Собрать
```sh
g++ *.cpp -o server -std=c++17 $(mysql_config --cflags --libs)
```

Запускать так
```sh
MYSQL_PASSWORD='your_password' ./server
```
