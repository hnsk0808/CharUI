#include <CharUI/Charui.h>
#include <CharUI/Utils/Terminal.h>
#include <cstdio>

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

void cui::printComponent(const std::vector<String>& charBuf, const FeColorBuffer& feColorBuf, const BkColorBuffer& bkColorBuf)
{
    for (size_t i = 0; i < charBuf.size(); ++i) {
        printf(applyColors(charBuf[i], feColorBuf[i], bkColorBuf[i]).data());
    }
}
