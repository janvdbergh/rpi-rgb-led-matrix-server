#ifndef DISPLAYSERVER_NETWORK_SERVER_H
#define DISPLAYSERVER_NETWORK_SERVER_H

#include <string>
#include "LedMatrixDisplay.h"

class NetworkServer {
public:
    NetworkServer(LedMatrixDisplay& ledMatrixDisplay): _ledMatrixDisplay(ledMatrixDisplay) {}

    void RunServer(int port);
private:
    LedMatrixDisplay& _ledMatrixDisplay;

    void handleClientConnection(int serverSocket);
    void handleMessage(std::string message);
};

#endif //DISPLAYSERVER_NETWORK_SERVER_H
