//
// Created by Jan Van den Bergh on 06/12/2017.
//

#ifndef DISPLAYSERVER_COMMAND_H
#define DISPLAYSERVER_COMMAND_H


#include <string>
#include <utility>
#include "Image.h"

class Display;

enum CommandCode {
    CLEAR,
    SHOW,
    COLOR,
    PIXEL,
    RECTANGLE,
    DIGIT,
    SMALL_TEXT,
    LARGE_TEXT,
    DEFINE_IMAGE,
    DRAW_IMAGE,
    COMPOSITE,
    DEFINE_ANIMATION
};

class Command {
public:
    virtual CommandCode GetCode() const =0;

    virtual void Visit(Display &display) const =0;
};

typedef boost::shared_ptr<const Command> CommandPtr;
typedef std::vector<CommandPtr> CommandVector;

class ClearCommand : public Command {
public:
    ClearCommand() = default;

    CommandCode GetCode() const override {
        return CommandCode::CLEAR;
    }

    void Visit(Display &display) const override;
};

class ShowCommand : public Command {
public:
    ShowCommand() = default;

    CommandCode GetCode() const override {
        return CommandCode::SHOW;
    }

    void Visit(Display &display) const override;
};

class ColorCommand : public Command {
public:
    ColorCommand(uint8_t red, uint8_t green, uint8_t blue) : _red(red), _green(green), _blue(blue) {}

    CommandCode GetCode() const override {
        return CommandCode::COLOR;
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

class PixelCommand : public Command {
public:
    PixelCommand(int16_t x, int16_t y) : _x(x), _y(y) {}

    CommandCode GetCode() const override {
        return CommandCode::PIXEL;
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

class RectangleCommand : public Command {
public:
    RectangleCommand(int16_t x, int16_t y, uint16_t width, uint16_t height) : _x(x), _y(y), _width(width),
                                                                              _height(height) {}

    CommandCode GetCode() const override {
        return CommandCode::RECTANGLE;
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

class DigitCommand : public Command {
public:
    DigitCommand(uint8_t position, uint8_t digit) : _position(position), _digit(digit) {}

    CommandCode GetCode() const override {
        return CommandCode::DIGIT;
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

class SmallTextCommand : public Command {
public:
    SmallTextCommand(int16_t x, int16_t y, std::string text) : _x(x), _y(y), _text(std::move(text)) {}

    CommandCode GetCode() const override {
        return CommandCode::SMALL_TEXT;
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

class LargeTextCommand : public Command {
public:
    LargeTextCommand(int16_t x, int16_t y, std::string text) : _x(x), _y(y), _text(std::move(text)) {}

    CommandCode GetCode() const override {
        return CommandCode::LARGE_TEXT;
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

class DefineImageCommand : public Command {
public:
    DefineImageCommand(std::string name, boost::shared_ptr<const Image> image) : name(std::move(name)),
                                                                                 _image(std::move(image)) {}

    CommandCode GetCode() const override {
        return CommandCode::DEFINE_IMAGE;
    }

    const std::string &GetName() const {
        return name;
    }

    const boost::shared_ptr<const Image> &GetImage() const {
        return _image;
    }

    void Visit(Display &display) const override;

private:
    std::string name;
    boost::shared_ptr<const Image> _image;
};

class ImageCommand : public Command {
public:
    ImageCommand(int16_t x, int16_t y, std::string name) : _x(x), _y(y), _name(std::move(name)) {}

    CommandCode GetCode() const override {
        return CommandCode::DRAW_IMAGE;
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

class CompositeCommand : public Command {
public:
    explicit CompositeCommand(const CommandVector &commands) : _commands(commands) {}

    CommandCode GetCode() const override {
        return CommandCode::COMPOSITE;
    }

    CommandVector GetCommands() const {
        return _commands;
    }

    CompositeCommand &operator<<(CommandPtr command) {
        _commands.push_back(command);
        return *this;
    }

    void Visit(Display &display) const override;

private:
    CommandVector _commands;
};

class DefineAnimationCommand : public Command {
public:
    DefineAnimationCommand(std::string name, CommandPtr command) : _name(std::move(name)), _command(std::move(command)) {}

    CommandCode GetCode() const override {
        return CommandCode::DEFINE_ANIMATION;
    }

    const std::string &GetName() const {
        return _name;
    }

    const CommandPtr &GetCommand() const {
        return _command;
    }

    void Visit(Display &display) const override;

private:
    std::string _name;
    CommandPtr _command;
};

class CommandFactory {
public:
    static CommandPtr CreateClearCommand() {
        return CommandPtr(new ClearCommand);
    }

    static CommandPtr CreateShowCommand() {
        return CommandPtr(new ShowCommand);
    }

    static CommandPtr CreateColorCommand(uint8_t red, uint8_t green, uint8_t blue) {
        return CommandPtr(new ColorCommand(red, green, blue));
    }

    static CommandPtr CreatePixelCommand(int16_t x, int16_t y) {
        return CommandPtr(new PixelCommand(x, y));
    }

    static CommandPtr CreateRectangleCommand(int16_t x, int16_t y, uint16_t width, uint16_t height) {
        return CommandPtr(new RectangleCommand(x, y, width, height));
    }

    static CommandPtr CreatePositionCommand(uint8_t position, uint8_t digit) {
        return CommandPtr(new DigitCommand(position, digit));
    }

    static CommandPtr CreateSmallTextCommand(int16_t x, int16_t y, std::string text) {
        return CommandPtr(new SmallTextCommand(x, y, text));
    }

    static CommandPtr CreateLargeTextCommand(int16_t x, int16_t y, std::string text) {
        return CommandPtr(new LargeTextCommand(x, y, text));
    }

    static CommandPtr CreateDefineImageCommand(const std::string &name, boost::shared_ptr<const Image> image) {
        return CommandPtr(new DefineImageCommand(name, image));
    }

    static CommandPtr CreateImageCommand(int16_t x, int16_t y, const std::string &name) {
        return CommandPtr(new ImageCommand(x, y, name));
    }

    static CommandPtr CreateCompositeCommand(CommandVector commands) {
        return CommandPtr(new CompositeCommand(commands));
    }

    static CommandPtr CreateDefineAnimationCommand(const std::string &name, CommandPtr command) {
        return CommandPtr(new DefineAnimationCommand(name, command));
    }
};

#endif //DISPLAYSERVER_COMMAND_H
