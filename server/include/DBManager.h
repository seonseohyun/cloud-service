#pragma once
#include <string>
#include <memory>
#include <mariadb/mysql.h>

class DBManager {
    
public:
    DBManager();
    ~DBManager();

    bool checkLogin(const std::string& id, const std::string& pw);
//     bool createUser(const std::string& id, const std::string& pw, const std::string& email);

private:
    MYSQL* conn; //커넥터 핸들
    bool connect(); //연결
};
