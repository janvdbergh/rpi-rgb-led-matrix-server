#ifndef DISPLAYSERVER_COMMAND_H
#define DISPLAYSERVER_COMMAND_H


#include <string>
#include <utility>
#include "Image.h"

class Display;

enum OldCommandCode {
	SET_BRIGHTNESS,
	CLEAR,
	SHOW,
	COLOR,
	PIXEL,
	RECTANGLE,
	DIGIT,
	SMALL_TEXT,
	LARGE_TEXT,
	IMAGE,
	ANIMATION,
	SLEEP,
	COMPOSITE,
	DEFINE_IMAGE,
	DEFINE_ANIMATION,
	SET_LAYER,
	CLEAR_LAYER,
	SET_LAYER_ALPHA
};

class OldCommand {
public:
	virtual OldCommandCode GetCode() const =0;

	virtual void Visit(Display &display) const =0;
};

typedef boost::shared_ptr<const OldCommand> OldCommandPtr;
typedef std::vector<OldCommandPtr> OldCommandVector;

class BrightnessCommand : public OldCommand {
public:
	explicit BrightnessCommand(uint8_t brightness) : _brightness(brightness) {};

	OldCommandCode GetCode() const override {
		return OldCommandCode::SET_BRIGHTNESS;
	}

	uint8_t GetBrightness() const {
		return _brightness;
	}

	void Visit(Display &display) const override;

private:
	uint8_t _brightness;
};


class ClearCommand : public OldCommand {
public:
	ClearCommand() = default;

	OldCommandCode GetCode() const override {
		return OldCommandCode::CLEAR;
	}

	void Visit(Display &display) const override;
};

class ShowCommand : public OldCommand {
public:
	ShowCommand() = default;

	OldCommandCode GetCode() const override {
		return OldCommandCode::SHOW;
	}

	void Visit(Display &display) const override;
};

class ColorCommand : public OldCommand {
public:
	ColorCommand(uint8_t red, uint8_t green, uint8_t blue) : _red(red), _green(green), _blue(blue) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::COLOR;
	}

	uint8_t GetRed() const {
		return _red;
	}

	uint8_t GetGreen() const {
		return _green;
	}

	uint8_t GetBlue() const {
		return _blue;
	}

	void Visit(Display &display) const override;


private:
	uint8_t _red, _green, _blue;
};

class PixelCommand : public OldCommand {
public:
	PixelCommand(int16_t x, int16_t y) : _x(x), _y(y) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::PIXEL;
	}

	int16_t GetX() const {
		return _x;
	}

	int16_t GetY() const {
		return _y;
	}

	void Visit(Display &display) const override;

private:
	int16_t _x, _y;
};

class RectangleCommand : public OldCommand {
public:
	RectangleCommand(int16_t x, int16_t y, uint16_t width, uint16_t height) :
			_x(x), _y(y), _width(width), _height(height) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::RECTANGLE;
	}

	int16_t GetX() const {
		return _x;
	}

	int16_t GetY() const {
		return _y;
	}

	uint16_t GetWidth() const {
		return _width;
	}

	uint16_t GetHeight() const {
		return _height;
	}

	void Visit(Display &display) const override;

private:
	int16_t _x, _y;
	uint16_t _width, _height;
};

class DigitCommand : public OldCommand {
public:
	DigitCommand(uint8_t position, uint8_t digit) : _position(position), _digit(digit) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::DIGIT;
	}

	uint8_t GetPosition() const {
		return _position;
	}

	uint8_t GetDigit() const {
		return _digit;
	}

	void Visit(Display &display) const override;

private:
	uint8_t _position, _digit;
};

class SmallTextCommand : public OldCommand {
public:
	SmallTextCommand(int16_t x, int16_t y, std::string text) : _x(x), _y(y), _text(std::move(text)) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::SMALL_TEXT;
	}

	int16_t GetX() const {
		return _x;
	}

	int16_t GetY() const {
		return _y;
	}

	const std::string &GetText() const {
		return _text;
	}

	void Visit(Display &display) const override;

private:
	int16_t _x, _y;
	std::string _text;
};

class LargeTextCommand : public OldCommand {
public:
	LargeTextCommand(int16_t x, int16_t y, std::string text) : _x(x), _y(y), _text(std::move(text)) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::LARGE_TEXT;
	}

	int16_t GetX() const {
		return _x;
	}

	int16_t GetY() const {
		return _y;
	}

	const std::string &GetText() const {
		return _text;
	}

	void Visit(Display &display) const override;

private:
	int16_t _x, _y;
	std::string _text;
};

class ImageCommand : public OldCommand {
public:
	ImageCommand(int16_t x, int16_t y, std::string name) : _x(x), _y(y), _name(std::move(name)) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::IMAGE;
	}

	int16_t GetX() const {
		return _x;
	}

	int16_t GetY() const {
		return _y;
	}

	const std::string &GetName() const {
		return _name;
	}

	void Visit(Display &display) const override;

private:
	int16_t _x, _y;
	std::string _name;
};

class AnimationCommand : public OldCommand {
public:
	explicit AnimationCommand(std::string name) : _name(std::move(name)) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::ANIMATION;
	}

	const std::string &GetName() const {
		return _name;
	}

	void Visit(Display &display) const override;

private:
	std::string _name;
};

class SleepCommand : public OldCommand {
public:
	explicit SleepCommand(uint16_t millis) : _millis(millis) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::SLEEP;
	};

	uint16_t GetMillis() const {
		return _millis;
	}

	void Visit(Display &display) const override;

private:
	uint16_t _millis;
};

class CompositeCommand : public OldCommand {
public:
	explicit CompositeCommand(const OldCommandVector &commands) : _commands(commands) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::COMPOSITE;
	}

	OldCommandVector GetCommands() const {
		return _commands;
	}

	CompositeCommand &operator<<(OldCommandPtr command) {
		_commands.push_back(command);
		return *this;
	}

	void Visit(Display &display) const override;

private:
	OldCommandVector _commands;
};


class DefineImageCommand : public OldCommand {
public:
	DefineImageCommand(std::string name, ImagePtr image) : name(std::move(name)), _image(std::move(image)) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::DEFINE_IMAGE;
	}

	const std::string &GetName() const {
		return name;
	}

	const ImagePtr &GetImage() const {
		return _image;
	}

	void Visit(Display &display) const override;

private:
	std::string name;
	ImagePtr _image;
};

class DefineAnimationCommand : public OldCommand {
public:
	DefineAnimationCommand(std::string name, OldCommandPtr command) :
			_name(std::move(name)), _command(std::move(command)) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::DEFINE_ANIMATION;
	}

	const std::string &GetName() const {
		return _name;
	}

	const OldCommandPtr &GetCommand() const {
		return _command;
	}

	void Visit(Display &display) const override;

private:
	std::string _name;
	OldCommandPtr _command;
};

class SetLayerCommand : public OldCommand {
public:
	explicit SetLayerCommand(uint8_t layer) : _layer(layer) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::SET_LAYER;
	}

	uint8_t GetLayer() const {
		return _layer;
	}

	void Visit(Display &display) const override;

private:
	uint8_t _layer;
};

class ClearLayerCommand : public OldCommand {
public:
	explicit ClearLayerCommand() = default;

	OldCommandCode GetCode() const override {
		return OldCommandCode::CLEAR_LAYER;
	}

	void Visit(Display &display) const override;
};

class SetLayerAlphaCommand: public OldCommand {
public:
	explicit SetLayerAlphaCommand(uint8_t alpha): _alpha(alpha) {}

	OldCommandCode GetCode() const override {
		return OldCommandCode::SET_LAYER_ALPHA;
	}

	uint8_t GetAlpha() const {
		return _alpha;
	}

	void Visit(Display &display) const override;
private:
	uint8_t _alpha;
};

#endif //DISPLAYSERVER_COMMAND_H
