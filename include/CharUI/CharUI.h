#ifndef CHARUI_H
#define CHARUI_H

#define CHARUI_MAJOR_VERSION 0
#define CHARUI_MINOR_VERSION 0
#define CHARUI_PATCH_VERSION 3

#include "Component/Canvas.h"
#include "Component/Image.h"
#include "Component/Text.h"
#include "Utils/Terminal.h"
#include <map>
#include <memory>

namespace cui
{

void hideCursor();
void showCursor();
void moveCursorToBeginning();

char getGlobalPaddingChar();
void setGlobalPaddingChar(char c);

FeColor getGlobalFeColor();
void setGlobalFeColor(FeColor newColor);

BkColor getGlobalBkColor();
void setGlobalBkColor(BkColor newColor);

void printComponent(const std::vector<String>& charBuf, const FeColorBuffer& feColorBuf, const BkColorBuffer& bkColorBuf);

}

#endif