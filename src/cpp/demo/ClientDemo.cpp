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
    commands.emplace_back(CommandPtr(new ClearCommand()));
    commands.emplace_back(CommandPtr(new ColorCommand(128, 128, 128)));
    commands.emplace_back(CommandPtr(new PixelCommand(12, 12)));
    commands.emplace_back(CommandPtr(new SmallTextCommand(5, 25, "test")));
    commands.emplace_back(CommandPtr(new ImageCommand(5, 5, "test")));
    commands.emplace_back(CommandPtr(new ShowCommand()));
    CommandPtr composite = CommandPtr(new CompositeCommand(commands));

    client
            .SendCommand(CommandPtr(new DefineImageCommand("test", imageBuilder.Build())))
            .SendCommand(CommandPtr(new DefineAnimationCommand("animation", composite)))
            .SendCommand(CommandPtr(new AnimationCommand("animation")));
    sleep(3);

    return 0;
}