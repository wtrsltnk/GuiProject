/* 
 * File:   Scene.cpp
 * Author: wouter
 * 
 * Created on April 4, 2011, 12:52 PM
 */

#include "Scene.h"

namespace geo
{

Scene::Scene()
{
}

Scene::~Scene()
{
	while (this->mEntities.empty() == false)
	{
		Entity* tmp = this->mEntities.back();
		this->mEntities.pop_back();
		delete tmp;
	}
}

void Scene::addEntity(Entity* entity)
{
	this->mEntities.push_back(entity);
}

}
