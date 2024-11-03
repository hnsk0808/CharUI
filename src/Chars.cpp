#include <CharUI/Utils/Chars.h>
#include <wtswidth/wtswidth.h>

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

cui::Bytes cui::RGBToANSIEscapeCode(int mod, int r, int g, int b)
{
    return "\x1b[" + std::to_string(mod) + ";2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

cui::Chars::Chars()
{}

cui::Chars::Chars(const char* bytes)
    : bytes(bytes)
{
    removeBadChar();
}

cui::Chars::Chars(size_t count, char c)
    : bytes(count, c)
{
    removeBadChar();
}

size_t cui::Chars::getSize() const
{
    return bytes.size();
}

size_t cui::Chars::getWidth() const
{
    size_t width = 0;
    for (auto c : *this) {
        width += charWidth(c);
    }
    return width;
}

size_t cui::Chars::getCount() const
{
    size_t i = 0;
    for (auto c : *this) {
        ++i;
    }
    return i;
}

size_t cui::Chars::getVCount() const
{
    size_t i = 0;
    for (auto c : *this) {
        if (charWidth(c) != 0) {
            ++i;
        }
    }
    return i;
}

const char* cui::Chars::getData() const
{
    return bytes.data();
}

void cui::Chars::append(const Chars& other)
{
    bytes += other.bytes;
}

void cui::Chars::insert(size_t index, const Chars& other)
{
    if (index > getVCount()) {
        return;
    }
    auto it = begin();
    for (size_t i = 0; i < index; ++i) {
        ++it;
    }
    bytes.insert(it.p - begin(), other.bytes);
}

void cui::Chars::erase(size_t index, size_t n)
{
    if (index > getCount() || n == 0) {
        return;
    }
    if (index + n > getCount()) {
        n = getCount() - index;
    }
    auto it = begin();
    for (size_t i = 0; i < index; ++i, ++it);
    size_t firstPos = it - begin();
    for (size_t i = 0; i < n; ++i, ++it);
    size_t lastPos = it - begin();
    bytes.erase(firstPos, lastPos - firstPos);
}

void cui::Chars::insertByte(size_t index, const Chars& other)
{
    bytes.insert(index, other.bytes);
}

void cui::Chars::insertByte(size_t index, const char* other)
{
    insertByte(index, Chars(other));
}

cui::Chars& cui::Chars::operator=(const Chars& other)
{
    bytes = other.bytes;
    return *this;
}

cui::Chars::iterator cui::Chars::begin()
{
    return { bytes.data() };
}

cui::Chars::iterator cui::Chars::end()
{
    return { bytes.data() + bytes.size() };
}

cui::Chars::ConstIterator cui::Chars::begin() const
{
    return { bytes.data() };
}

cui::Chars::ConstIterator cui::Chars::end() const
{
    return { bytes.data() + bytes.size() };
}

void cui::Chars::removeBadChar()
{
    for (auto& byte : bytes) {
        if (byte == '\n' || byte == '\t') {
            byte = ' ';
        }
    }
}

cui::Chars::iterator& cui::Chars::iterator::operator++()
{
    p += charSize(p);
    return *this;
}

bool cui::Chars::iterator::operator==(const iterator& other) const
{
    return p == other.p;
}

char* cui::Chars::iterator::operator*()
{
    return p;
}

cui::Chars::iterator::operator char* ()
{
    return p;
}

cui::Chars::ConstIterator& cui::Chars::ConstIterator::operator++()
{
    p += charSize(p);
    return *this;
}

bool cui::Chars::ConstIterator::operator==(const ConstIterator& other) const
{
    return p == other.p;
}

const char* cui::Chars::ConstIterator::operator*() const
{
    return p;
}

cui::Chars::ConstIterator::operator const char* () const
{
    return p;
}