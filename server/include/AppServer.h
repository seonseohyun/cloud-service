#pragma once
#include <httplib.h>
#include "Router.h"

class AppServer {
public:
    AppServer();
    void start(const std::string& host, int port);

private:
    httplib::Server svr;
    Router router;
};
