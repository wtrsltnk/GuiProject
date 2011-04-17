/* 
 * File:   TextureLoader.h
 * Author: wouter
 *
 * Created on November 27, 2010, 3:45 PM
 */

#ifndef TEXTURELOADER_H
#define	TEXTURELOADER_H

#include "Texture.h"
#include "FileLoader.h"

class TextureLoader : public FileLoader
{
public:
	TextureLoader();
	virtual ~TextureLoader();

	Texture* loadTexture(const char* filename);
	Texture* loadFromTga(const char* textureName);

	bool writeTGA(const char *file, const Texture* texture);
	
};

#endif	/* TEXTURELOADER_H */

