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
std::basic_regex<char> REGEX_TEXT("^text (\\d+) (\\d+) \"([^\"]*)\"\\s*");

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
    while (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        _messages = _messages + std::string(buffer);
        handleMessage();

        bytesRead = read(clientSocket, buffer, 255);
    }
    if (bytesRead < 0) {
        perror("Error reading from socket");
    }
    close(clientSocket);
}

void NetworkServer::handleMessage() {
    size_t index = _messages.find('\n');
    while(index != std::string::npos) {
        std::string message = _messages.substr(0, index);
        std::cout << message << std::endl;
        _messages = _messages.substr(index + 1);
        index = _messages.find('\n');

        std::smatch match;
        if (regex_match(message, REGEX_CLEAR)) {
            _ledMatrixDisplay.Clear();
        } else if (regex_match(message, REGEX_SHOW)) {
            _ledMatrixDisplay.Show();
        } else if (regex_match(message, match, REGEX_COLOR)) {
            int r = atoi(match[1].str().c_str());
            int g = atoi(match[2].str().c_str());
            int b = atoi(match[3].str().c_str());

            _ledMatrixDisplay.SetColor(r, g, b);
        } else if (regex_match(message, match, REGEX_PIXEL)) {
            int x = atoi(match[1].str().c_str());
            int y = atoi(match[2].str().c_str());

            _ledMatrixDisplay.DrawPixel(x, y);
        } else if (regex_match(message, match, REGEX_RECTANGLE)) {
            int x = atoi(match[1].str().c_str());
            int y = atoi(match[2].str().c_str());
            int width = atoi(match[3].str().c_str());
            int height = atoi(match[4].str().c_str());

            _ledMatrixDisplay.DrawRectangle(x, y, width, height);
        } else if (regex_match(message, match, REGEX_DIGIT)) {
            int position = atoi(match[1].str().c_str());
            int digit = atoi(match[2].str().c_str());

            _ledMatrixDisplay.DrawDigit(position, digit);
        }  else if (regex_match(message, match, REGEX_TEXT)) {
            int x = atoi(match[1].str().c_str());
            int y = atoi(match[2].str().c_str());
            std::string text = match[3].str();

            _ledMatrixDisplay.DrawSmallText(x, y, text);
        } else {
            std::cerr << "Invalid message **" << message << "**" << std::endl;
        }
    }
}
