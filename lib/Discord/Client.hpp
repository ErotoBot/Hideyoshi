#ifndef DISCORD_CLIENT_HPP
#define DISCORD_CLIENT_HPP

#ifndef API_BASE
#define API_BASE "https://discordapp.com/api/v6"
#endif

#include <string>
#include <thread>
#include <map>

#include <nlohmann/json.hpp>
#include <uWS/uWS.h>

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
                thread hbThread;
                int heartbeatInterval;
                function<void (WebSocket<CLIENT> *, json)> eventHandler;

                json getGateway(bool);
                void onMessage(WebSocket<CLIENT> *, string);
            public:
                json user;
                vector<json> guilds;
                vector<json> users;

                string token;
                int shards;

                Client(string);
                void connect();
                void sendMessage(string, json);
                void onEvent(function<void (WebSocket<CLIENT> *, json)>);
        };
    }
}

#endif
