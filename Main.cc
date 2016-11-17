#include <iostream>
#include <unistd.h>

#include "LedMatrixDisplay.h"

int main(int argc, char* argv[]) {
    LedMatrixDisplay ledMatrixDisplay;
    if (!ledMatrixDisplay.Initialize(argc, argv)) {
        return 1;
    }

    ledMatrixDisplay.Clear();
    ledMatrixDisplay.DrawDigit(0, 0);
    ledMatrixDisplay.DrawDigit(1, 8);
    ledMatrixDisplay.DrawDigit(2, 1);
    ledMatrixDisplay.DrawDigit(3, 1);
    ledMatrixDisplay.Show();

    sleep(2);

    return 0;
}
