#include <CharUI/Config.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>

cui::FeColor defaultFeColor(0xFFFFFFFF);
cui::BkColor defaultBkColor(0x00000000);
char paddingChar = ' ';

void cui::init()
{
    signal(SIGINT, [](int) noexcept {
        printf("\x1B[?25h");
        exit(0);
        });
    printf("\x1B[?25l");
}

char cui::getPaddingChar()
{
    return paddingChar;
}

void cui::setPaddingChar(char c)
{
    paddingChar = c;
}

cui::FeColor cui::getDefaultFeColor()
{
    return defaultFeColor;
}

void cui::setDefaultFeColor(FeColor newColor)
{
    defaultFeColor = newColor;
}

cui::BkColor cui::getDefaultBkColor()
{
    return defaultBkColor;
}

void cui::setDefaultBkColor(BkColor newColor)
{
    defaultBkColor = newColor;
}
