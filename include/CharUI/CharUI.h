#ifndef CHARUI_H
#define CHARUI_H

#define CHARUI_MAJOR_VERSION 0
#define CHARUI_MINOR_VERSION 0
#define CHARUI_PATCH_VERSION 3

#include "Component/Canvas.h"
#include "Component/Image.h"
#include "Component/Text.h"
#include <map>
#include <memory>

namespace cui
{

bool getCursorVisible();
void setCursorVisible(bool visible);

void moveCursorToBeginning();

void clearTerminal();
void getTerminalSize(uint32_t* width, uint32_t* height);

char getGlobalPaddingChar();
void setGlobalPaddingChar(char c);

Color getGlobalFeColor();
void setGlobalFeColor(Color newColor);

Color getGlobalBkColor();
void setGlobalBkColor(Color newColor);

void print(const std::vector<String>& charBuf, const ColorBuffer& feColorBuf, const ColorBuffer& bkColorBuf);

}

#endif