/* 
 * File:   MapLoader.cpp
 * Author: wouter
 * 
 * Created on April 4, 2011, 12:51 PM
 */

#include "MapLoader.h"
#include "../common/Tokenizer.h"
#include "../common/MemoryGuard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace geo
{

MapLoader::MapLoader()
{
}

MapLoader::~MapLoader()
{
}

bool MapLoader::load(const char* filename, Scene* scene)
{
	printf("load\n");
	MemoryGuard<char> data;
	FILE* file = fopen(filename, "rb");
	if (file != false)
	{
		fseek(file, 0, SEEK_END);
		data.size = ftell(file);
		fseek(file, 0, SEEK_SET);
		if (data.size > 0)
		{
			data.data = new char[data.size];
			fread(data.data, 1, data.size, file);
		}
		fclose(file);
	}
	if (data.data != 0 && data.size > 0)
	{
		common::Tokenizer tok(data.data, data.size);

		// Do load
		while (tok.nextToken())
		{
			if (strcmp(tok.getToken(), "{") == 0)
			{
				if (this->loadEntity(tok, scene) == false)
					return false;
			}
		}

		return true;
	}
	return false;
}

bool MapLoader::loadEntity(common::Tokenizer& tok, Scene* scene)
{
	printf("loadEntity\n");
	MemoryGuard<Entity> e;
	e.data = new Entity();

	while (tok.nextToken() && strcmp(tok.getToken(), "}") != 0)
	{
		if (strcmp(tok.getToken(), "{") == 0)
		{
			if (this->loadBrush(tok, e.data) == false)
				return false;
		}
		else
		{
			std::string key(tok.getToken());
			if (tok.nextToken() == false)
				return false;
			std::string value(tok.getToken());
			e.data->addKeyValuePair(key, value);
		}
	}
	
	scene->addEntity(e.data);
	e.data = 0;

	return strcmp(tok.getToken(), "}") == 0;
}

bool MapLoader::loadBrush(common::Tokenizer& tok, Entity* entity)
{
	printf("loadBrush\n");
	MemoryGuard<Brush> b;
	b.data = new Brush();

	while (tok.nextToken() && strcmp(tok.getToken(), "}") != 0)
	{
		Plane p;
		if (tok.nextToken() == false) return false;	// Skip the "("
		p.mBase[0].mXyz[0] = atoi(tok.getToken()); if (tok.nextToken() == false) return false;
		p.mBase[0].mXyz[1] = atoi(tok.getToken()); if (tok.nextToken() == false) return false;
		p.mBase[0].mXyz[2] = atoi(tok.getToken()); if (tok.nextToken() == false) return false;
		if (tok.nextToken() == false) return false;	// Skip the ")"

		if (tok.nextToken() == false) return false;	// Skip the "("
		p.mBase[1].mXyz[0] = atoi(tok.getToken()); if (tok.nextToken() == false) return false;
		p.mBase[1].mXyz[1] = atoi(tok.getToken()); if (tok.nextToken() == false) return false;
		p.mBase[1].mXyz[2] = atoi(tok.getToken()); if (tok.nextToken() == false) return false;
		if (tok.nextToken() == false) return false;	// Skip the ")"

		if (tok.nextToken() == false) return false;	// Skip the "("
		p.mBase[2].mXyz[0] = atoi(tok.getToken()); if (tok.nextToken() == false) return false;
		p.mBase[2].mXyz[1] = atoi(tok.getToken()); if (tok.nextToken() == false) return false;
		p.mBase[2].mXyz[2] = atoi(tok.getToken()); if (tok.nextToken() == false) return false;
		if (tok.nextToken() == false) return false;	// Skip the ")"

		 if (tok.nextToken() == false) return false;	// Texture name
		 if (tok.nextToken() == false) return false;	// x_off     - Texture x-offset (must be multiple of 16)
		 if (tok.nextToken() == false) return false;	// y_off     - Texture y-offset (must be multiple of 16)
		 if (tok.nextToken() == false) return false;	// rot_angle - floating point value indicating texture rotation
		 if (tok.nextToken() == false) return false;	// x_scale   - scales x-dimension of texture (negative value to flip)Te
//		 if (tok.nextToken() == false) return false;	// y_scale   - scales y-dimension of texture (negative value to flip)

		b.data->addPlane(p);
	}
	
	entity->addBrush(b.data);
	b.data = 0;
	
	return strcmp(tok.getToken(), "}") == 0;
}

}
