#include <CharUI/Charui.h>
#include <cstdio>
#include <CharUI/Utils/Terminal.h>

cui::FeColor globalFeColor(0xFFFFFFFF);
cui::BkColor globalBkColor(0x00000000);
char globalPaddingChar = ' ';

void cui::hideCursor()
{
    printf("\x1B[?25l");
}

void cui::showCursor()
{
    printf("\x1B[?25h");
}

char cui::getGlobalPaddingChar()
{
    return globalPaddingChar;
}

void cui::setGlobalPaddingChar(char c)
{
    globalPaddingChar = c;
}

cui::FeColor cui::getGlobalFeColor()
{
    return globalFeColor;
}

void cui::setGlobalFeColor(FeColor newColor)
{
    globalFeColor = newColor;
}

cui::BkColor cui::getGlobalBkColor()
{
    return globalBkColor;
}

void cui::setGlobalBkColor(BkColor newColor)
{
    globalBkColor = newColor;
}

void cui::printComponent(const Component& component)
{
    auto& charBuffer = component.getCharBuffer();
    auto& feColorBuffer = component.getFeColorBuffer();
    auto& bkColorBuffer = component.getBkColorBuffer();
    for (size_t i = 0; i < component.getHeight(); ++i) {
        printf("%s", applyColors(charBuffer[i], feColorBuffer[i], bkColorBuffer[i]).data());
    }
}
