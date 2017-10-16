#ifndef DISPLAYSERVER_NETWORK_SERVER_H
#define DISPLAYSERVER_NETWORK_SERVER_H

#include <string>
#include "led-matrix-display/LedMatrixDisplay.h"

class NetworkServer {
public:
    explicit NetworkServer(LedMatrixDisplay& ledMatrixDisplay): _ledMatrixDisplay(ledMatrixDisplay), _messages("") {}

    void RunServer(int port);
private:
    LedMatrixDisplay& _ledMatrixDisplay;
    std::string _messages;

    void handleClientConnection(int serverSocket);
    void handleMessage();
};

#endif //DISPLAYSERVER_NETWORK_SERVER_H
