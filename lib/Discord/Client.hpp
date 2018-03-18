#ifndef DISCORD_CLIENT_HPP
#define DISCORD_CLIENT_HPP

#ifndef API_BASE
#define API_BASE "https://discordapp.com/api/v6"
#endif

#include <string>
#include <thread>

#include <nlohmann/json.hpp>
#include <uWS/uWS.h>

#include "./struct/User.hpp"

using namespace std;
using namespace uWS;
using json = nlohmann::json;

namespace Hideyoshi {
    namespace Discord {
        class Client {
            private:
                Hub h;
                WebSocket<CLIENT> *ws;
                bool ready;
                int heartbeatInterval;

                json getGateway(bool);
                void onMessage(WebSocket<CLIENT> *, string);
            public:
                user_t user;
                string token;
                int shards;

                Client(string);
                void connect();
        };
    }
}

#endif
