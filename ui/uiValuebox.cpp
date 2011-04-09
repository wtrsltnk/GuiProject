#include "uiControls.h"
#include "uiFont.h"
#include "uiClipper.h"

namespace ui
{

Valuebox::Valuebox(int x, int y, int w, int h, float value, float min, float max)
	: Control(ControlTypes::Valuebox, x, y, w, h), mValue(value), mMinValue(min), mMaxValue(max)
{
	this->mInput[0] = 0;
}

Valuebox::~Valuebox()
{
}

void Valuebox::render()
{
	this->renderBox(true);

	char str[32] = { 0 };
	if (this->mInput[0] == 0)
		sprintf(str, "%0.4f", this->mValue);
	else
		strcpy(str, this->mInput);

	int length = this->mBox.font->getTextLength(str);
	int height = this->mBox.font->getTextHeight("W");

	Control::renderText(this->mBox.boxPosition[0]+this->mBox.boxSize[0]/2.0f - float(length)/2.0f,
			this->mBox.boxPosition[1]+this->mBox.boxSize[1]/2.0f - float(height) / 4.0f-4,
			str, RGBA(255, 255, 255, 255));
}

void Valuebox::keyDown(Key::Code key)
{
	float diff = (this->maxValue() - this->minValue()) / 10.0f;
	if (key == Key::Left || key == Key::Down)
		this->setValue(this->value() - diff);
	else if (key == Key::Right || key == Key::Up)
		this->setValue(this->value() + diff);
	else if (key >= Key::Num0 && key < Key::Num9)
	{
		this->addInput('a' + (key - Key::A));
		Control::keyDown(key);
	}
}

float Valuebox::value()
{
	return this->mValue;
}

void Valuebox::setValue(float value)
{
	this->mValue = value;
	if (this->mValue < this->mMinValue) this->mValue = this->mMinValue;
	if (this->mValue > this->mMaxValue) this->mValue = this->mMaxValue;
}

float Valuebox::minValue()
{
	return this->mMinValue;
}

void Valuebox::setMinValue(float min)
{
	this->mMinValue = min;
}

float Valuebox::maxValue()
{
	return this->mMaxValue;
}

void Valuebox::setMaxValue(float max)
{
	this->mMaxValue = max;
}

void Valuebox::addInput(char key)
{
	int i = 0;
	while (this->mInput[i] != 0 && i < 31)
		i++;
	if (key == 8)
	{
		this->mInput[i-1] = '\0';
	}
	else if ((key >= '0' && key <= '9') || key == '.')
	{
		this->mInput[i] = key;
		this->mInput[i+1] = 0;
	}
	else if (key == 13)
	{
		float val = atof(this->mInput);
		this->setValue(val);
		this->mInput[0] = 0;
	}
}

}
