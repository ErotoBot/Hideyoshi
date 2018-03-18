#include <iostream>
#include <thread>

#include "Server.hpp"

namespace Hideyoshi {
    Server::Server(string token, int port) {
        this->client = new Discord::Client(token);

        this->h.onConnection([this](WebSocket<SERVER> *ws, HttpRequest req) {
            Header auth = req.getHeader("authentication");

            if (auth.value == NULL) {
                ws->close();
            } else {
                socks.push_back(ws);
            }

            cout << "(SERVER) [INFO] New connection from " << ws->getAddress().address << endl;
        });

        this->h.onDisconnection([this](WebSocket<SERVER> *ws, int code, char *msg, size_t len) {
            socks.erase(remove(socks.begin(), socks.end(), ws), socks.end());
        });

        this->client->onEvent([this](WebSocket<CLIENT> *ws, json payload) {
            if (socks.empty()) {
                messageQueue.push(payload);
            } else {
                for (int i = 0; i < (int) messageQueue.size(); i++) {
                    json msg = messageQueue.front();
                    messageQueue.pop();

                    for (WebSocket<SERVER> *sock : socks) {
                        sock->send(msg.dump().c_str());
                    }
                }

                for (WebSocket<SERVER> *sock : socks) {
                    sock->send(payload.dump().c_str());
                }
            }
        });

        if (h.listen(port)) {
            thread discordThread([this]() {
                client->connect();
            });

            discordThread.detach();

            h.run();
        }
    }
}
