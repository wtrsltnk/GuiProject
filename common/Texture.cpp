/* 
 * File:   Texture.cpp
 * Author: wouter
 * 
 * Created on May 14, 2010, 2:22 PM
 */

#include "Texture.h"
#include <stdlib.h>
#include <memory.h>
#include "../GLee.h"

Texture::Texture()
	: mData(NULL), mWidth(0), mHeight(0), mBpp(0), mGlIndex(0)
{
	this->setName("");
}

Texture::Texture(const Texture& orig)
{
	(*this) = orig;
}

Texture::~Texture()
{
	if (this->mData != 0)
	{
		delete []this->mData;
		this->mData = 0;
	}
}

void Texture::setData(int w, int h, int bpp, unsigned char* data)
{
	int dataSize = w * h * bpp;

	if (dataSize > 0)
	{
		this->mWidth = w;
		this->mHeight = h;
		this->mBpp = bpp;

		if (this->mData != NULL)
			delete []this->mData;

		this->mData = new unsigned char[dataSize];
		if (data != 0)
			memcpy(this->mData, data, sizeof(unsigned char) * dataSize);
		else
			memset(this->mData, 0, dataSize);
	}
}

const Texture& Texture::operator = (const Texture& tex)
{
	this->setData(tex.mWidth, tex.mHeight, tex.mBpp, tex.mData);
	this->setName(tex.getName());
	return (*this);
}

unsigned int Texture::upload(bool repeat)
{
	int format = GL_RGB;

	// Determine format
	switch (this->mBpp)
	{
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
	}

	// Only upload when gl index == -1 (this means it's not uploaded yet)
	if (this->mGlIndex == 0)
	{
		glGenTextures(1, &this->mGlIndex);
		glBindTexture(GL_TEXTURE_2D, this->mGlIndex);

		if (repeat)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, format, this->mWidth, this->mHeight, 0, format, GL_UNSIGNED_BYTE, this->mData);
	}

	return this->mGlIndex;
}

void Texture::download()
{
	int format = GL_RGB;

	// Determine format
	switch (this->mBpp)
	{
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
	}

	glBindTexture(GL_TEXTURE_2D, this->mGlIndex);

	if (this->mData == 0)
		this->mData = new unsigned char[this->mWidth * this->mHeight * this->mBpp];

	glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)this->mData);
}

void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, this->mGlIndex);
}

const char* Texture::getName() const
{
	return this->mName;
}

void Texture::setName(const char* name)
{
	strcpy(this->mName, name);
}
