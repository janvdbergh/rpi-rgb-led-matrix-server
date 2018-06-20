#include "Display.h"

void BrightnessCommand::Visit(Display &display) const {
	display.SetBrightness(_brightness);
}

void ClearCommand::Visit(Display &display) const {
	display.Clear();
}

void ShowCommand::Visit(Display &display) const {
	display.Show();
}

void ColorCommand::Visit(Display &display) const {
	display.SetColor(GetRed(), GetGreen(), GetBlue());
}

void PixelCommand::Visit(Display &display) const {
	display.DrawPixel(GetX(), GetY());
}

void RectangleCommand::Visit(Display &display) const {
	display.DrawRectangle(GetX(), GetY(), GetWidth(), GetHeight());
}

void DigitCommand::Visit(Display &display) const {
	display.DrawDigit(GetPosition(), GetDigit());
}

void SmallTextCommand::Visit(Display &display) const {
	display.DrawSmallText(GetX(), GetY(), GetText());
}

void LargeTextCommand::Visit(Display &display) const {
	display.DrawLargeText(GetX(), GetY(), GetText());
}

void SleepCommand::Visit(Display &display) const {
	display.Sleep(GetMillis());
}

void ImageCommand::Visit(Display &display) const {
	display.DrawImage(GetX(), GetY(), GetName());
}

void AnimationCommand::Visit(Display &display) const {
	display.ShowAnimation(GetName());
}

void CompositeCommand::Visit(Display &display) const {
	for (const auto &it: GetCommands()) {
		it->Visit(display);
	}
}

void DefineImageCommand::Visit(Display &display) const {
	display.DefineImage(GetName(), GetImage());
}

void DefineAnimationCommand::Visit(Display &display) const {
	display.DefineAnimation(_name, _command);
}

void SetLayerCommand::Visit(Display &display) const {
	display.SetLayer(_layer);
}

void ClearLayerCommand::Visit(Display &display) const {
	display.ClearLayer();
}

void SetLayerAlphaCommand::Visit(Display &display) const {
	display.SetLayerAlpha(_alpha);
}
