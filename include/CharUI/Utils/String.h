#pragma once
#include <string>

namespace cui {

using Bytes = std::string;
using BytesView = std::string_view;

size_t charSize(const char* src);
size_t charWidth(const char* src);

constexpr BytesView defaultColor = "\x1b[0m";
Bytes RGB(uint8_t r, uint8_t g, uint8_t b);
Bytes bkRGB(uint8_t r, uint8_t g, uint8_t b);

class String {
public:
    struct ConstIterator {
        const char* operator*() const;
        ConstIterator& operator++();
        ConstIterator operator+(size_t offset) const;
        ConstIterator operator-(size_t offset) const;
        size_t operator-(const ConstIterator& other) const;
        bool operator==(const ConstIterator& other) const;

        const char* ptr; // => cgui::string::bytes
    };
    struct Iterator : public ConstIterator {
        char* operator*() const;
        Iterator& operator++();
        Iterator operator+(size_t offset) const;
        Iterator operator-(size_t offset) const;
        size_t operator-(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
    };
    struct WPosResult {
        Iterator iterator;
        size_t currentWidth;
    };

public:
    String();
    String(const String& str);
    String(const ConstIterator& first, const ConstIterator& last);
    String(const Bytes& bytes);
    String(const char* bytes);
    String(BytesView bytesView);
    String(size_t count, char c);

    size_t getSize() const;
    size_t getWidth() const;
    size_t getCount() const;
    size_t getVCount() const;
    const char* getData() const;

    void append(const String& other);
    void insert(const ConstIterator& it, const String& other);
    void insert(const ConstIterator& it, const char* other);
    void insert(size_t index, const String& other);
    void erase(size_t index, size_t n);
    
    void appendV(const String& other);
    void insertV(size_t index, const String& other);
    // void eraseV(size_t first, size_t last);
    
    void replaceW(size_t index, const String& other);
    String takeW(size_t index, size_t w) const;

    void setDefaultRGB(size_t index);
    void setRGB(size_t index, int r, int g, int b);
    void setBackgroundRGB(size_t index, int r, int g, int b);
    String operator+(const String& other);
    String& operator+=(const String& other);
    String& operator=(const String& other);
    String& operator=(std::string_view other);

    ConstIterator begin() const;
    ConstIterator end() const;
    Iterator begin();
    Iterator end();
    Iterator pos(const ConstIterator& offset, size_t n) const;
    Iterator pos(size_t n) const;
    Iterator vPos(const ConstIterator& offset, size_t n) const;
    Iterator vPos(size_t n) const;
    WPosResult wPos(const ConstIterator& offset, size_t w) const; // WPosResult.currentWidth >= w
    WPosResult wPos(size_t w) const; // WPosResult.currentWidth >= w
    WPosResult wPos1(const ConstIterator& offset, size_t w) const; // WPosResult.currentWidth <= w
    WPosResult wPos1(size_t w) const; // WPosResult.currentWidth <= w
    size_t wBeforeEnd(const ConstIterator& offset) const;

private:
    Bytes bytes;
    void removeBadChar();
    size_t pushBackPos() const;
    void __setRGB(size_t index, const String& other);
};
String operator+(BytesView lhs, const String& rhs);
String operator+(BytesView lhs, String&& rhs);

}