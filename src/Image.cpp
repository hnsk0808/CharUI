#include <CharUI/Component/Image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

static std::vector<cui::String> getImage(std::string_view path) {
    std::vector<cui::String> lines;
    int width = 0, height = 0, channels = 0;
    auto pixels = stbi_load(path.data(), &width, &height, &channels, STBI_rgb_alpha);
    for (int y = 0; y < height; ++y) {
        cui::String line = "";
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * channels;
            int r = pixels[i], g = pixels[i + 1], b = pixels[i + 2], a = pixels[i + 3];
            if (a < 128) {
                line += "  ";
            }
            else {
                line.pushBackRGB(r, g, b);
                line += "██";
            }
        }
        lines.push_back(line);
    }
    stbi_image_free(pixels);
    return lines;
}

cui::Image::Image(std::string_view path)
    : data(getImage(path))
{}

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

void cui::Image::set(std::string_view path)
{
	this->data = getImage(path);
}