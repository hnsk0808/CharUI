#include <CharUI/CharUI.h>
#include <CharUI/Utils/Terminal.h>
#include <thread>
#include <functional>
#include <fstream>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

using namespace std::literals;

stbtt_fontinfo font;
static std::vector<cui::String> getCharImage(char c) {
    int w = 0, h = 0;
    unsigned char* bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, 20.0f), c, &w, &h, 0, 0);
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

static void initFont() {
    std::ifstream file("../../asserts/simhei.ttf", std::ios::binary);
    assert(file);
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    unsigned char* ttf_buffer = new unsigned char[size];
    file.read((char*)ttf_buffer, size);
    stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
}

class RainbowText : public cui::Text
{
public:
    static constexpr int rainbowRad[] = { 255, 255, 255, 0, 0, 0, 238 };
    static constexpr int rainbowGreen[] = { 0, 165, 255, 255, 255, 0, 130 };
    static constexpr int rainbowBlue[] = { 0, 0, 0, 0, 255, 255, 238 };

public:
    RainbowText(std::string_view bytesView)
        : Text(bytesView)
    {
        get().resize(1);
        update();
    }

    void update()
    {
        for (int i = 0; i < get()[0].getVCount(); ++i) {
            int j = (i + rainbowOffset) % 7;
            get()[0].setRGB(i, rainbowRad[j], rainbowGreen[j], rainbowBlue[j]);
        }
        ++rainbowOffset;
    }

private:
    size_t rainbowOffset = 0;
};

int main() {
    cui::init();
    initFont();

    auto progressBar1 = cui::progressBar(15);
    auto progressBar2 = cui::progressBar(20);
    auto progressBar3 = cui::progressBar(25);
    auto rainbowText = std::make_shared<RainbowText>("CharUI支持彩色字符");
    auto fpsText = cui::text("FPS: 0");

    auto grid = cui::grid();
    grid->set(0, 0,
        cui::vBox(
            cui::hBox(
                cui::image("../../asserts/textures/diamond_sword.png"),
                cui::text("   "),
                cui::text(getCharImage('C')),
                cui::text("   "),
                cui::text(getCharImage('U')),
                cui::text("   "),
                cui::text(getCharImage('I')),
                cui::text("   ")
            ),
            cui::vBox(
                progressBar1,
                progressBar2,
                progressBar3
            ),
            fpsText
        ));
    grid->set(0, 1,
        cui::vBox(
            cui::text("CharUI是跨平台的控制台UI库\nCharUI支持UTF8字符😊"),
            rainbowText,
            cui::image("../../asserts/textures/apple.png")
        ));

    cui::Page page;
    page.set(0, 0, grid);
    page.onUpdate.connect([&]() { if (progressBar1->isDone()) { progressBar1->set(0); } else { progressBar1->set(progressBar1->get() + 10); } });
    page.onUpdate.connect([&]() { if (progressBar2->isDone()) { progressBar2->set(0); } else { progressBar2->set(progressBar2->get() + 10); } });
    page.onUpdate.connect([&]() { if (progressBar3->isDone()) { progressBar3->set(0); } else { progressBar3->set(progressBar3->get() + 10); } });
    page.onUpdate.connect(rainbowText, &RainbowText::update);

    auto lastTime = std::chrono::high_resolution_clock::now();
    int fps = 0;
    int frameCount = 0;
    page.onUpdate.connect([&]() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        frameCount++;
        if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count() >= 1) {
            fps = frameCount; 
            frameCount = 0; 
            lastTime = currentTime;
            fpsText->set(cui::Bytes("FPS: ") + std::to_string(fps));
        }
        });

    while (true) {
        page.update();
    }
    return 0;
}