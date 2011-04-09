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
	MemoryGuard<Brush> b;
	b.data = new Brush();

	while (tok.nextToken() && strcmp(tok.getToken(), "}") != 0)
	{
		Vector3 v1, v2, v3;
		if (tok.nextToken() == false) return false;	// Skip the "("
		v1.x(atoi(tok.getToken())); if (tok.nextToken() == false) return false;
		v1.y(atoi(tok.getToken())); if (tok.nextToken() == false) return false;
		v1.z(atoi(tok.getToken())); if (tok.nextToken() == false) return false;
		if (tok.nextToken() == false) return false;	// Skip the ")"

		if (tok.nextToken() == false) return false;	// Skip the "("
		v2.x(atoi(tok.getToken())); if (tok.nextToken() == false) return false;
		v2.y(atoi(tok.getToken())); if (tok.nextToken() == false) return false;
		v2.z(atoi(tok.getToken())); if (tok.nextToken() == false) return false;
		if (tok.nextToken() == false) return false;	// Skip the ")"

		if (tok.nextToken() == false) return false;	// Skip the "("
		v3.x(atoi(tok.getToken())); if (tok.nextToken() == false) return false;
		v3.y(atoi(tok.getToken())); if (tok.nextToken() == false) return false;
		v3.z(atoi(tok.getToken())); if (tok.nextToken() == false) return false;
		if (tok.nextToken() == false) return false;	// Skip the ")"

		if (tok.nextToken() == false) return false;	// Texture name
		if (tok.nextToken() == false) return false;	// x_off     - Texture x-offset (must be multiple of 16)
		if (tok.nextToken() == false) return false;	// y_off     - Texture y-offset (must be multiple of 16)
		if (tok.nextToken() == false) return false;	// rot_angle - floating point value indicating texture rotation
		if (tok.nextToken() == false) return false;	// x_scale   - scales x-dimension of texture (negative value to flip)Te
//		if (tok.nextToken() == false) return false;	// y_scale   - scales y-dimension of texture (negative value to flip)

		Plane p = Plane::fromVertices(v1, v2, v3);
		b.data->addPlane(p);
	}

	b.data->updateVertices();
	entity->addBrush(b.data);
	b.data = 0;
	
	return strcmp(tok.getToken(), "}") == 0;
}

}
