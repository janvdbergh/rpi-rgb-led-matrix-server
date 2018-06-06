#ifndef DISPLAYSERVER_DISPLAY_H
#define DISPLAYSERVER_DISPLAY_H

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "Command.h"

class Display {
public:
	virtual ~Display() = default;

	virtual void Reset();

	void ExecuteCommand(const CommandPtr &command) {
		command->Visit(*this);
	}

	virtual bool Initialize(int argc, char **argv) =0;

	virtual void SetBrightness(uint8_t brightness) =0;

	virtual void SetColor(uint8_t r, uint8_t g, uint8_t b) =0;

	virtual void Clear() =0;

	virtual void DrawPixel(int16_t x, int16_t y) =0;

	virtual void DrawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height) =0;

	virtual void DrawDigit(uint8_t position, uint8_t digit) =0;

	virtual void DrawSmallText(int16_t x, int16_t y, std::string text) =0;

	virtual void DrawLargeText(int16_t x, int16_t y, std::string text) =0;

	virtual void DrawImage(int16_t x, int16_t y, const std::string &imageName) =0;

	virtual void ShowAnimation(const std::string &name);

	virtual void Sleep(uint16_t millis);

	virtual void SetLayer(uint8_t layer) =0;

	virtual void ClearLayer() =0;

	virtual void Show() =0;

	void DefineImage(const std::string &name, const ImagePtr &image);

	virtual void DefineAnimation(const std::string &name, const CommandPtr &command);

protected:
	const ImagePtr GetImage(const std::string &name) const;

	const CommandPtr GetAnimation(const std::string &name) const;

private:
	std::map<std::string, ImagePtr> _images;
	std::map<std::string, CommandPtr> _animations;
};

boost::shared_ptr<Display> CreateDisplay();

#endif //DISPLAYSERVER_DISPLAY_H
