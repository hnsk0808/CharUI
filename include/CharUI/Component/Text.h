#pragma once
#include "Component.h"

namespace cui
{

class Text : public Component {
public:
    Text(std::string_view bytesView);
    Text(const std::vector<String>& lines);

    int32_t getWidth() const override;
    int32_t getHeight() const override;
    std::vector<String> getData() const override;

    void set(const std::vector<String>& lines);
    std::vector<String>& get();
    const std::vector<String>& get() const;
    void pushBack(const String& newLine);

    String& operator[](size_t index);
    const String& operator[](size_t index) const;

private:
    std::vector<String> lines;
};

}