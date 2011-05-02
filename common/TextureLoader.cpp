/* 
 * File:   TextureLoader.cpp
 * Author: wouter
 * 
 * Created on November 27, 2010, 3:45 PM
 */

#include "TextureLoader.h"
#include <stdio.h>
#include <string.h>

TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{
}

Texture* TextureLoader::loadTexture(const char* filename)
{
	const char* ext = FileLoader::getExtension(filename);

	if (strcmp(ext, ".tga") == 0)
		return this->loadFromTga(filename);

	return 0;
}

namespace TGA
{
	#pragma pack(1)

	typedef struct sTGAHeader
	{
	   unsigned char idLength;
	   unsigned char colormapType;
	   unsigned char imageType;
	   unsigned short colormapIndex;
	   unsigned short colormapLength;
	   unsigned char colormapSize;
	   unsigned short xOrigin, yOrigin;
	   unsigned short width, height;
	   unsigned char pixelSize;
	   unsigned char attributes;

	} tTGAHeader;

	#pragma pack()

	namespace TgaDataType
	{
		enum
		{
			NoImageData = 0,							// No image data included.
			UncompressedColorMapped = 1,				// Uncompressed, color-mapped images.
			UncompressedRGB = 2,						// Uncompressed, RGB images.
			UncompressedBlackWhite = 3,					// Uncompressed, black and white images.
			RunlengthEncodedColorMapped = 9,			// Runlength encoded color-mapped images.
			RunlengthEncodedRGB = 10,					// Runlength encoded RGB images.
			CompressedBlackWhite = 11,					// Compressed, black and white images.
			CompressedColorMapped = 32,					// Compressed color-mapped data, using Huffman, Delta, and runlength encoding.
			CompressedColorMapped4PassQuadtree = 33		// Compressed color-mapped data, using Huffman, Delta, and runlength encoding.  4-pass quadtree-type process.
		};
	}
}

Texture* TextureLoader::loadFromTga(const char* textureName)
{
	Texture* result = 0;
	byte* data;
	int len;
	if (this->loadBinaryFile(textureName, data, len) == false)
	{
#ifdef SHOW_DEBUG_TEXT
		printf("Could not load TGA texture %s\n\n", textureName);
#endif
		return 0;
	}

	TGA::tTGAHeader* header = (TGA::tTGAHeader*)data;

	if (header->imageType == TGA::TgaDataType::UncompressedRGB)
	{
		int bpp = header->pixelSize >> 3;
		int datasize = header->width * header->height * bpp;
		byte* pixel = new byte[datasize];

		memcpy(pixel, data + sizeof(TGA::tTGAHeader) + header->idLength, datasize);
		for (unsigned int c = 0; c < datasize; c += bpp)
		{
			unsigned char t = pixel[c];
			pixel[c] = pixel[c+2];
			pixel[c+2] = t;
		}
		
		result = new Texture();
		result->setData(header->width, header->height, bpp, pixel);
		delete []pixel;
		
		if ((header->attributes & (1 << 5)) == 0)
			result->flipHorizontally();
		if ((header->attributes & (1 << 4)) == 0)
			result->flipVertically();
	}
	delete []data;
	return result;
}

bool TextureLoader::writeTGA(const char *file, const Texture* texture)
{
	// To save a screen shot is just like reading in a image.  All you do
	// is the opposite.  Istead of calling fread to read in data you call
	// fwrite to save it.
	FILE *pFile;
	// The file pointer.
	unsigned char uselessChar;
	// used for useless char.
	short int uselessInt;
	// used for useless int.
	unsigned char imageType;
	// Type of image we are saving.
	int index;
	// used with the for loop.
	unsigned char bits;
	// Bit depth.
	long Size;
	// Size of the picture.
	int colorMode;
	unsigned char tempColors;
	unsigned char* data;

	pFile = fopen(file, "wb");

	if(!pFile)
	{
		fclose(pFile);
		return false;
	}
	// Set the image type, the color mode, and the bit depth.
	imageType = 2;
	colorMode = texture->mBpp;
	bits = texture->mBpp == 3 ? 24 : 32;

	int dataSize = texture->mWidth*texture->mHeight*texture->mBpp;
	data = new unsigned char[dataSize];
	memcpy(data, texture->mData, dataSize);
	
	// Set these two to 0.
	uselessChar = 0;
	uselessInt = 0;
	// Write useless data.
	fwrite(&uselessChar, sizeof(unsigned char), 1, pFile);
	fwrite(&uselessChar, sizeof(unsigned char), 1, pFile);
	// Now image type.
	fwrite(&imageType, sizeof(unsigned char), 1, pFile);
	// Write useless data.
	fwrite(&uselessInt, sizeof(short int), 1, pFile);
	fwrite(&uselessInt, sizeof(short int), 1, pFile);
	fwrite(&uselessChar, sizeof(unsigned char), 1, pFile);
	fwrite(&uselessInt, sizeof(short int), 1, pFile);
	fwrite(&uselessInt, sizeof(short int), 1, pFile);
	// Write the size that you want.
	fwrite(&texture->mWidth, sizeof(short int), 1, pFile);
	fwrite(&texture->mHeight, sizeof(short int), 1, pFile);
	fwrite(&bits, sizeof(unsigned char), 1, pFile);
	// Write useless data.
	fwrite(&uselessChar, sizeof(unsigned char), 1, pFile);
	// Get image size.
	Size = texture->mWidth * texture->mHeight * colorMode;
	// Now switch image from RGB to BGR.
	for(index = 0; index < Size; index += colorMode)
	{
		tempColors = data[index];
		data[index] = data[index + 2];
		data[index + 2] = tempColors;
	}

	fwrite(data, sizeof(unsigned char), Size, pFile);
	delete []data;

	fclose(pFile);
	return true;
}
