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
		printf("Could not load TGA texture %s\n\n", textureName);
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
	}
	delete []data;
	return result;
}
