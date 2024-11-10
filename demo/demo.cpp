#include <CharUI/CharUI.h>
#include <CharUI/Utils/Terminal.h>
#include <thread>
#include <functional>
#include <fstream>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

using namespace std::literals;

static std::vector<uint8_t> readFile(std::string_view filepath) {
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
static void initFont(std::string_view fontPath) {
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

class FPS : public cui::Component
{
public:
    FPS(cui::OnUpdate& onUpdate)
        : text("FPS: 0")
    {
        onUpdate.connect(this, &FPS::update);
        lastTime = std::chrono::high_resolution_clock::now();
    }

    int32_t getWidth() const override { return text.getWidth(); }
    int32_t getHeight() const override { return text.getHeight(); }
    std::vector<cui::String> getData() const { return text.getData(); }

private:
    cui::Text text;
    std::chrono::high_resolution_clock::time_point lastTime;
    int fps = 0;
    int frameCount = 0;
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
};

int main() 
{
    cui::init();
    initFont("../../asserts/simhei.ttf");
    cui::Page page;

    auto progressBar1 = cui::progressBar(15);
    auto progressBar2 = cui::progressBar(20);
    auto progressBar3 = cui::progressBar(25);
    auto fps = std::make_shared<FPS>(page.onUpdate);
    auto apple = cui::image("../../asserts/textures/apple.png");

    {
        std::vector<std::shared_ptr<cui::Component>> hBox = {
            cui::image("../../asserts/textures/diamond_sword.png"),
            cui::text("   "),
            cui::text(getCharImage('C')),
            cui::text("   "),
            cui::text(getCharImage('U')),
            cui::text("   "),
            cui::text(getCharImage('I')),
            cui::text("   "),
            cui::text("CharUI是跨平台的控制台UI库\nCharUI支持UTF8字符😊")
        };
        int32_t w = 0;
        for (auto&& c : hBox) {
            page.set(w, 0, 0, c);
            w += c->getWidth();
        }
    }
    {
        std::vector<std::shared_ptr<cui::Component>> vBox = {
            progressBar1,
            progressBar2,
            progressBar3,
            fps
        };
        int32_t h = 16;
        for (auto&& c : vBox) {
            page.set(0, h, 0, c);
            h += c->getHeight();
        }
    }
    int32_t offset = 0;
    page.set(70, offset, 1, apple);

    page.onUpdate.connect([&]() { if (progressBar1->isDone()) { progressBar1->set(0); } else { progressBar1->set(progressBar1->get() + 10); } });
    page.onUpdate.connect([&]() { if (progressBar2->isDone()) { progressBar2->set(0); } else { progressBar2->set(progressBar2->get() + 10); } });
    page.onUpdate.connect([&]() { if (progressBar3->isDone()) { progressBar3->set(0); } else { progressBar3->set(progressBar3->get() + 10); } });
    page.onUpdate.connect([&]() {
        page.erase(70, offset, 1);
        page.set(70, ++offset, 1, apple);
        if (offset == 30) {
            page.erase(70, offset, 1);
            offset = -30;
        }
        });

    while (true) {
        page.update();
        page.display();
        //std::this_thread::sleep_for(100ms);
    }
    return 0;
}