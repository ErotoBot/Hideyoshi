#include <iostream>

#include "Discord/Client.hpp"

using namespace std;
using namespace Hideyoshi;

int main() {
    /*Hub h;

    h.onConnection([](WebSocket<SERVER> *ws, HttpRequest req) {
        cout << "New connection!" << endl;
    });*/

    Discord::Client bot(getenv("BOT_TOKEN"));

    bot.connect();

    /*if (h.listen(6080)) {
        h.run();
    }*/
}
