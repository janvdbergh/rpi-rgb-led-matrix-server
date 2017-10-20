#include <memory>
#include "led-matrix-display/Display.h"
#include "NetworkServer.h"

int main(int argc, char* argv[]) {
    std::unique_ptr<displays::Display> display(displays::createDisplay());
    if (!display->Initialize(argc, argv)) {
        return 1;
    }

    NetworkServer networkServer(display);
    networkServer.RunServer(1236);

    return 0;
}
