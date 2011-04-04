/* 
 * File:   Entity.cpp
 * Author: wouter
 * 
 * Created on April 4, 2011, 1:15 PM
 */

#include "Entity.h"

namespace geo
{

Entity::Entity()
{
}

Entity::~Entity()
{
	while (this->mBrushes.empty() == false)
	{
		Brush* tmp = this->mBrushes.back();
		this->mBrushes.pop_back();
		delete tmp;
	}
}

void Entity::addBrush(Brush* brush)
{
	this->mBrushes.push_back(brush);
}

void Entity::addKeyValuePair( const std::string& key, const std::string& value)
{
	this->mKeyValuePairs[key] = value;
}

}
