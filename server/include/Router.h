#pragma once
#include <httplib.h>
#include "AuthController.h"
// #include "FileController.h"

class Router {
public:
    void registerRoutes(httplib::Server& svr);
};