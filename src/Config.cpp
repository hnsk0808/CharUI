#include <CharUI/Config.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>

cui::Color defaultColor(255, 255, 255, 0, 0, 0);
char paddingChar = ' ';

void cui::init()
{
    signal(SIGINT, [](int s) {
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

cui::Color cui::getDefaultColor()
{
    return defaultColor;
}

void cui::setDefaultColor(Color newColor)
{
    defaultColor = newColor;
}
