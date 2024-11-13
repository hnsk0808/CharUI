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
    charBuffer.push_back(str);
}

cui::Text::Text(const std::vector<String>& lines)
    : charBuffer(lines) {}

int32_t cui::Text::getWidth() const
{
    int32_t maxWidth = 0;
    for (auto& line : charBuffer) {
        maxWidth = std::max(static_cast<int32_t>(line.getWidth()), maxWidth);
    }
    return maxWidth;
}

int32_t cui::Text::getHeight() const
{
    return static_cast<int32_t>(charBuffer.size());
}

const std::vector<cui::String>& cui::Text::getCharBuffer() const
{
    return charBuffer;
}

const cui::FeColorBuffer& cui::Text::getFeColorBuffer() const
{
    return feColorBuffer;
}

const cui::BkColorBuffer& cui::Text::getBkColorBuffer() const
{
    return bkColorBuffer;
}

void cui::Text::set(BytesView bytesView)
{
    charBuffer.clear();
    for (auto&& part : bytesView | std::views::split('\n') | std::ranges::to<std::vector<std::string>>()) {
        charBuffer.emplace_back(part.data());
    }
}

void cui::Text::set(const std::vector<String>& charBuffer)
{
    this->charBuffer = charBuffer;
}

std::vector<cui::String>& cui::Text::get()
{
    return charBuffer;
}

const std::vector<cui::String>& cui::Text::get() const
{
    return charBuffer;
}

void cui::Text::pushBack(const String& newLine) 
{
    charBuffer.push_back(newLine);
}

cui::String& cui::Text::operator[](size_t index) 
{
    return charBuffer.at(index);
}

const cui::String& cui::Text::operator[](size_t index) const 
{
    return charBuffer.at(index);
}