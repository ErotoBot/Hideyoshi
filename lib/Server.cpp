#include <iostream>
#include <thread>

#include "Server.hpp"

namespace Hideyoshi {
    Server::Server(string token, int port) {
        this->client = new Discord::Client(token);

        this->h.onConnection([this](WebSocket<SERVER> *ws, HttpRequest req) {
            Header auth = req.getHeader("Authentication");

            if (auth != 0) {
                ws->close();
            } else {
                this->socks.push_back(ws);
            }

            cout << "(SERVER) [INFO] New connection from " << ws->getAddress().address << endl;
        });

        this->h.onDisconnection([this](WebSocket<SERVER> *ws, int code, char *msg, size_t len) {
            this->socks.erase(remove(this->socks.begin(), this->socks.end(), ws), this->socks.end());
        });

        this->client->onEvent([this](WebSocket<CLIENT> *ws, json payload) {
            for (WebSocket<SERVER> *sock : this->socks) {
                sock->send(payload.dump().c_str());
            }
        });

        thread discordThread([this]() {
            this->client->connect();
        });

        discordThread.detach();

        if (h.listen(port)) {
            h.run();;
        }
    }
}
