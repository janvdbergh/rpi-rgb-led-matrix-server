//
// Created by Jan Van den Bergh on 06/12/2017.
//

#include "Display.h"

void ClearCommand::visit(Display &display) const {
    display.Clear();
}

void ShowCommand::visit(Display &display) const {
    display.Show();
}

void ColorCommand::visit(Display &display) const {
    display.SetColor(GetRed(), GetGreen(), GetBlue());
}

void PixelCommand::visit(Display &display) const {
    display.DrawPixel(GetX(), GetY());
}

void RectangleCommand::visit(Display &display) const {
    display.DrawRectangle(GetX(), GetY(), GetWidth(), GetHeight());
}

void DigitCommand::visit(Display &display) const {
    display.DrawDigit(GetPosition(), GetDigit());
}

void SmallTextCommand::visit(Display &display) const {
    display.DrawSmallText(GetX(), GetY(), GetText());
}

void LargeTextCommand::visit(Display &display) const {
    display.DrawLargeText(GetX(), GetY(), GetText());
}

void DefineImageCommand::visit(Display &display) const {
    display.DefineImage(GetName(), GetImage());
}

void ImageCommand::visit(Display &display) const {
    display.DrawImage(GetX(), GetY(), GetName());
}
