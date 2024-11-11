#pragma once
#include "Component.h"

namespace cui
{

class Text : public Component {
public:
    Text(BytesView bytesView);
    Text(const char* str);
    Text(const String& str);
    Text(const std::vector<String>& lines);

    int32_t getWidth() const override;
    int32_t getHeight() const override;
    const std::vector<String>& getCharBuffer() const override;
    const std::vector<std::vector<Color>>& getColorBuffer() const override;

    void set(BytesView bytesView);
    void set(const std::vector<String>& lines);
    std::vector<String>& get();
    const std::vector<String>& get() const;
    void pushBack(const String& newLine);

    String& operator[](size_t index);
    const String& operator[](size_t index) const;

private:
    std::vector<String> charBuffer;
    std::vector<std::vector<Color>> colorBuffer;
};

}