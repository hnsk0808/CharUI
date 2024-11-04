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
    int32_t maxWidth = 0;
    for (auto& line : data) {
        maxWidth = std::max(static_cast<int32_t>(line.getWidth()), maxWidth);
    }
    return maxWidth;
}

int32_t cui::Image::getHeight() const
{
    return static_cast<int32_t>(data.size());
}

std::vector<cui::String> cui::Image::getData() const
{
    return data;
}

void cui::Image::set(BytesView path)
{
    data.clear();
    int width = 0, height = 0, channels = 0;
    auto pixels = stbi_load(path.data(), &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels) {
        return;
    }
    for (int y = 0; y < height; ++y) {
        cui::String line = "";
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * channels;
            int r = pixels[i], g = pixels[i + 1], b = pixels[i + 2], a = pixels[i + 3];
            if (a < 128) {
                line.append("  ");
            }
            else {
                line.pushBackRGB(r, g, b);
                line.append("██");
            }
        }
        data.push_back(line);
    }
    stbi_image_free(pixels);
}

void cui::Image::set(BytesView path, int32_t width, int32_t height)
{
    data.clear();
    stbi_uc* pixels = nullptr;
    int channels = 0;
    {
        int srcWidth = 0, srcHeight = 0;
        auto rawPixels = stbi_load(path.data(), &srcWidth, &srcHeight, &channels, STBI_rgb);
        if (!rawPixels) {
            return;
        }
        pixels = new stbi_uc[width * height * channels];
        stbir_resize_uint8_linear(rawPixels, srcWidth, srcHeight, 0, pixels, width, height, 0, STBIR_RGB);
        stbi_image_free(rawPixels);
    }
    if (!pixels) {
        return;
    }

    for (int y = 0; y < height; ++y) {
        cui::String line = "";
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * channels;
            int r = pixels[i], g = pixels[i + 1], b = pixels[i + 2], a = pixels[i + 3];
            if (a < 128) {
                line.append("  ");
            }
            else {
                line.pushBackRGB(r, g, b);
                line.append("██");
            }
        }
        data.push_back(line);
    }
    stbi_image_free(pixels);
}
