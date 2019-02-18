#ifndef DISPLAYSERVER_DISPLAYSERVERPACKETHANDLER_H
#define DISPLAYSERVER_DISPLAYSERVERPACKETHANDLER_H

#include <server/PacketHandler.h>
#include <common/Command.h>
#include "ServerCommand.h"

class DisplayServerPacketHandler : public PacketHandler {
public:
	explicit DisplayServerPacketHandler(AbstractDisplay& display): _commandReader(std::shared_ptr<ServerCommandFactory>(new ServerCommandFactory())), _executionContext(display) {}

	const NetworkPacketPtr handle_packet(NetworkPacketPtr request) override;

private:
	CommandReader<ServerCommand> _commandReader;
	ExecutionContext _executionContext;
};


#endif //DISPLAYSERVER_DISPLAYSERVERPACKETHANDLER_H
