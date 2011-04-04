/* 
 * File:   Scene.h
 * Author: wouter
 *
 * Created on April 4, 2011, 12:52 PM
 */

#ifndef SCENE_H
#define	SCENE_H

#include "Entity.h"
#include <vector>

namespace geo
{

class Scene
{
public:
	Scene();
	virtual ~Scene();

	void addEntity(Entity* entity);
	
	std::vector<Entity*> mEntities;

};

}

#endif	/* SCENE_H */

