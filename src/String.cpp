#include <CharUI/Utils/String.h>
#include <CharUI/Config.h>
#include <wtswidth/wtswidth.h>
#include <format>

static size_t utf8CharSize(char firstByte)
{
    if ((firstByte & 0x80) == 0x00) { return 1; }
    else if ((firstByte & 0xE0) == 0xC0) { return 2; }
    else if ((firstByte & 0xF0) == 0xE0) { return 3; }
    else if ((firstByte & 0xF8) == 0xF0) { return 4; }
    else { return 0; }
}

size_t cui::charSize(const char* src)
{
    if (!src) {
        return 0;
    }
    if (src[0] == '\x1b') {
        size_t ret = 1;
        for (; src[ret] != 0; ++ret) {
            if (src[ret] == 'm') {
                return ret + 1;
            }
        }
        return ret;
    }
    size_t ret = 0;
    for (; (ret < utf8CharSize(src[0])) && (src[ret] != 0 && src[ret] != '\x1b'); ++ret);
    return ret;
}

size_t cui::charWidth(const char* src)
{
    if (!src) {
        return 0;
    }
    if (src[0] == '\x1b') {
        return 0;
    }
    size_t ret = 0;
    for (; (ret < utf8CharSize(src[0])) && (src[ret] != 0 && src[ret] != '\x1b'); ++ret);
    return wts8width(src, ret);
}

cui::Bytes cui::RGB(uint8_t r, uint8_t g, uint8_t b)
{
    return std::format("\x1b[38;2;{};{};{}m", r, g, b);
}

cui::Bytes cui::bkRGB(uint8_t r, uint8_t g, uint8_t b)
{
    return std::format("\x1b[48;2;{};{};{}m", r, g, b);
}

cui::String::String()
    : bytes(defaultColor)
{}

cui::String::String(const String& str)
    : bytes(str.bytes)
{}

cui::String::String(const ConstIterator& first, const ConstIterator& last)
    : String(Bytes(first.ptr, last.ptr))
{}

cui::String::String(const Bytes& bytes)
    : bytes(bytes)
{
    removeBadChar();
    if (!this->bytes.ends_with(defaultColor)) {
        this->bytes.append(defaultColor);
    }
}

cui::String::String(const char* bytes)
    : String(Bytes(bytes))
{}

cui::String::String(BytesView bytesView)
    : String(Bytes(bytesView))
{}

cui::String::String(size_t count, char c)
    : bytes(Bytes(count, c))
{
    removeBadChar();
    this->bytes.append(defaultColor);
}

size_t cui::String::getSize() const
{ 
    return bytes.size();
}

size_t cui::String::getWidth() const
{ 
    return wBeforeEnd(begin());
}

size_t cui::String::getCount() const
{
    size_t ret = 0;
    for (auto c : *this) {
        ++ret;
    }
    return ret;
}

size_t cui::String::getVCount() const
{
    size_t ret = 0;
    for (auto c : *this) {
        if (charWidth(c) != 0) {
            ++ret;
        }
    }
    return ret;
}

const char* cui::String::getData() const
{
    return bytes.data();
}

void cui::String::append(const String& other)
{
    bytes += other.bytes;
}

void cui::String::insert(const ConstIterator& it, const String& other)
{
    bytes.insert(it - begin(), other.getData());
}

void cui::String::insert(const ConstIterator& it, const char* other)
{
    bytes.insert(it - begin(), other);
}

void cui::String::insert(size_t index, const String& other)
{
    auto it = pos(index);
    if (it == end()) {
        return;
    }
    insert(it, other);
}

void cui::String::erase(size_t index, size_t n)
{
    auto first = pos(index);
    if (first == end()) {
        return;
    }
    auto last = pos(first, n);
    bytes.erase(first - begin(), last - first);
}

void cui::String::appendV(const String& other)
{
    insert(begin() + pushBackPos(), other.bytes.substr(0, other.pushBackPos()).data());
}

void cui::String::insertV(size_t index, const String& other)
{
    auto it = vPos(index);
    if (it == end()) {
        return;
    }
    insert(it, other.bytes.substr(0, other.pushBackPos()).data());
}

cui::String cui::String::takeW(size_t index, size_t w) const
{
    if (w == 0) {
        return {};
    }
    auto [first, indexWidth] = wPos(index);
    if (first == end()) {
        return {};
    }
    auto [last, currentWidth] = wPos1(first, w);
    return Bytes(indexWidth - index, ' ') + Bytes(first.ptr, last.ptr) + Bytes(w - currentWidth, getPaddingChar());
}

cui::String cui::String::replaceW(size_t index, const String& other) const
{
    auto [first, indexWidth] = wPos1(index);
    if (first == end()) {
        return {};
    }
    auto remainingWidth = wBeforeEnd(first);
    auto [otherIt, otherWidth] = other.wPos1(remainingWidth);
    return Bytes(begin().ptr, first.ptr) + Bytes(index - indexWidth, ' ') + Bytes(other.begin().ptr, otherIt.ptr) + takeW(index + otherWidth, remainingWidth - otherWidth);
}

void cui::String::setDefaultRGB(size_t index)
{
    __setRGB(index, defaultColor.data());
}

void cui::String::setRGB(size_t index, int r, int g, int b)
{
    __setRGB(index, RGB(r, g, b).data());
}

void cui::String::setBackgroundRGB(size_t index, int r, int g, int b)
{
    __setRGB(index, bkRGB(r, g, b).data());
}

cui::String cui::String::operator+(const String& other)
{
    String ret = *this;
    ret.appendV(other);
    return ret;
}

cui::String& cui::String::operator+=(const String& other) {
    appendV(other);
    return *this;
}

cui::String& cui::String::operator=(const String& other)
{
    bytes = other.bytes;
    return *this;
}

cui::String& cui::String::operator=(std::string_view other)
{
    return operator=(String(other));
}

cui::String::ConstIterator cui::String::begin() const
{
    return { bytes.data() };
}

cui::String::ConstIterator cui::String::end() const
{
    return { bytes.data() + bytes.size() };
}

cui::String::Iterator cui::String::begin()
{
    return { bytes.data() };
}

cui::String::Iterator cui::String::end()
{
    return { bytes.data() + bytes.size() };
}

cui::String::Iterator cui::String::pos(const ConstIterator& offset, size_t n) const
{
    ConstIterator ret = offset;
    for (size_t i = 0; i < n && ret != end(); ++i, ++ret);
    return { ret.ptr };
}

cui::String::Iterator cui::String::pos(size_t n) const
{
    return pos(begin(), n);
}

cui::String::Iterator cui::String::vPos(const ConstIterator& offset, size_t n) const
{
    ConstIterator ret = offset;
    for (; !charWidth(ret.ptr) && ret != end(); ++ret);
    for (size_t i = 0; i < n && ret != end(); ++ret, charWidth(ret.ptr) ? ++i : 0);
    return { ret.ptr };
}

cui::String::Iterator cui::String::vPos(size_t n) const
{
    return vPos(begin(), n);
}

cui::String::WPosResult cui::String::wPos(const ConstIterator& offset, size_t w) const
{
    ConstIterator ret = offset;
    size_t retWidth = 0;
    for (; retWidth < w && ret != end(); retWidth += charWidth(ret.ptr), ++ret);
    return { ret, retWidth };
}

cui::String::WPosResult cui::String::wPos(size_t w) const
{
    return wPos(begin(), w);
}

cui::String::WPosResult cui::String::wPos1(const ConstIterator& offset, size_t w) const
{
    ConstIterator ret = offset;
    size_t retWidth = 0;
    for (size_t cWidth = 0; ret != end(); ++ret) {
        cWidth = charWidth(ret.ptr);
        if (retWidth + cWidth <= w) {
            retWidth += cWidth;
        }
        else {
            break;
        }
    }
    return { ret, retWidth };
}

cui::String::WPosResult cui::String::wPos1(size_t w) const
{
    return wPos1(begin(), w);
}

size_t cui::String::wBeforeEnd(const ConstIterator& offset) const
{
    ConstIterator it = offset;
    size_t width = 0;
    for (; it != end(); ++it) {
        width += charWidth(it.ptr);
    }
    return width;
}

void cui::String::removeBadChar()
{
    for (auto& byte : bytes) {
        if (byte == '\n' || byte == '\t') {
            byte = ' ';
        }
    }
}

size_t cui::String::pushBackPos() const
{
     return bytes.size() - defaultColor.size();
}

void cui::String::__setRGB(size_t index, const String& other)
{
    if (index > getVCount()) {
        return;
    }
    auto it = begin();
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
    insert(it, other.bytes.substr(0, other.pushBackPos()).data());
    erase(eraseIndex, eraseCount);
}

const char* cui::String::ConstIterator::operator*() const
{
    return ptr;
}

cui::String::ConstIterator& cui::String::ConstIterator::operator++()
{
    ptr += charSize(ptr);
    return *this;
}

cui::String::ConstIterator cui::String::ConstIterator::operator+(size_t offset) const
{
    auto ret = *this;
    ret.ptr += offset;
    return ret;
}

cui::String::ConstIterator cui::String::ConstIterator::operator-(size_t offset) const
{
    auto ret = *this;
    ret.ptr -= offset;
    return ret;
}

size_t cui::String::ConstIterator::operator-(const ConstIterator& other) const
{
    return ptr - other.ptr;
}

bool cui::String::ConstIterator::operator==(const ConstIterator& other) const
{
    return ptr == other.ptr;
}

char* cui::String::Iterator::operator*() const
{
    return const_cast<char*>(ptr);
}

cui::String::Iterator& cui::String::Iterator::operator++()
{
    ptr += charSize(ptr);
    return *this;
}

cui::String::Iterator cui::String::Iterator::operator+(size_t offset) const
{
    auto ret = *this;
    ret.ptr += offset;
    return ret;
}

cui::String::Iterator cui::String::Iterator::operator-(size_t offset) const
{
    auto ret = *this;
    ret.ptr -= offset;
    return ret;
}

size_t cui::String::Iterator::operator-(const Iterator& other) const
{
    return ptr - other.ptr;
}

bool cui::String::Iterator::operator==(const Iterator& other) const
{
    return ptr == other.ptr;
}

cui::String cui::operator+(BytesView lhs, const String& rhs)
{
    return String(lhs) + rhs;
}

cui::String cui::operator+(BytesView lhs, String&& rhs)
{
    return String(lhs) + rhs;
}
