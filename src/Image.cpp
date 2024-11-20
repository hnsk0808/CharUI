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

const cui::ColorBuffer& cui::Image::getColorBuffer() const
{
    return colorBuffer;
}

cui::ColorBuffer& cui::Image::getColorBuffer()
{
    return colorBuffer;
}

void cui::Image::set(const uint8_t* pixels, int32_t w, int32_t h, int32_t channels)
{
    width = w * 2;
    height = h;
    colorBuffer.clear();
    for (int y = 0; y < h; ++y) {
        std::vector<Color> line;
        for (int x = 0; x < w; ++x) {
            int i = (y * w + x) * channels;
            if (channels == 4) {
                uint8_t r = pixels[i], g = pixels[i + 1], b = pixels[i + 2], a = pixels[i + 3];
                line.emplace_back(RGBA(r, g, b, a));
                line.emplace_back(RGBA(r, g, b, a));
            }
            else if (channels == 3) {
                uint8_t r = pixels[i], g = pixels[i + 1], b = pixels[i + 2];
                line.emplace_back(RGBA(r, g, b, 0xFF));
                line.emplace_back(RGBA(r, g, b, 0xFF));
            }
        }
        colorBuffer.push_back(std::move(line));
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
