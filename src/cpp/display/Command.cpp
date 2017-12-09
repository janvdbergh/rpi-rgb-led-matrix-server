//
// Created by Jan Van den Bergh on 06/12/2017.
//

#include "Display.h"

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

void DefineImageCommand::Visit(Display &display) const {
    display.DefineImage(GetName(), GetImage());
}

void ImageCommand::Visit(Display &display) const {
    display.DrawImage(GetX(), GetY(), GetName());
}

void CompositeCommand::Visit(Display &display) const {
    for(const auto &it: GetCommands()) {
        it->Visit(display);
    }
}

void DefineAnimationCommand::Visit(Display &display) const {
    display.DefineAnimation(_name, _command);
}
