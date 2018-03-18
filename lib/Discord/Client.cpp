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

        void Client::onEvent(function<void (WebSocket<CLIENT> *ws, json data)> handler) {
            this->eventHandler = handler;
        }

        void Client::connect() {
            json body = getGateway(true);
            this->shards = body.at("shards").get<int>();
            string url = body.at("url").get<string>();

            h.onConnection([this](WebSocket<CLIENT> *ws, HttpRequest /* req */) {
                this->ws = ws;
                cout << "(DISCORD) [INFO] Connected to Discord" << endl;
            });

            h.onMessage([this](WebSocket<CLIENT> *ws, char *msg, size_t len, OpCode op) {
                string message = static_cast<string>(msg).substr(0, len);

                this->onMessage(ws, message);
            });

            h.onDisconnection([this](WebSocket<CLIENT> *ws, int code, char *msg, size_t len) {
                string message = static_cast<string>(msg).substr(0, len);

                this->hbThread.~thread();

                cout << "(DISCORD) [INFO] Disconnected from Discord: "  << message << " (" << code << ")" << endl;

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

                cout << "(DISCORD) [DEBUG] Sent heartbeat to Discord" << endl;
                ws->send(toSend.dump().c_str());
            }
        }

        void Client::sendMessage(string channelID, json body) {
            string url = static_cast<string>(API_BASE) + "/channels/" + channelID + "/messages";
            map<string, string> headers = {
                {"Authorization", "Bot " + token}, 
                {"Content-Type", "application/json"}
            };

            string *res = Http::post(url, headers, body.dump());

            cout << *res << endl;
        }

        void Client::onMessage(WebSocket<CLIENT> *ws, string msg) {
            json j = json::parse(msg);
            
            if (this->eventHandler != NULL) {
                this->eventHandler(ws, j);
            }

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
                        user = d.at("user").get<json>();
                    } else if (t == "GUILD_CREATE") {
                        guilds.push_back(d);
                    } else {
                        cout << "(DISCORD) [WARN] Unhandled event: " << t << endl;
                    }
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

                        hbThread = thread(doHeartbeat, heartbeatInterval, ws);
                        hbThread.detach();
                    }
                }
            }
        }
    }
}
