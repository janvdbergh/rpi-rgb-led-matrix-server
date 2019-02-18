#include <server/NetworkServer.h>
#include <server/LedMatrixDisplay.h>
#include <server/DisplayServerPacketHandler.h>

int main(int argc, char *argv[]) {
	LedMatrixDisplay ledMatrixDisplay;
	ledMatrixDisplay.initialize(argc, argv);

	DisplayServerPacketHandler displayServerPacketHandler(ledMatrixDisplay);
	NetworkServer networkServer(1236, displayServerPacketHandler);
	networkServer.StartServerAndBlock();

	return 0;
}
