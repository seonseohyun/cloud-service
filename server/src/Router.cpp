#include "Router.h"

void Router::registerRoutes(httplib::Server& svr) {

    AuthController auth;
    // FileController files;

    // health
    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("{\"status\":\"ok\"}", "application/json");
    });

    // Auth
    svr.Post("/login", [&](const auto& req, auto& res) { auth.login(req, res); });
    svr.Post("/signup", [&](const auto& req, auto& res) { auth.signup(req, res); });

    // // Files
    // svr.Post("/files/upload", [&](const auto& req, auto& res) { files.upload(req, res); });
    // svr.Get("/files/list", [&](const auto& req, auto& res) { files.list(req, res); });
}
