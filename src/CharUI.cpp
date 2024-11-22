#include <CharUI/Charui.h>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>

cui::Color globalFeColor(0xFFFFFFFF);
cui::Color globalBkColor(0x00000000);
char globalPaddingChar = ' ';
bool globalCursorVisible = true;

bool cui::getCursorVisible()
{
    return globalCursorVisible;
}

void cui::setCursorVisible(bool visible)
{
    globalCursorVisible = visible;
    if (globalCursorVisible) {
        printf("\x1B[?25h");
    }
    else {
        printf("\x1B[?25l");
    }
}

void cui::moveCursorToBeginning()
{
    printf("\x1B[1;1H");
}

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>

void cui::getTerminalSize(uint32_t* width, uint32_t* height)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        *width = static_cast<uint32_t>(csbi.srWindow.Right - csbi.srWindow.Left + 1);
        *height = static_cast<uint32_t>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
    }
}

void cui::clearTerminal()
{
    system("cls");
}
#else
#include <sys/ioctl.h>
#include <unistd.h>

void cui::getTerminalSize(uint32_t* width, uint32_t* height)
{
    winsize w{};
    const int32_t status = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *width = w.ws_col;
    *height = w.ws_row;
}

void cui::clearTerminal()
{
    system("clear");
}
#endif

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
