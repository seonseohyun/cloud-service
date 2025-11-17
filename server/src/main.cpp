#include "AppServer.h"

int main() {
    AppServer server;
    server.start("0.0.0.0", 8080);
    return 0;
}
