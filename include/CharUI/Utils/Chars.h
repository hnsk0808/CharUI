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

    // Iterator
    struct iterator {
        iterator& operator++();
        bool operator==(const iterator& other) const;
        char* operator*();
        operator char* ();
        char* p; // => cgui::string::bytes
    };
    struct ConstIterator {
        ConstIterator& operator++();
        bool operator==(const ConstIterator& other) const;
        const char* operator*() const;
        operator const char* () const;
        const char* p; // => cgui::string::bytes
    };
    iterator begin();
    iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

private:
    Bytes bytes;

    void removeBadChar();
};

}