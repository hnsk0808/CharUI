#pragma once
#include <CharUI/Utils/String.h>
#include <vector>

namespace cui
{

class Text
{
public:
    Text(BytesView bytesView);
    Text(const char* str);
    Text(const String& str);
    Text(const std::vector<String>& lines);

    int32_t getWidth() const;
    int32_t getHeight() const;
    const std::vector<String>& getCharBuffer() const;

    void set(BytesView bytesView);
    void set(const std::vector<String>& lines);
    std::vector<String>& get();
    const std::vector<String>& get() const;
    void pushBack(const String& newLine);

    String& operator[](size_t index);
    const String& operator[](size_t index) const;

private:
    std::vector<String> charBuffer;
};

}