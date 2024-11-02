#include <CharUI/Utils/String.h>
#include <CharUI/Config.h>

cui::String::String()
    : chars(defaultColor.data())
{}

cui::String::String(const String & str)
    : chars(str.chars)
{}

cui::String::String(const Chars & chars)
    : chars(chars)
{}

cui::String::String(const char* bytes)
    : chars((Bytes(bytes) + Bytes(defaultColor)).c_str())
{}

cui::String::String(BytesView bytes)
    : String(bytes.data())
{}

cui::String::String(size_t count, char c)
    : chars(count, c)
{
    chars.append(defaultColor.data());
}

size_t cui::String::getSize() const
{ 
    return chars.getSize();
}

size_t cui::String::getWidth() const
{ 
    return chars.getWidth();
}

size_t cui::String::getCount() const
{
    return chars.getCount();
}

size_t cui::String::getVCount() const
{
    return chars.getVCount();
}

const char* cui::String::getData() const
{
    return chars.getData();
}

cui::Chars& cui::String::getChars()
{
    return chars;
}

const cui::Chars& cui::String::getChars() const
{
    return chars;
}

void cui::String::append(const String& other)
{
    chars.insertByte(pushBackPos(), other.chars.getBytes().substr(0, other.pushBackPos()).data());
}

void cui::String::insert(size_t index, const String& other)
{
    if (index > chars.getVCount()) {
        return;
    }
    auto it = chars.begin();
    for (size_t i = 0; i < index; ++it) {
        if (charWidth(it) != 0) {
            ++i;
        }
    }
    while (charWidth(it) == 0) {
        ++it;
    }
    chars.insertByte(it.p - chars.begin(), other.chars.getBytes().substr(0, other.pushBackPos()).data());
}

cui::String cui::String::takeW(size_t index, size_t w) const
{
    if (index > chars.getWidth() || w == 0) {
        return "";
    }
    auto it = chars.begin();
    Bytes ret = "";
    {
        size_t i = 0;
        for (; i < index; i += charWidth(it), ++it);
        if (i > index) {
            ret = Bytes(i - index, ' ');
        }
    }
    auto first = it;
    auto last = it;
    size_t currentWidth = index;
    for (; (currentWidth < w) && (it != chars.end()); ++it) {
        last = it;
        currentWidth += charWidth(it);
    }
    if (currentWidth == w) {
        return (ret + Bytes(first.p, it.p)).data();
    }
    else if (currentWidth > w) {
        currentWidth -= charWidth(last);
    }
    ret += Bytes(first.p, last.p) + Bytes(w - currentWidth, getPaddingChar());
    return { ret.data() };
}

void cui::String::pushBackDefaultRGB()
{
    append(defaultColor.data());
}

void cui::String::pushBackRGB(int r, int g, int b)
{
    append(colorAnsiEscapeCode(38, r, g, b).data());
}

void cui::String::pushBackBackgroundRGB(int r, int g, int b)
{
    append(colorAnsiEscapeCode(48, r, g, b).data());
}

void cui::String::insertDefaultRGB(size_t index)
{
    insert(index, defaultColor.data());
}

void cui::String::insertRGB(size_t index, int r, int g, int b)
{
    insert(index, colorAnsiEscapeCode(38, r, g, b).data());
}

void cui::String::insertBackgroundRGB(size_t index, int r, int g, int b)
{
    insert(index, colorAnsiEscapeCode(48, r, g, b).data());
}

void cui::String::setDefaultRGB(size_t index)
{
    __setRGB(index, defaultColor.data());
}

void cui::String::setRGB(size_t index, int r, int g, int b)
{
    __setRGB(index, colorAnsiEscapeCode(38, r, g, b).data());
}

void cui::String::setBackgroundRGB(size_t index, int r, int g, int b)
{
    __setRGB(index, colorAnsiEscapeCode(48, r, g, b).data());
}

cui::String cui::String::operator+(const String& other)
{
    String ret = *this;
    ret.append(other);
    return ret;
}

cui::String& cui::String::operator+=(const String& other) {
    append(other);
    return *this;
}

cui::String& cui::String::operator=(const String& other)
{
    chars = other.chars;
    return *this;
}

cui::String& cui::String::operator=(std::string_view other)
{
    return operator=(String(other));
}

size_t cui::String::pushBackPos() const
{
     return chars.getSize() - defaultColor.size();
}

void cui::String::__setRGB(size_t index, const String& other)
{
    if (index > chars.getVCount()) {
        return;
    }
    auto it = chars.begin();
    size_t eraseIndex = 0;
    for (size_t i = 0; i < index; ++it, ++eraseIndex) {
        if (charWidth(it) != 0) {
            ++i;
        }
    }
    size_t eraseCount = 0;
    while (charWidth(it) == 0) {
        ++eraseCount;
        ++it;
    }
    chars.insertByte(it.p - chars.begin(), other.chars.getBytes().substr(0, other.pushBackPos()).data());
    chars.erase(eraseIndex, eraseCount);
}

cui::String cui::operator+(BytesView lhs, String& rhs)
{
    return String(lhs) + rhs;
}

cui::String cui::operator+(BytesView lhs, String&& rhs)
{
    return String(lhs) + rhs;
}
