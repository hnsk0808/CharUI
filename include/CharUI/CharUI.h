#ifndef CHARUI_H
#define CHARUI_H

#define CHARUI_MAJOR_VERSION 0
#define CHARUI_MINOR_VERSION 0
#define CHARUI_PATCH_VERSION 2

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

char getGlobalPaddingChar();
void setGlobalPaddingChar(char c);

FeColor getGlobalFeColor();
void setGlobalFeColor(FeColor newColor);

BkColor getGlobalBkColor();
void setGlobalBkColor(BkColor newColor);

void printComponent(const Component& component);

template<class...T>
auto canvas(T&&...t) { return std::make_shared<Canvas>(std::forward<T>(t)...); }

template<class...T>
auto image(T&&...t) { return std::make_shared<Image>(std::forward<T>(t)...); }

template<class...T>
auto text(T&&...t) { return std::make_shared<Text>(std::forward<T>(t)...); }

}

#endif