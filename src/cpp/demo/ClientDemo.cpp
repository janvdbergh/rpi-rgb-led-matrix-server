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

    CommandVector commands;
    commands.emplace_back(CommandFactory::CreateClearCommand());
    commands.emplace_back(CommandFactory::CreateColorCommand(128, 128, 128));
    commands.emplace_back(CommandFactory::CreatePixelCommand(12, 12));
    commands.emplace_back(CommandFactory::CreateImageCommand(5, 5, "test"));
    commands.emplace_back(CommandFactory::CreateShowCommand());
    CommandPtr composite = CommandFactory::CreateCompositeCommand(commands);

    client
            .SendCommand(CommandFactory::CreateDefineImageCommand("test", imageBuilder.Build()))
            .SendCommand(CommandFactory::CreateDefineAnimationCommand("animation", composite));
    sleep(3);

    return 0;
}