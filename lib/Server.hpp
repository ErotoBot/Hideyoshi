#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <string>
#include <uWS/uWS.h>

#include "Discord/Client.hpp"

using namespace std;
using namespace uWS;

namespace Hideyoshi {
    class Server {
        private:
            Hub h;
            Discord::Client *client;

            list<WebSocket<SERVER> *> socks;
            void onMessage(WebSocket<SERVER> *, string);
            void onConnect(WebSocket<SERVER> *);
            void onClose(WebSocket<SERVER> *, string, int);

        public:
            Server(string, int);
    };
}

#endif
