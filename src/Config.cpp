#include <CharUI/Config.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>

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
