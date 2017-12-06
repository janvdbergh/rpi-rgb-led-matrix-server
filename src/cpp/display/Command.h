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
    DRAW_IMAGE
};

class Command {
public:
    virtual uint16_t GetCode() const =0;

    virtual void visit(Display &display) const =0;
};

class ClearCommand : public Command {
public:
    ClearCommand() = default;

    uint16_t GetCode() const override {
        return CommandCode::CLEAR;
    }

    void visit(Display &display) const override;
};

class ShowCommand : public Command {
public:
    ShowCommand() = default;

    uint16_t GetCode() const override {
        return CommandCode::SHOW;
    }

    void visit(Display &display) const override;
};

class ColorCommand : public Command {
public:
    ColorCommand(uint8_t red, uint8_t green, uint8_t blue) : _red(red), _green(green), _blue(blue) {}

    uint16_t GetCode() const override {
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

    void visit(Display &display) const override;


private:
    uint8_t _red, _green, _blue;
};

class PixelCommand : public Command {
public:
    PixelCommand(int16_t x, int16_t y) : _x(x), _y(y) {}

    uint16_t GetCode() const override {
        return CommandCode::PIXEL;
    }

    int16_t GetX() const {
        return _x;
    }

    int16_t GetY() const {
        return _y;
    }

    void visit(Display &display) const override;

private:
    int16_t _x, _y;
};

class RectangleCommand : public Command {
public:
    RectangleCommand(int16_t x, int16_t y, uint16_t width, uint16_t height) : _x(x), _y(y), _width(width),
                                                                              _height(height) {}

    uint16_t GetCode() const override {
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

    void visit(Display &display) const override;

private:
    int16_t _x, _y;
    uint16_t _width, _height;
};

class DigitCommand : public Command {
public:
    DigitCommand(uint8_t position, uint8_t digit) : _position(position), _digit(digit) {}

    uint16_t GetCode() const override {
        return CommandCode::DIGIT;
    }

    uint8_t GetPosition() const {
        return _position;
    }

    uint8_t GetDigit() const {
        return _digit;
    }

    void visit(Display &display) const override;

private:
    uint8_t _position, _digit;
};

class SmallTextCommand : public Command {
public:
    SmallTextCommand(int16_t x, int16_t y, std::string text) : _x(x), _y(y), _text(std::move(text)) {}

    uint16_t GetCode() const override {
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

    void visit(Display &display) const override;

private:
    int16_t _x, _y;
    std::string _text;
};

class LargeTextCommand : public Command {
public:
    LargeTextCommand(int16_t x, int16_t y, std::string text) : _x(x), _y(y), _text(std::move(text)) {}

    uint16_t GetCode() const override {
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

    void visit(Display &display) const override;

private:
    int16_t _x, _y;
    std::string _text;
};

class DefineImageCommand : public Command {
public:
    DefineImageCommand(std::string name, boost::shared_ptr<const Image> image) : name(std::move(name)),
                                                                                 _image(std::move(image)) {}

    uint16_t GetCode() const override {
        return CommandCode::DEFINE_IMAGE;
    }

    const std::string &GetName() const {
        return name;
    }

    const boost::shared_ptr<const Image> &GetImage() const {
        return _image;
    }

    void visit(Display &display) const override;

private:
    std::string name;
    boost::shared_ptr<const Image> _image;
};

class ImageCommand : public Command {
public:
    ImageCommand(int16_t x, int16_t y, std::string name) : _x(x), _y(y), _name(std::move(name)) {}

    uint16_t GetCode() const override {
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

    void visit(Display &display) const override;

private:
    int16_t _x, _y;
    std::string _name;
};

#endif //DISPLAYSERVER_COMMAND_H
