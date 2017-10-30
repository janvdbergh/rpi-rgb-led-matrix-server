#ifndef DISPLAYSERVER_NETWORK_SERVER_H
#define DISPLAYSERVER_NETWORK_SERVER_H

#include "../displays/Display.h"

class NetworkServer {
public:
    explicit NetworkServer(boost::shared_ptr<displays::Display> &display) : _messages(""), _display(display) {}

    void RunServer(int port);

private:
    boost::shared_ptr<displays::Display> _display;
    std::string _messages;

    void handleClientConnection(int serverSocket);

    void handleMessage();
};

#endif //DISPLAYSERVER_NETWORK_SERVER_H
