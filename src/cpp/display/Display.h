#ifndef DISPLAYSERVER_DISPLAY_H
#define DISPLAYSERVER_DISPLAY_H

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "Command.h"

class Display {
public:
    virtual ~Display() = default;

    void ExecuteCommand(const boost::shared_ptr<const Command> &command) {
        command->visit(*this);
    }

    virtual bool Initialize(int argc, char **argv) =0;

    virtual void SetColor(uint8_t r, uint8_t g, uint8_t b) =0;

    virtual void Clear() =0;

    virtual void DrawPixel(int16_t x, int16_t y) =0;

    virtual void DrawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height) =0;

    virtual void DrawDigit(uint8_t position, uint8_t digit) =0;

    virtual void DrawSmallText(int16_t x, int16_t y, std::string text) =0;

    virtual void DrawLargeText(int16_t x, int16_t y, std::string text) =0;

    void DefineImage(const std::string &name, const boost::shared_ptr<const Image> &image);

    virtual void DrawImage(int16_t x, int16_t y, const std::string &imageName) =0;

    virtual void Show() =0;

    virtual void DefineAnimation(const std::string &name, const std::vector<boost::shared_ptr<const Command>> &commands);

protected:
    boost::shared_ptr<const Image> GetImage(const std::string &name) const {
        return _images.at(name);
    }

private:
    std::map<std::string, boost::shared_ptr<const Image>> _images;
    std::map<std::string, std::vector<boost::shared_ptr<const Command>>> _commands;
};

boost::shared_ptr<Display> CreateDisplay();

#endif //DISPLAYSERVER_DISPLAY_H
