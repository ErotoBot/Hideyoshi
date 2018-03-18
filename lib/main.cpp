#include <iostream>

#include "Server.hpp"

using namespace std;
using namespace Hideyoshi;

int main() {
    Server server(getenv("BOT_TOKEN"), 6080);
    return 0;
}
