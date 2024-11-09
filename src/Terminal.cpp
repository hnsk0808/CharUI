#include <CharUI/Utils/Terminal.h>

#ifdef _WIN32
#define NOMINMAX
#include <cstdlib>
#include <windows.h>

void cui::terminalSize(int32_t* width, int32_t* height)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
}

void cui::terminalClear() {
    system("cls");
}

#else
#include <cstdlib>
#include <sys/ioctl.h>
#include <unistd.h>

void cui::terminalSize(int32_t* width, int32_t* height)
{
    winsize w{};
    const int32_t status = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *width = w.ws_col;
    *height = w.ws_row;
}

void cui::terminalClear() {
    system("clear");
}

#endif