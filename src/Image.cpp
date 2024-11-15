#include <CharUI/Component/Image.h>
#include <filesystem>
#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>

cui::Image::Image() = default;

cui::Image::Image(BytesView path)
{
    set(path);
}

cui::Image::Image(BytesView path, int32_t w, int32_t h)
{
    set(path, w, h);
}

int32_t cui::Image::getWidth() const
{
    return width;
}

int32_t cui::Image::getHeight() const
{
    return height;
}

const std::vector<cui::String>& cui::Image::getCharBuffer() const
{
    return charBuffer;
}

const cui::FeColorBuffer& cui::Image::getFeColorBuffer() const
{
    return feColorBuffer;
}

const cui::BkColorBuffer& cui::Image::getBkColorBuffer() const
{
    return bkColorBuffer;
}

std::vector<std::vector<cui::Color>>& cui::Image::get()
{
    return bkColorBuffer;
}

void cui::Image::set(const uint8_t* pixels, int32_t w, int32_t h, int32_t channels)
{
    width = w * 2;
    height = h;
    bkColorBuffer.clear();
    for (int y = 0; y < height; ++y) {
        String line;
        std::vector<Color> lineColor;
        for (int x = 0; x < w; ++x) {
            int i = (y * w + x) * channels;
            if (channels == 4) {
                int r = pixels[i], g = pixels[i + 1], b = pixels[i + 2], a = pixels[i + 3];
                lineColor.emplace_back(r, g, b, a);
                lineColor.emplace_back(r, g, b, a);
            }
            else if (channels == 3) {
                int r = pixels[i], g = pixels[i + 1], b = pixels[i + 2];
                lineColor.emplace_back(r, g, b, 0xff);
                lineColor.emplace_back(r, g, b, 0xff);
            }
        }
        bkColorBuffer.push_back(std::move(lineColor));
    }
}

void cui::Image::set(BytesView path)
{
    std::filesystem::path p(path);
    bool isPng = p.extension() == ".png";

    int w = 0, h = 0, channels = 0;
    auto pixels = stbi_load(path.data(), &w, &h, &channels, isPng ? STBI_rgb_alpha : STBI_rgb);
    if (!pixels) {
        return;
    }
    set(pixels, w, h, channels);
    stbi_image_free(pixels);
}

void cui::Image::set(BytesView path, int32_t w, int32_t h)
{
    std::filesystem::path p(path);
    bool isPng = p.extension() == ".png";

    stbi_uc* pixels = nullptr;
    int channels = 0;
    {
        int srcWidth = 0, srcHeight = 0;
        auto rawPixels = stbi_load(path.data(), &srcWidth, &srcHeight, &channels, isPng ? STBI_rgb_alpha : STBI_rgb);
        if (!rawPixels) {
            return;
        }
        pixels = new stbi_uc[static_cast<size_t>(w * h * channels)];
        stbir_resize_uint8(rawPixels, srcWidth, srcHeight, 0, pixels, w, h, 0, channels);
    }
    if (!pixels) {
        return;
    }
    set(pixels, w, h, channels);
    stbi_image_free(pixels);
}
