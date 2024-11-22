#include <CharUI/Utils/String.h>
#include <CharUI/CharUI.h>
#include <cassert>
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
    assert(src);
    return utf8CharSize(src[0]);
}

size_t cui::charWidth(const char* src)
{
    assert(src);
    return static_cast<size_t>(wts8width(src, charSize(src)));
}

cui::Bytes cui::takeFirstChar(const char* src)
{
    assert(src);
    return Bytes(src, utf8CharSize(src[0]));
}

cui::String::String() = default;

cui::String::String(const String& other) : bytes(other.bytes) {}

cui::String::String(const ConstIterator& first, const ConstIterator& last) : String(Bytes(first.ptr, last.ptr)) {}

cui::String::String(BytesView bytesView) : bytes(bytesView)
{
    for (auto& byte : bytes) {
        if (byte == '\n' || byte == '\t') {
            byte = ' ';
        }
    }
}

cui::String::String(size_t count, char c) : String(Bytes(count, c)) {}

size_t cui::String::getSize() const { return bytes.size(); }

size_t cui::String::getCount() const { return beforeEnd(begin()); }

size_t cui::String::getWidth() const { return wBeforeEnd(begin()); }

const char* cui::String::getData() const { return bytes.data(); }

void cui::String::append(const String& other) { bytes += other.bytes; }

void cui::String::insert(const ConstIterator& first, const String& other) 
{ 
    assert(first >= begin() && first <= end());
    bytes.insert(first - begin(), other.bytes);
}

void cui::String::insert(size_t index, const String& other) { if (auto first = pos(index); first != end()) { insert(first, other); } }

void cui::String::erase(const ConstIterator& first, size_t n) 
{ 
    assert(first >= begin() && first <= end()); 
    bytes.erase(first - begin(), pos(first, n) - first);
}

void cui::String::erase(size_t index, size_t n) { if (auto first = pos(index); first != end()) { erase(first, n); } }

void cui::String::replace(const ConstIterator& first, const String& other) 
{ 
    assert(first >= begin() && first <= end());
    bytes.replace(first - begin(), end() - first, Bytes(other.begin().ptr, other.pos(beforeEnd(first)).ptr));
}

void cui::String::replace(size_t index, const String& other) { if (auto first = pos(index); first != end()) { replace(first, other); } }

void cui::String::replaceW(size_t index, const String& other)
{
    auto [first, indexWidth] = wPos1(index);
    if (first == end()) {
        return;
    }
    auto remainingWidth = wBeforeEnd(first);
    auto [otherIt, otherWidth] = other.wPos1(remainingWidth);
    auto str = String(begin(), first);
    str.append(String(index - indexWidth, ' '));
    str.append(String(other.begin(), otherIt));
    str.append(takeW(index + otherWidth, remainingWidth - otherWidth));
    *this = std::move(str);
}

cui::String cui::String::takeW(const ConstIterator& first, size_t w) const
{
    assert(first >= begin() && first <= end());
    if (w == 0) { return {}; }
    auto [last, currentWidth] = wPos1(first, w);
    return { Bytes(first.ptr, last.ptr) + Bytes(w - currentWidth, getGlobalPaddingChar()) };
}

cui::String cui::String::takeW(size_t index, size_t w) const
{
    if (w == 0) { return {}; }
    if (auto [first, indexWidth] = wPos(index); first != end()) {
        return String(indexWidth - index, ' ') + takeW(first, w);
    }
    return {};
}

cui::String cui::String::operator+(const String& other) { String ret = *this; ret.append(other); return ret; }

cui::String& cui::String::operator+=(const String& other) { append(other); return *this; }

cui::String& cui::String::operator=(const String& other) { bytes = other.bytes; return *this; }

cui::String::ConstIterator cui::String::begin() const { return bytes.data(); }

cui::String::ConstIterator cui::String::end() const { return bytes.data() + bytes.size(); }

cui::String::Iterator cui::String::begin() { return bytes.data(); }

cui::String::Iterator cui::String::end() { return bytes.data() + bytes.size(); }

cui::String::Iterator cui::String::pos(const ConstIterator& offset, size_t n) const
{
    assert(offset >= begin() && offset <= end());
    ConstIterator ret = offset;
    for (size_t i = 0; i < n && ret != end(); ++i, ++ret);
    return ret;
}

cui::String::Iterator cui::String::pos(size_t n) const { return pos(begin(), n); }

size_t cui::String::beforeEnd(const ConstIterator& offset) const
{
    assert(offset >= begin() && offset <= end());
    size_t ret = 0;
    for (ConstIterator it = offset; it != end(); ++ret, ++it);
    return ret;
}

cui::String::WPosResult cui::String::wPos(const ConstIterator& offset, size_t w) const
{
    assert(offset >= begin() && offset <= end());
    ConstIterator ret = offset;
    size_t retWidth = 0;
    for (; retWidth < w && ret != end(); retWidth += charWidth(ret.ptr), ++ret);
    return { ret, retWidth };
}

cui::String::WPosResult cui::String::wPos(size_t w) const { return wPos(begin(), w); }

cui::String::WPosResult cui::String::wPos1(const ConstIterator& offset, size_t w) const
{
    assert(offset >= begin() && offset <= end());
    ConstIterator ret = offset;
    size_t retWidth = 0;
    for (size_t cWidth = 0; (cWidth = charWidth(ret.ptr), retWidth + cWidth <= w) && ret != end(); retWidth += cWidth, ++ret);
    return { ret, retWidth };
}

cui::String::WPosResult cui::String::wPos1(size_t w) const { return wPos1(begin(), w); }

size_t cui::String::wBeforeEnd(const ConstIterator& offset) const
{
    assert(offset >= begin() && offset <= end());
    size_t width = 0;
    for (ConstIterator it = offset; it != end(); width += charWidth(it.ptr), ++it);
    return width;
}
