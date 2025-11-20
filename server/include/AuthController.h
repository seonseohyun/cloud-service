#pragma once
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "DBManager.h"

class AuthController {
public:
    void login(const httplib::Request& req, httplib::Response& res);
    // void signup(const httplib::Request& req, httplib::Response& res);

private:
    DBManager db;
};
