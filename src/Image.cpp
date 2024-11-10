#include <CharUI/Component/Image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize2.h>

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
    return width * 2;
}

int32_t cui::Image::getHeight() const
{
    return height;
}

std::vector<cui::String> cui::Image::getData() const
{
    return {};
}

std::vector<std::vector<cui::Color>> cui::Image::getColorBuffer() const
{
    return colorBuffer;
}

void cui::Image::set(const uint8_t* pixels, int32_t width, int32_t height, int32_t channels)
{
    this->width = width;
    this->height = height;
    colorBuffer.clear();
    for (int y = 0; y < height; ++y) {
        String line;
        std::vector<Color> lineColor;
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * channels;
            int r = pixels[i], g = pixels[i + 1], b = pixels[i + 2], a = pixels[i + 3];
            if (a < 128) {
                lineColor.emplace_back(0xff, 0xff);
                lineColor.emplace_back(0xff, 0xff);
            }
            else {
                lineColor.emplace_back(r, g, b, r, g, b);
                lineColor.emplace_back(r, g, b, r, g, b);
            }
        }
        colorBuffer.push_back(std::move(lineColor));
    }
}

void cui::Image::set(BytesView path)
{
    int width = 0, height = 0, channels = 0;
    auto pixels = stbi_load(path.data(), &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels) {
        return;
    }
    set(pixels, width, height, channels);
    stbi_image_free(pixels);
}

void cui::Image::set(BytesView path, int32_t width, int32_t height)
{
    stbi_uc* pixels = nullptr;
    int channels = 0;
    {
        int srcWidth = 0, srcHeight = 0;
        auto rawPixels = stbi_load(path.data(), &srcWidth, &srcHeight, &channels, STBI_rgb_alpha);
        if (!rawPixels) {
            return;
        }
        pixels = new stbi_uc[width * height * channels];
        stbir_resize_uint8_linear(rawPixels, srcWidth, srcHeight, 0, pixels, width, height, 0, STBIR_RGBA);
        stbi_image_free(rawPixels);
    }
    if (!pixels) {
        return;
    }
    set(pixels, width, height, channels);
    stbi_image_free(pixels);
}
