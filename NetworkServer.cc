#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <regex>

#include "NetworkServer.h"

std::basic_regex<char> REGEX_CLEAR("^clear\\s*");
std::basic_regex<char> REGEX_COLOR("^color (\\d+) (\\d+) (\\d+)\\s*");
std::basic_regex<char> REGEX_SHOW("^show\\s*");
std::basic_regex<char> REGEX_PIXEL("^pixel (\\d+) (\\d+)\\s*");
std::basic_regex<char> REGEX_RECTANGLE("^rectangle (\\d+) (\\d+) (\\d+) (\\d+)\\s*");
std::basic_regex<char> REGEX_DIGIT("^digit (\\d+) (\\d+)\\s*");

void NetworkServer::RunServer(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error opening server socket");
        return;
    }

    struct sockaddr_in serverAddress;
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error on binding");
        return;
    }
    listen(serverSocket, 0);

    while(true) {
        handleClientConnection(serverSocket);
    }

    close(serverSocket);
}

void NetworkServer::handleClientConnection(int serverSocket) {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
        perror("Error on accept");
        return;
    }

    char buffer[256];
    bzero(buffer, 256);

    int bytesRead = read(clientSocket, buffer, 255);
    while (bytesRead>0) {
        buffer[bytesRead] = '\0';
        handleMessage(buffer);
        bytesRead = read(clientSocket, buffer, 255);
    }
    if (bytesRead < 0) {
        perror("Error reading from socket");
    }
    close(clientSocket);
}

void NetworkServer::handleMessage(std::string message) {
    std::smatch match;

    if (regex_match(message, REGEX_CLEAR)) {
        std::cout << "Clear" << std::endl;
        _ledMatrixDisplay.Clear();
    } else if (regex_match(message, REGEX_SHOW)) {
        std::cout << "Show" << std::endl;
        _ledMatrixDisplay.Show();
    } else if (regex_match(message, match, REGEX_COLOR)) {
        std::cout << "Color" << std::endl;
        int r = atoi(match[1].str().c_str());
        int g = atoi(match[2].str().c_str());
        int b = atoi(match[3].str().c_str());

        _ledMatrixDisplay.SetColor(r, g, b);
    } else if (regex_match(message, match, REGEX_PIXEL)) {
        std::cout << "Pixel" << std::endl;
        int x = atoi(match[1].str().c_str());
        int y = atoi(match[2].str().c_str());

        _ledMatrixDisplay.DrawPixel(x, y);
    } else if (regex_match(message, match, REGEX_RECTANGLE)) {
        std::cout << "Rectangle" << std::endl;
        int x = atoi(match[1].str().c_str());
        int y = atoi(match[2].str().c_str());
        int width = atoi(match[3].str().c_str());
        int height = atoi(match[4].str().c_str());

        _ledMatrixDisplay.DrawRectangle(x, y, width, height);
    } else if (regex_match(message, match, REGEX_DIGIT)) {
        std::cout << "Digit" << std::endl;
        int position = atoi(match[1].str().c_str());
        int digit = atoi(match[2].str().c_str());

        _ledMatrixDisplay.DrawDigit(position, digit);
    } else {
        std::cerr << "Invalid message **" << message << "**" << std::endl;
    }
}
