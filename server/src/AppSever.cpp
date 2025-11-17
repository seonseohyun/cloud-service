#include "AppServer.h"

AppServer::AppServer() {
    // 미들웨어 예시
    svr.set_logger([](const httplib::Request& req, const httplib::Response& res) {
        std::cout << req.method << " " << req.path << " -> " << res.status << "\n";
    });

    // 라우터 연결
    router.registerRoutes(svr);
}

void AppServer::start(const std::string& host, int port) {
    std::cout << "[server] start on " << host << ":" << port << std::endl;
    svr.listen(host.c_str(), port);
}
