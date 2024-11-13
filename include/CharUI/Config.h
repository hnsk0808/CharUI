#pragma once
#include "Utils/Color.h"

namespace cui
{

void init();

char getPaddingChar();
void setPaddingChar(char c);

FeColor getDefaultFeColor();
void getDefaultFeColor(FeColor newColor);

BkColor getDefaultBkColor();
void setDefaultBkColor(BkColor newColor);

}