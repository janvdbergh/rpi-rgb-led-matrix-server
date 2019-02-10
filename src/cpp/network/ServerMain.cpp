#include "NetworkServer.h"

class DummyPacketHandler: public PacketHandler {
public:
	const NetworkPacketPtr handle_packet(NetworkPacketPtr request) override {
		return request;
	}
};

int main(int argc, char *argv[]) {
	DummyPacketHandler packetHandler;
	NetworkServer networkServer(1236, packetHandler);
	networkServer.StartServerAndBlock();

	return 0;
}
