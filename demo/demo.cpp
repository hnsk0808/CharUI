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

class FPS
{
public:
    FPS() : text("FPS: 0") {}
    const std::vector<cui::String>& getCharBuffer() const { return text.getCharBuffer(); }
    void update()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        ++frameCount;
        if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count() >= 1) {
            text[0] = cui::String(cui::Bytes("FPS: ") + std::to_string(frameCount));
            fps = frameCount;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

private:
    cui::Text text;
    std::chrono::high_resolution_clock::time_point lastTime;
    int fps = 0;
    int frameCount = 0;
};

int main()
{
    signal(SIGINT, [](int) noexcept {
        cui::showCursor();
        exit(0);
        });
    cui::hideCursor();
    cui::setGlobalPaddingChar('.');
    cui::setGlobalFeColor(0x222222aa);
    cui::setGlobalBkColor(0xaaddddaa);
    initFont("../../asserts/simhei.ttf");
    cui::Canvas canvas(0, 0);

    cui::Image bkImage;
    cui::Image diamondSword("../../asserts/textures/diamond_sword.png");
    cui::Image apple("../../asserts/textures/apple.png");
    int32_t appleOffset = 0;
    FPS fps;
    while (true) {
        int32_t newWidth = 0, newHeight = 0;
        cui::terminalSize(&newWidth, &newHeight);
        if (canvas.getWidth() != newWidth || canvas.getHeight() != newHeight) {
            bkImage.set("../../asserts/textures/bk.jpeg", static_cast<int32_t>(std::ceil(static_cast<double>(newWidth) / 2.0)), newHeight);
            for (auto& line : bkImage.get()) {
                for (auto& color : line) {
                    color.value -= 0x00000088;
                }
            }
            canvas.resize(newWidth, newHeight);
            cui::terminalClear();
        }

        fps.update();

        canvas.clear();
        {
            canvas.setBkColorBuffer(0, 0, bkImage.getBkColorBuffer());
        }
        {
            canvas.setBkColorBuffer(70, appleOffset, apple.getBkColorBuffer());
            if (++appleOffset == 30) {
                appleOffset = -30;
            }
        }
        {
            std::vector<std::shared_ptr<cui::Component>> hBox = {
                cui::image(diamondSword),
                cui::text(getCharImage('C')),
                cui::text(getCharImage('U')),
                cui::text(getCharImage('I')),
                cui::text("CharUI是跨平台的控制台UI库😊\nCharUI支持UTF8字符😊")
            };
            int32_t w = 0;
            for (auto&& c : hBox) {
                canvas.setCharBuffer(w, 0, c->getCharBuffer());
                canvas.setFeColorBuffer(w, 0, c->getFeColorBuffer());
                canvas.setBkColorBuffer(w, 0, c->getBkColorBuffer());
                w += c->getWidth() + 3;
            }
        }
        {
            canvas.setCharBuffer(0, 16, fps.getCharBuffer());
        }
        printf("\x1B[1;1H");
        cui::printComponent(canvas);
        // std::this_thread::sleep_for(100ms);
    }
    return 0;
}