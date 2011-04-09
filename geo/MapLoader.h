/* 
 * File:   MapLoader.h
 * Author: wouter
 *
 * Created on April 4, 2011, 12:51 PM
 */

#ifndef MAPLOADER_H
#define	MAPLOADER_H

#include "../common/Tokenizer.h"
#include "Scene.h"

namespace geo
{

class MapLoader
{
public:
	MapLoader();
	virtual ~MapLoader();

	bool load(const char* filename, Scene* scene);

private:
	bool loadEntity(common::Tokenizer& tok, Scene* scene);
	bool loadBrush(common::Tokenizer& tok, Entity* entity);

	int mVersion;
};

}

#endif	/* MAPLOADER_H */

