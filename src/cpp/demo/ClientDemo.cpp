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

    std::vector<boost::shared_ptr<const Command>> commands;
    commands.push_back(boost::shared_ptr<const Command>(new ClearCommand));
    commands.push_back(boost::shared_ptr<const Command>(new ColorCommand(128, 128, 128)));
    commands.push_back(boost::shared_ptr<const Command>(new PixelCommand(12, 12)));
    commands.push_back(boost::shared_ptr<const Command>(new ImageCommand(5, 5, "test")));
    commands.push_back(boost::shared_ptr<const Command>(new ShowCommand()));

    client
            .SendCommand(boost::shared_ptr<const Command>(new DefineImageCommand("test", imageBuilder.Build())))
            .SendCommand(boost::shared_ptr<const Command>(new DefineAnimationCommand("animation", commands)));

    sleep(3);

    return 0;
}