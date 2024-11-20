#include <CharUI/Charui.h>
#include <CharUI/Utils/Terminal.h>
#include <cstdio>

cui::Color globalFeColor(0xFFFFFFFF);
cui::Color globalBkColor(0x00000000);
char globalPaddingChar = ' ';

void cui::hideCursor()
{
    printf("\x1B[?25l");
}

void cui::showCursor()
{
    printf("\x1B[?25h");
}

void cui::moveCursorToBeginning()
{
    printf("\x1B[1;1H");
}

char cui::getGlobalPaddingChar()
{
    return globalPaddingChar;
}

void cui::setGlobalPaddingChar(char c)
{
    globalPaddingChar = c;
}

cui::Color cui::getGlobalFeColor()
{
    return globalFeColor;
}

void cui::setGlobalFeColor(Color newColor)
{
    globalFeColor = newColor;
}

cui::Color cui::getGlobalBkColor()
{
    return globalBkColor;
}

void cui::setGlobalBkColor(Color newColor)
{
    globalBkColor = newColor;
}

void cui::print(const std::vector<String>& charBuf, const ColorBuffer& feColorBuf, const ColorBuffer& bkColorBuf)
{
    for (size_t i = 0; i < charBuf.size(); ++i) {
        printf(applyColors(charBuf[i], feColorBuf[i], bkColorBuf[i]).data());
    }
}
