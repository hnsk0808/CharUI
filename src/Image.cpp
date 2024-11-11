#include <CharUI/Component/Image.h>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

cui::Image::Image() = default;

cui::Image::Image(BytesView path)
{
    set(path);
}

cui::Image::Image(BytesView path, int32_t width, int32_t height)
{
    set(path, width, height);
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

const std::vector<std::vector<cui::Color>>& cui::Image::getColorBuffer() const
{
    return colorBuffer;
}

std::vector<std::vector<cui::Color>>& cui::Image::get()
{
    return colorBuffer;
}

void cui::Image::set(const uint8_t* pixels, int32_t width, int32_t height, int32_t channels)
{
    this->width = width * 2;
    this->height = height;
    colorBuffer.clear();
    for (int y = 0; y < height; ++y) {
        String line;
        std::vector<Color> lineColor;
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * channels;
            if (channels == 4) {
                int r = pixels[i], g = pixels[i + 1], b = pixels[i + 2], a = pixels[i + 3];
                if (a < 128) {
                    lineColor.emplace_back();
                    lineColor.emplace_back();
                }
                else {
                    lineColor.emplace_back(0, 0, 0, 0, r, g, b, 0xff);
                    lineColor.emplace_back(0, 0, 0, 0, r, g, b, 0xff);
                }
            }
            else if (channels == 3) {
                int r = pixels[i], g = pixels[i + 1], b = pixels[i + 2];
                lineColor.emplace_back(0, 0, 0, 0, r, g, b, 0xff);
                lineColor.emplace_back(0, 0, 0, 0, r, g, b, 0xff);
            }
        }
        colorBuffer.push_back(std::move(lineColor));
    }
}

void cui::Image::set(BytesView path)
{
    std::filesystem::path p(path);
    bool isPng = p.extension() == ".png";

    int width = 0, height = 0, channels = 0;
    auto pixels = stbi_load(path.data(), &width, &height, &channels, isPng ? STBI_rgb_alpha : STBI_rgb);
    if (!pixels) {
        return;
    }
    set(pixels, width, height, channels);
    stbi_image_free(pixels);
}

void cui::Image::set(BytesView path, int32_t width, int32_t height)
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
        pixels = new stbi_uc[width * height * channels];
        stbir_resize_uint8(rawPixels, srcWidth, srcHeight, 0, pixels, width, height, 0, channels);
    }
    if (!pixels) {
        return;
    }
    set(pixels, width, height, channels);
    stbi_image_free(pixels);
}
