#include "../Http.hpp"
#include "Client.hpp"
#include "OPCodes.hpp"

#include <iostream>

namespace Hideyoshi {
    namespace Discord {
        Client::Client(string token) {
            this->token = token;
        }

        json Client::getGateway(bool bot) {
            string url = static_cast<string>(API_BASE) + "/gateway";
            map<string, string> headers;

            if (bot) {
                headers["Authorization"] = "Bot " + token;
                url.append("/bot");
            }

            string *body = Http::get(url, headers);

            return json::parse(*body);
        }

        void Client::connect() {
            json body = getGateway(true);
            this->shards = body.at("shards").get<int>();
            string url = body.at("url").get<string>();

            h.onMessage([this](WebSocket<CLIENT> *ws, char *msg, size_t len, OpCode op) {
                string message = static_cast<string>(msg).substr(0, len);

                this->onMessage(ws, message);
            });

            h.onDisconnection([](WebSocket<CLIENT> *ws, int code, char *msg, size_t len) {
                string message = static_cast<string>(msg).substr(0, len);

                cout << "Disconnected: "  << message << " (" << code << ")" << endl;
            });

            h.connect(url);
            h.run();
        }

        static int seq = 0;

        static void doHeartbeat(int heartbeat, WebSocket<CLIENT> *ws) {
            while (true) {
                usleep(heartbeat * 1000);
                json toSend = {
                    {"op", OPCodes::HEARTBEAT},
                    {"d", seq}
                };

                cout << "Hearbeat: " << toSend.dump(4) << endl;
                ws->send(toSend.dump().c_str());
            }
        }

        void Client::onMessage(WebSocket<CLIENT> *ws, string msg) {
            this->ws = ws;
            json j = json::parse(msg);
            json toSend;
            
            int op = j.at("op").get<int>();

            switch (op) {
                case OPCodes::DISPATCH: {
                    string t = j.at("t").get<string>();
                    json d = j.at("d").get<json>();

                    if (!j.at("s").is_null()) {
                        seq = j.at("s").get<int>();
                    }

                    if (t == "READY") {
                        json user = d.at("user").get<json>();

                        this->user.username = user.at("username").get<string>();
                        this->user.id = user.at("id").get<string>();
                        this->user.discriminator = user.at("discriminator").get<string>();
                        this->user.bot = user.at("bot").get<bool>();
                        this->user.avatar = "https://cdn.discordapp.com/avatars/" + this->user.id + "/" + user.at("avatar").get<string>() + ".png";
                    }

                    cout << "Event: " << t << endl;
                    // cout << "Data: " << d.dump(4) << endl;
                }

                case OPCodes::HELLO: {
                    json d = j.at("d").get<json>();

                    if (d.find("heartbeat_interval") != d.end()) {
                        heartbeatInterval = d.at("heartbeat_interval").get<int>();
                    }

                    toSend = {
                        {"op", OPCodes::IDENTIFY},
                        {"d", {
                            {"token", token},
                            {"properties", {
                                {"$os", "linux"},
                                {"$browser", "Hideyoshi"},
                                {"$device", "Hideyoshi"}
                            }},
                            {"compress", false},
                            {"large_threshold", 250},
                            {"shard", {0, shards}}           
                        }}
                    };

                    if (!ready) {
                        ws->send(toSend.dump().c_str());
                        ready = true;

                        thread hbThread(doHeartbeat, heartbeatInterval, ws);
                        hbThread.detach();
                    }
                }
            }
        }
    }
}
