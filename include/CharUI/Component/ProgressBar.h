#pragma once
#include "Component.h"

namespace cui
{

struct ProgressStyle
{
	char beginChar, fillChar, seqChar, paddingChar, endChar;
};

class ProgressBar : public Component
{
public:
	ProgressBar(int32_t length);

	int32_t getWidth() const override;
	int32_t getHeight() const override;
	std::vector<String> getData() const override;

	void setStyle(const ProgressStyle& style);
	void set(int progress);
	int get() const;
	bool isDone() const;

private:
	int progress;
	int32_t length;
	ProgressStyle style;
};

}