#ifndef DISPLAYSERVER_NETWORK_SERVER_H
#define DISPLAYSERVER_NETWORK_SERVER_H

#include <string>
#include "displays/Display.h"

class NetworkServer {
public:
    explicit NetworkServer(std::unique_ptr<displays::Display> &display) : _messages("") {
        _display = std::move(display);
    }

    void RunServer(int port);

private:
    std::unique_ptr<displays::Display> _display;
    std::string _messages;

    void handleClientConnection(int serverSocket);

    void handleMessage();
};

#endif //DISPLAYSERVER_NETWORK_SERVER_H
