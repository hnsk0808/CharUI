#include <CharUI/CharUI.h>
#include <csignal>
#include <functional>
#include <fstream>
#include <thread>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

using namespace std::literals;

static std::vector<uint8_t> readFile(std::string_view filepath) 
{
    std::ifstream file(filepath.data(), std::ios::binary);
    assert(file);
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    return buffer;
}

std::vector<uint8_t> ttf_buffer;
stbtt_fontinfo font;
static void initFont(std::string_view fontPath)
{
    ttf_buffer = readFile(fontPath);
    stbtt_InitFont(&font, ttf_buffer.data(), stbtt_GetFontOffsetForIndex(ttf_buffer.data(), 0));
}

static std::vector<cui::String> getCharImage(char c) 
{
    int w = 0, h = 0;
    uint8_t* bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, 20.0f), c, &w, &h, 0, 0);
    std::vector<cui::String> image;
    for (int j = 0; j < h; ++j) {
        cui::Bytes bytes = "";
        for (int i = 0; i < w; ++i) {
            bytes += " .:ioVM@"[bitmap[j * w + i] >> 5];
        }
        image.push_back(cui::String(bytes.data()));
    }
    return image;
}

int main()
{
    signal(SIGINT, [](int) noexcept {
        cui::setCursorVisible(true);
        exit(0);
        });
    cui::setCursorVisible(true);
    cui::setGlobalPaddingChar('.');
    cui::setGlobalFeColor(0x222222aa);
    cui::setGlobalBkColor(0xaaddddaa);
    initFont("../../asserts/simhei.ttf");

    cui::Canvas canvas(0, 0);
    cui::Image bkImage;
    cui::Image diamondSword("../../asserts/textures/diamond_sword.png");
    cui::Image apple("../../asserts/textures/apple.png");
    int32_t appleY = 0;
    cui::Text fpsText("FPS: 0");

    int fps = 0, frameCount = 0;
    std::chrono::high_resolution_clock::time_point lastTime;
    while (true) {
        // Deal Terminal Resize
        {
            uint32_t newWidth = 0, newHeight = 0;
            cui::getTerminalSize(&newWidth, &newHeight);
            if (canvas.getWidth() != newWidth || canvas.getHeight() != newHeight) {
                bkImage.set("../../asserts/textures/bk.jpeg", static_cast<int32_t>(std::ceil(static_cast<double>(newWidth) / 2.0)), newHeight);
                for (auto& line : bkImage.getColorBuffer()) {
                    for (auto& color : line) {
                        color -= 0x00000088;
                    }
                }
                canvas.resize(newWidth, newHeight);
                cui::clearTerminal();
            }
        }
        
        // Update FPS
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            ++frameCount;
            if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count() >= 1) {
                fpsText = cui::String(cui::Bytes("FPS: ") + std::to_string(frameCount));
                fps = frameCount;
                frameCount = 0;
                lastTime = currentTime;
            }
        }

        // Update Apple Position
        if (++appleY == 30) {
            appleY = -30;
        }
        
        // Update Canvas
        canvas.clear();
        {
            // Background image
            canvas.setBkColorBuffer(0, 0, bkImage.getColorBuffer());
        }
        {
            // Apple image
            canvas.setBkColorBuffer(70, appleY, apple.getColorBuffer());
        }
        {
            // Banner
            int32_t w = 0;
            canvas.setBkColorBuffer(w, 0, diamondSword.getColorBuffer());
            w += diamondSword.getWidth() + 3;
            canvas.setCharBuffer(w, 0, getCharImage('C'));
            w += 14;
            canvas.setCharBuffer(w, 0, getCharImage('U'));
            w += 14;
            canvas.setCharBuffer(w, 0, getCharImage('I'));
            w += 14;
            canvas.setCharBuffer(w, 0, cui::Text("CharUI是跨平台的控制台UI库😊\nCharUI支持UTF8字符😊").getCharBuffer());
        }
        {
            // FPS
            canvas.setCharBuffer(0, 16, fpsText.getCharBuffer());
        }

        // Print Canvas
        cui::moveCursorToBeginning();
        cui::print(canvas.getCharBuffer(), canvas.getFeColorBuffer(), canvas.getBkColorBuffer());
       
        //std::this_thread::sleep_for(100ms);
    }
    return 0;
}