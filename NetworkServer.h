#ifndef DISPLAYSERVER_NETWORK_SERVER_H
#define DISPLAYSERVER_NETWORK_SERVER_H

#include <string>
#include "LedMatrixDisplay.h"

class NetworkServer {
public:
    NetworkServer(LedMatrixDisplay& ledMatrixDisplay): _ledMatrixDisplay(ledMatrixDisplay), _messages("") {}

    void RunServer(int port);
private:
    LedMatrixDisplay& _ledMatrixDisplay;
    std::string _messages;

    void handleClientConnection(int serverSocket);
    void handleMessage();
};

#endif //DISPLAYSERVER_NETWORK_SERVER_H
