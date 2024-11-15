#pragma once
#include <string>

namespace cui {

using Bytes = std::string;
using BytesView = std::string_view;

size_t charSize(const char* src);
size_t charWidth(const char* src);
Bytes takeFirstChar(const char* src);

class String {
public:
    struct ConstIterator {
        ConstIterator() : ptr(nullptr) {}
        ConstIterator(const ConstIterator& other) : ptr(other.ptr) {}
        ConstIterator(const char* ptr) : ptr(ptr) {}
        const char* operator*() const { return ptr; }
        ConstIterator& operator++() { ptr += charSize(ptr); return *this; }
        ConstIterator operator+(size_t offset) const { ConstIterator ret = *this; ret.ptr += offset; return ret; }
        ConstIterator operator-(size_t offset) const { ConstIterator ret = *this; ret.ptr -= offset; return ret; }
        size_t operator-(const ConstIterator& other) const { return static_cast<size_t>(ptr - other.ptr); }
        bool operator==(const ConstIterator& other) const { return ptr == other.ptr; }
        std::strong_ordering operator<=>(const ConstIterator& other) const { return ptr <=> other.ptr; }
        const char* ptr; // => cgui::string::bytes
    };
    struct Iterator : public ConstIterator {
        Iterator() : ConstIterator() {}
        Iterator(const ConstIterator& other) : ConstIterator(other.ptr) {}
        Iterator(const char* ptr) : ConstIterator(ptr) {}
        char* operator*() const { return const_cast<char*>(ptr); }
        Iterator& operator++() { ptr += charSize(ptr); return *this; }
        Iterator operator+(size_t offset) const { Iterator ret = *this; ret.ptr += offset; return ret; }
        using ConstIterator::operator-;
        Iterator operator-(size_t offset) const { Iterator ret = *this; ret.ptr -= offset; return ret; }
        size_t operator-(const Iterator& other) const { return static_cast<size_t>(ptr - other.ptr); }
        using ConstIterator::operator==;
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        using ConstIterator::operator<=>;
        std::strong_ordering operator<=>(const Iterator& other) const { return ptr <=> other.ptr; }
    };
    struct WPosResult {
        Iterator iterator;
        size_t currentWidth;
    };

public:
    String();
    String(const String& other);
    String(const ConstIterator& first, const ConstIterator& last);
    String(const Bytes& cstr);
    String(const char* cstr);
    String(BytesView bytesView);
    String(size_t count, char c);

    size_t getSize() const;
    size_t getCount() const;
    size_t getWidth() const;
    const char* getData() const;

    void append(const String& other);
    void insert(const ConstIterator& first, const String& other);
    void insert(size_t index, const String& other);
    void erase(const ConstIterator& index, size_t n);
    void erase(size_t index, size_t n);
    void replace(const ConstIterator& first, const String& other);
    void replace(size_t index, const String& other);
    
    void replaceW(size_t index, const String& other);
    String takeW(const ConstIterator& first, size_t w) const;
    String takeW(size_t index, size_t w) const;

    String operator+(const String& other);
    String& operator+=(const String& other);
    String& operator=(const String& other);

    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator begin();
    Iterator end();
    Iterator pos(const ConstIterator& offset, size_t n) const;
    Iterator pos(size_t n) const;
    size_t beforeEnd(const ConstIterator& offset) const;
    WPosResult wPos(const ConstIterator& offset, size_t w) const; // WPosResult.currentWidth >= w
    WPosResult wPos(size_t w) const; // WPosResult.currentWidth >= w
    WPosResult wPos1(const ConstIterator& offset, size_t w) const; // WPosResult.currentWidth <= w
    WPosResult wPos1(size_t w) const; // WPosResult.currentWidth <= w
    size_t wBeforeEnd(const ConstIterator& offset) const;

private:
    Bytes bytes;
};

}