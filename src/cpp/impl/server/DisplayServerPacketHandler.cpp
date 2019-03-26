#include <server/DisplayServerPacketHandler.h>

const NetworkPacketPtr DisplayServerPacketHandler::handle_packet(NetworkPacketPtr request) {
	ServerCommandPtr command = _commandReader.read_command(request);
	command->execute(_executionContext);

	return NetworkPacketPtr(new NetworkPacket());
}
