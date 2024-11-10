#include <CharUI/Component/Text.h>
#include <ranges>

cui::Text::Text(BytesView bytesView)
{
    set(bytesView);
}

cui::Text::Text(const char* str)
    : Text(BytesView(str))
{}

cui::Text::Text(const String& str)
{
    lines.push_back(str);
}

cui::Text::Text(const std::vector<String>& lines)
    : lines(lines) {}

int32_t cui::Text::getWidth() const
{
    int32_t maxWidth = 0;
    for (auto& line : lines) {
        maxWidth = std::max(static_cast<int32_t>(line.getWidth()), maxWidth);
    }
    return maxWidth;
}

int32_t cui::Text::getHeight() const
{
    return static_cast<int32_t>(lines.size());
}

std::vector<cui::String> cui::Text::getData() const
{
    return lines;
}

std::vector<std::vector<cui::Color>> cui::Text::getColorBuffer() const
{
    return {};
}

void cui::Text::set(BytesView bytesView)
{
    lines.clear();
    for (auto&& part : bytesView | std::views::split('\n') | std::ranges::to<std::vector<std::string>>()) {
        lines.emplace_back(part.data());
    }
}

void cui::Text::set(const std::vector<String>& lines)
{
    this->lines = lines;
}

std::vector<cui::String>& cui::Text::get()
{
    return lines;
}

const std::vector<cui::String>& cui::Text::get() const
{
    return lines;
}

void cui::Text::pushBack(const String& newLine) 
{
    lines.push_back(newLine);
}

cui::String& cui::Text::operator[](size_t index) 
{
    return lines.at(index);
}

const cui::String& cui::Text::operator[](size_t index) const 
{
    return lines.at(index);
}