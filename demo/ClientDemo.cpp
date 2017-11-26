#include "../src/NetworkClient.h"

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
    boost::asio::io_service io_service;
    Client client(io_service, "localhost", 1236);

    client
            .Clear()
            .SetColor(128, 128, 128)
            .DrawPixel(12, 12)
            .Show();

    sleep(8);

    return 0;
}