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
    : String(Bytes(bytes))
{}

cui::String::String(const Bytes& bytes)
    : chars((bytes + Bytes(defaultColor)).data())
{}

cui::String::String(BytesView bytesView)
    : String(Bytes(bytesView))
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
        if (charWidth(it.ptr) != 0) {
            ++i;
        }
    }
    while (charWidth(it.ptr) == 0) {
        ++it;
    }
    chars.insertByte(it - chars.begin(), other.chars.getBytes().substr(0, other.pushBackPos()).data());
}

cui::String cui::String::takeW(size_t index, size_t w) const
{
    if (index > chars.getWidth() || w == 0) {
        return "";
    }
    auto it = chars.begin();
    Bytes ret;
    size_t cW = 0;
    for (; cW < index; cW += charWidth(it.ptr), ++it);
    if (cW > index) {
        ret = Bytes(cW - index, ' ');
    }
    auto first = it, last = it;
    for (cW = 0; (it != chars.end()) && (cW < w); last = it, cW += charWidth(it.ptr), ++it);
    if (cW == w) {
        ret += Bytes(first.ptr, it.ptr);
    }
    else if (cW > w) {
        ret += Bytes(first.ptr, last.ptr) + Bytes(cW - w, getPaddingChar());
    }
    else if (cW < w) {
        ret += Bytes(first.ptr, it.ptr) + Bytes(w - cW, getPaddingChar());
    }
    return ret;
}

cui::String cui::String::replaceW(size_t index, const String& other) const
{
    if (index > chars.getWidth()) {
        return "";
    }
    auto width = getWidth();
    auto otherWidth = other.getWidth();
    auto before = takeW(0, index);
    if (otherWidth + index > width) {
        return before + other.takeW(0, width - index);
    }
    else {
        auto after = takeW(index + otherWidth, width - index - otherWidth);
        return before + other + after;
    }
}

void cui::String::pushBackDefaultRGB()
{
    append(defaultColor.data());
}

void cui::String::pushBackRGB(int r, int g, int b)
{
    append(RGBToANSIEscapeCode(38, r, g, b).data());
}

void cui::String::pushBackBackgroundRGB(int r, int g, int b)
{
    append(RGBToANSIEscapeCode(48, r, g, b).data());
}

void cui::String::insertDefaultRGB(size_t index)
{
    insert(index, defaultColor.data());
}

void cui::String::insertRGB(size_t index, int r, int g, int b)
{
    insert(index, RGBToANSIEscapeCode(38, r, g, b).data());
}

void cui::String::insertBackgroundRGB(size_t index, int r, int g, int b)
{
    insert(index, RGBToANSIEscapeCode(48, r, g, b).data());
}

void cui::String::setDefaultRGB(size_t index)
{
    __setRGB(index, defaultColor.data());
}

void cui::String::setRGB(size_t index, int r, int g, int b)
{
    __setRGB(index, RGBToANSIEscapeCode(38, r, g, b).data());
}

void cui::String::setBackgroundRGB(size_t index, int r, int g, int b)
{
    __setRGB(index, RGBToANSIEscapeCode(48, r, g, b).data());
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
        if (charWidth(it.ptr) != 0) {
            ++i;
        }
    }
    size_t eraseCount = 0;
    while (charWidth(it.ptr) == 0) {
        ++eraseCount;
        ++it;
    }
    chars.insertByte(it - chars.begin(), other.chars.getBytes().substr(0, other.pushBackPos()).data());
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
