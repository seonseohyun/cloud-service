#pragma once
#include <string>

class DBManager {
public:
    DBManager();
    bool checkLogin(const std::string& id, const std::string& pw);
    bool createUser(const std::string& id, const std::string& pw, const std::string& email);

private:
    void connect();
};
