#include <CharUI/Component/ProgressBar.h>
#include <cassert>

cui::ProgressBar::ProgressBar(int32_t length)
	: length(length), progress(0)
{
	style.beginChar = '[';
	style.endChar = ']';
	style.fillChar = '=';
	style.paddingChar = '.';
	style.seqChar = '>';
	assert(length > 3);
}

int32_t cui::ProgressBar::getWidth() const
{
	return length;
}

int32_t cui::ProgressBar::getHeight() const
{
	return 1;
}

std::vector<cui::String> cui::ProgressBar::getData() const
{
	cui::Bytes ret = "";
	ret += style.beginChar;
	int fillCount = (int)((double)progress * (double)(length - 2) / 100.0);
	for (int i = 0; i < length - 2; ++i) {
		if (i < fillCount) {
			ret += style.fillChar;
		}
		else if (i == fillCount) {
			ret += style.seqChar;
		}
		else {
			ret += style.paddingChar;
		}
	}
	ret += style.endChar;
	return { ret.data() };
}

void cui::ProgressBar::set(int progress)
{
	this->progress = progress;
}

int cui::ProgressBar::get() const
{
	return progress;
}

bool cui::ProgressBar::isDone() const
{
	return progress >= 100;
}

void cui::ProgressBar::setStyle(const ProgressStyle& style)
{
	this->style.beginChar = style.beginChar;
	this->style.endChar = style.endChar;
	this->style.fillChar = style.fillChar;
	this->style.paddingChar = style.paddingChar;
	this->style.seqChar = style.seqChar;
}