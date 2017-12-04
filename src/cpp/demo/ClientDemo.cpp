#include "../network/NetworkClient.h"

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
    boost::asio::io_service io_service;
    Client client(io_service, "localhost", 1236);

    ImageBuilder imageBuilder(2, 2);
    imageBuilder.SetPixel(0, 0, 1, 2, 3);
    imageBuilder.SetPixel(0, 1, 4, 5, 6);
    imageBuilder.SetPixel(1, 0, 7, 8, 9);
    imageBuilder.SetPixel(1, 1, 10, 11, 12);

    client
            .DefineImage("test", imageBuilder.Build())
            .Clear()
            .SetColor(128, 128, 128)
            .DrawPixel(12, 12)
            .DrawImage(5, 5, "test")
            .Show();

    sleep(8);

    return 0;
}