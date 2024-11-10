#pragma once
#include "Utils/Color.h"

namespace cui
{

void init();

char getPaddingChar();
void setPaddingChar(char c);

Color getDefaultColor();
void setDefaultColor(Color newColor);

}