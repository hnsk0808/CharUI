#pragma once
#include <string>
#include "Chars.h"

namespace cui {

constexpr BytesView defaultColor = "\x1b[0m";

class String {
public:
    String();
    String(const String& str);
    String(const Chars& chars);
    String(const char* bytes);
    String(const Bytes& bytes);
    String(BytesView bytesView);
    String(size_t count, char c);

    template<std::input_iterator Iter>
    String(Iter first, Iter last)
        : chars((Bytes(first, last) + Bytes(defaultColor)).data())
    {}

    size_t getSize() const;
    size_t getWidth() const;
    size_t getCount() const;
    size_t getVCount() const;
    const char* getData() const;
    Chars& getChars();
    const Chars& getChars() const;
    
    void append(const String& other);
    void insert(size_t index, const String& other);
    // void erase(size_t first, size_t last);
    
    String takeW(size_t index, size_t w) const;
    String replaceW(size_t index, size_t w, const String& other) const;

    void pushBackDefaultRGB();
    void pushBackRGB(int r, int g, int b);
    void pushBackBackgroundRGB(int r, int g, int b);
    void insertDefaultRGB(size_t index);
    void insertRGB(size_t index, int r, int g, int b);
    void insertBackgroundRGB(size_t index, int r, int g, int b);
    void setDefaultRGB(size_t index);
    void setRGB(size_t index, int r, int g, int b);
    void setBackgroundRGB(size_t index, int r, int g, int b);
    String operator+(const String& other);
    String& operator+=(const String& other);
    String& operator=(const String& other);
    String& operator=(std::string_view other);

private:
    Chars chars;

    size_t pushBackPos() const;
    void __setRGB(size_t index, const String& other);
};
String operator+(BytesView lhs, String& rhs);
String operator+(BytesView lhs, String&& rhs);

}