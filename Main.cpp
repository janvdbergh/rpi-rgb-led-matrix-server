#include "led-matrix-display/LedMatrixDisplay.h"
#include "NetworkServer.h"

int main(int argc, char* argv[]) {
    LedMatrixDisplay ledMatrixDisplay;
    if (!ledMatrixDisplay.Initialize(argc, argv)) {
        return 1;
    }

    NetworkServer networkServer(ledMatrixDisplay);
    networkServer.RunServer(1236);

    return 0;
}
