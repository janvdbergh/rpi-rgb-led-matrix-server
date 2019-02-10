#include "ServerCommand.h"

void ServerResetCommand::execute(ExecutionContext &context) const {
	context.reset();
}

void ServerShowCommand::execute(ExecutionContext &context) const {
	context.show();
}

void ServerSetBrightnessCommand::execute(ExecutionContext &context) const {
	context.set_brighness(get_brightness());
}

void ServerCreateLayerCommand::execute(ExecutionContext &context) const {
	LayerPtr layer(new Layer(get_offset(), get_width(), get_height()));
	context.add_layer(get_index(), layer);
}

void ServerSelectLayerCommand::execute(ExecutionContext &context) const {
	context.select_layer(get_index());
}

void ServerDrawPixelCommand::execute(ExecutionContext &context) const {
	context.selected_layer()->set_pixel(get_location(), get_value());
}

void ServerDrawRectangleCommand::execute(ExecutionContext &context) const {
	context.selected_layer()->rectangle(get_location(), get_width(), get_height(), get_value());
}

void ServerDefineImageCommand::execute(ExecutionContext &context) const {
	context.define_image(get_name(), get_image());
}

void ServerDrawImageCommand::execute(ExecutionContext &context) const {
	context.draw_image(get_location(), get_name());
}

ServerCommandPtr ServerCommandFactory::create_reset_command() const {
	return ServerCommandPtr(new ServerResetCommand());
}

ServerCommandPtr ServerCommandFactory::create_show_command() const {
	return ServerCommandPtr(new ServerShowCommand());
}

ServerCommandPtr ServerCommandFactory::create_set_brightness_command(uint8_t brightness) const {
	return ServerCommandPtr(new ServerSetBrightnessCommand(brightness));
}

ServerCommandPtr ServerCommandFactory::create_create_layer_command(uint8_t index, Point offset, uint16_t width,
																   uint16_t height) const {
	return ServerCommandPtr(new ServerCreateLayerCommand(index, offset, width, height));
}

ServerCommandPtr ServerCommandFactory::create_select_layer_command(uint8_t index) const {
	return ServerCommandPtr(new ServerSelectLayerCommand(index));
}

ServerCommandPtr ServerCommandFactory::create_draw_pixel_command(Point offset, Pixel value) const {
	return ServerCommandPtr(new ServerDrawPixelCommand(offset, value));
}

ServerCommandPtr ServerCommandFactory::create_draw_rectangle_command(Point location, uint16_t width, uint16_t height, Pixel value) const {
	return ServerCommandPtr(new ServerDrawRectangleCommand(location, width, height, value));
}

ServerCommandPtr ServerCommandFactory::create_define_image_command(const std::string &name, ImagePtr image) const {
	return ServerCommandPtr(new ServerDefineImageCommand(name, image));
}

ServerCommandPtr ServerCommandFactory::create_draw_image_command(Point location, const std::string &name) const {
	return ServerCommandPtr(new ServerDrawImageCommand(location, name));
}
