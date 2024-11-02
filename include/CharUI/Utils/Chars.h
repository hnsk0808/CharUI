#pragma once
#include <string>

namespace cui {

using Bytes = std::string;
using BytesView = std::string_view;

// 返回字符串中第一个UTF-8字符/ANSI转义序列的字节数
size_t charSize(const char* src);
// 返回字符串中第一个UTF-8字符/ANSI转义序列的宽度
size_t charWidth(const char* src);
// rgb转换为ANSI转义序列
Bytes colorAnsiEscapeCode(int mod, int r, int g, int b);

// 包含ANSI转义字符，UTF-8字符
// 不包含\n \t
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

    // 移除\n \t
    void removeBadChar();
};

}