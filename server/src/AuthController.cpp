#include "AuthController.h"
using json = nlohmann::json;

void AuthController::login(const httplib::Request& req, httplib::Response& res) {
    json body = json::parse(req.body, nullptr, false);
    if (!body.is_object()) {
        res.status = 400;
        res.set_content("{\"error\":\"invalid json\"}", "application/json");
        return;
    }

    std::string id = body["id"];
    std::string pw = body["pw"];

    bool ok = db.checkLogin(id, pw);

    json resp;
    resp["result"] = ok ? "success" : "fail";

    res.set_content(resp.dump(), "application/json");
}

// void AuthController::signup(const httplib::Request& req, httplib::Response& res) {
//     json body = json::parse(req.body, nullptr, false);
//     if (!body.is_object()) {
//         res.status = 400;
//         return;
//     }

//     bool ok = db.createUser(body["id"], body["pw"], body["email"]);

//     json resp;
//     resp["result"] = ok ? "success" : "fail";
//     res.set_content(resp.dump(), "application/json");
// }
