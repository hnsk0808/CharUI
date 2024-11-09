#pragma once
#include <string>

namespace cui {

using Bytes = std::string;
using BytesView = std::string_view;

size_t charSize(const char* src);
size_t charWidth(const char* src);
Bytes RGBToANSIEscapeCode(int mod, int r, int g, int b);

class Chars {
public:
    Chars();
    Chars(const char* bytes);
    Chars(size_t count, char c);

    size_t getSize() const;
    size_t getWidth() const;
    size_t getCount() const;
    size_t getVCount() const;
    const char* getData() const;

    Bytes& getBytes() { return bytes; }
    const Bytes& getBytes() const { return bytes; }

    void append(const Chars& other);
    void insert(size_t index, const Chars& other);
    void erase(size_t index, size_t n);

    void insertByte(size_t index, const Chars& other);
    void insertByte(size_t index, const char* other);

    Chars& operator=(const Chars& other);

public:
    struct ConstIterator {
        const char* operator*() const;
        ConstIterator& operator++();
        size_t operator-(const ConstIterator& other) const;
        bool operator==(const ConstIterator& other) const;

        const char* ptr; // => cgui::string::bytes
    };
    struct Iteratror : public ConstIterator {
        char* operator*() const;
        Iteratror& operator++();
        size_t operator-(const Iteratror& other) const;
        bool operator==(const Iteratror& other) const;
    };

public:
    ConstIterator begin() const;
    ConstIterator end() const;
    Iteratror begin();
    Iteratror end();

private:
    Bytes bytes;

    void removeBadChar();
};

}