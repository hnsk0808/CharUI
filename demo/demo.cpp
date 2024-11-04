#include <CharUI/CharUI.h>
#include <CharUI/Utils/Terminal.h>
#include <thread>
#include <functional>
#include <fstream>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

using namespace std::literals;

stbtt_fontinfo font;
static void initFont()
{
    std::ifstream file("../../asserts/simhei.ttf", std::ios::binary);
    assert(file);
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    unsigned char* ttf_buffer = new unsigned char[size];
    file.read((char*)ttf_buffer, size);
    stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
}

static std::vector<cui::String> getCharImage(char c) 
{
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

class RainbowText : public cui::Text
{
public:
    static constexpr int rainbowRad[] = { 255, 255, 255, 0, 0, 0, 238 };
    static constexpr int rainbowGreen[] = { 0, 165, 255, 255, 255, 0, 130 };
    static constexpr int rainbowBlue[] = { 0, 0, 0, 0, 255, 255, 238 };

public:
    RainbowText(cui::BytesView bytesView)
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

class FPS : public cui::Text
{
public:
    FPS(cui::OnUpdate& onUpdate)
        : Text("FPS: 0")
    {
        onUpdate.connect(this, &FPS::update);
        lastTime = std::chrono::high_resolution_clock::now();
    }

    int getFPS() const
    {
        return fps;
    }

private:
    std::chrono::high_resolution_clock::time_point lastTime;
    int fps = 0;
    int frameCount = 0;
    void update()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        frameCount++;
        if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count() >= 1) {
            get()[0] = cui::String(cui::Bytes("FPS: ") + std::to_string(frameCount));
            fps = frameCount;
            frameCount = 0;
            lastTime = currentTime;
        }
    }
};

int main() 
{
    cui::init();
    initFont();
    cui::Page page;

    auto progressBar1 = cui::progressBar(15);
    auto progressBar2 = cui::progressBar(20);
    auto progressBar3 = cui::progressBar(25);
    auto rainbowText = std::make_shared<RainbowText>("CharUI支持彩色字符");
    auto fps = std::make_shared<FPS>(page.onUpdate);
    auto apple = cui::image("../../asserts/textures/apple.png");

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
            fps
        ));
    grid->set(1, 0,
        cui::vBox(
            cui::text("CharUI是跨平台的控制台UI库\nCharUI支持UTF8字符😊"),
            rainbowText
        ));

    page.set(0, 0, 0, grid);
    page.set(grid->get(0, 0)->getWidth() + grid->get(1, 0)->getWidth(), 0, 0, apple);

    page.onUpdate.connect(rainbowText, &RainbowText::update);
    page.onUpdate.connect([&]() { if (progressBar1->isDone()) { progressBar1->set(0); } else { progressBar1->set(progressBar1->get() + 10); } });
    page.onUpdate.connect([&]() { if (progressBar2->isDone()) { progressBar2->set(0); } else { progressBar2->set(progressBar2->get() + 10); } });
    page.onUpdate.connect([&]() { if (progressBar3->isDone()) { progressBar3->set(0); } else { progressBar3->set(progressBar3->get() + 10); } });
    page.onUpdate.connect([&]() {
        static int32_t offset = 0;
        page.erase(grid->get(0, 0)->getWidth() + grid->get(1, 0)->getWidth(), offset, 0);
        page.set(grid->get(0, 0)->getWidth() + grid->get(1, 0)->getWidth(), ++offset, 0, apple);
        if (offset == 30) {
            page.erase(grid->get(0, 0)->getWidth() + grid->get(1, 0)->getWidth(), offset, 0);
            offset = -30;
        }
        });

    while (true) {
        page.update();
        page.display();
        std::this_thread::sleep_for(100ms);
    }
    return 0;
}