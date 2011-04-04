/* 
 * File:   Entity.h
 * Author: wouter
 *
 * Created on April 4, 2011, 1:15 PM
 */

#ifndef ENTITY_H
#define	ENTITY_H

#include "Brush.h"
#include <vector>
#include <map>
#include <string>

namespace geo
{

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void addBrush(Brush* brush);
	void addKeyValuePair( const std::string& key, const std::string& value);

	std::vector<Brush*> mBrushes;
	std::map<std::string, std::string> mKeyValuePairs;
};

}

#endif	/* ENTITY_H */

