#ifndef DISPLAYSERVER_SERVERCOMMAND_H
#define DISPLAYSERVER_SERVERCOMMAND_H

#include <cstdint>
#include <common/Command.h>
#include <server/ExecutionContext.h>

class ExecutionContext;

class ServerCommand : public Command {
public:
	virtual void execute(ExecutionContext &context) const = 0;

protected:
	ServerCommand() = default;
};

typedef std::shared_ptr<const ServerCommand> ServerCommandPtr;

class ServerResetCommand : public ServerCommand, public ResetCommand {
public:
	ServerResetCommand() : ResetCommand() {}

	void execute(ExecutionContext &context) const override;
};


class ServerShowCommand : public ServerCommand, public ShowCommand {
public:
	ServerShowCommand() : ShowCommand() {}

	void execute(ExecutionContext &context) const override;
};

class ServerSetBrightnessCommand : public ServerCommand, public SetBrightnessCommand {
public:
	explicit ServerSetBrightnessCommand(uint8_t brightness) : SetBrightnessCommand(brightness) {}

	void execute(ExecutionContext &context) const override;
};

class ServerCreateLayerCommand : public ServerCommand, public CreateLayerCommand {
public:
	ServerCreateLayerCommand(uint8_t index, Point offset, uint16_t width, uint16_t height) :
			CreateLayerCommand(index, offset, width, height) {}

	void execute(ExecutionContext &context) const override;
};

class ServerSelectLayerCommand : public ServerCommand, public SelectLayerCommand {
public:
	explicit ServerSelectLayerCommand(uint8_t index) : SelectLayerCommand(index) {}

	void execute(ExecutionContext &context) const override;
};

class ServerDrawPixelCommand : public ServerCommand, public DrawPixelCommand {
public:
	ServerDrawPixelCommand(Point location, Pixel value) :
			DrawPixelCommand(location, value) {}

	void execute(ExecutionContext &context) const override;
};

class ServerDrawRectangleCommand : public ServerCommand, DrawRectangleCommand {
public:
	ServerDrawRectangleCommand(Point location, uint16_t width, uint16_t height, Pixel value) :
			DrawRectangleCommand(location, width, height, value) {}

	void execute(ExecutionContext &context) const override;
};

class ServerDefineImageCommand : public ServerCommand, DefineImageCommand {
public:
	ServerDefineImageCommand(std::string name, ImagePtr image) : DefineImageCommand(std::move(name),
																					std::move(image)) {}

	void execute(ExecutionContext &context) const override;
};

class ServerDrawImageCommand : public ServerCommand, DrawImageCommand {
public:
	ServerDrawImageCommand(Point location, const std::string &name) : DrawImageCommand(location, name) {}

	void execute(ExecutionContext &context) const override;
};

class ServerCommandFactory : public CommandFactory<ServerCommand> {
public:
	ServerCommandPtr create_reset_command() const override;

	ServerCommandPtr create_show_command() const override;

	ServerCommandPtr create_set_brightness_command(uint8_t brightness) const override;

	ServerCommandPtr create_create_layer_command(uint8_t index, Point offset, uint16_t width, uint16_t height) const override;

	ServerCommandPtr create_select_layer_command(uint8_t index) const override;

	ServerCommandPtr create_draw_pixel_command(Point location, Pixel value) const override;

	ServerCommandPtr create_draw_rectangle_command(Point location, uint16_t width, uint16_t height, Pixel value) const override;

	ServerCommandPtr create_define_image_command(const std::string &name, ImagePtr image) const override;

	ServerCommandPtr create_draw_image_command(Point location, const std::string &name) const override;
};

#endif //DISPLAYSERVER_SERVERCOMMAND_H
