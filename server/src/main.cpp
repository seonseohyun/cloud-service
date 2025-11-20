#include "AppServer.h"
/*main: 서버 켜

AppServer: 서버 환경 세팅

Router: URL 매핑

AuthController: 로직 처리

DBManager: DB 처리*/

int main() {
    AppServer server;
    server.start("0.0.0.0", 8080);
    return 0;
}
