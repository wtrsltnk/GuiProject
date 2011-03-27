/* 
 * File:   Clipper.h
 * Author: wouter
 *
 * Created on March 26, 2011, 9:23 AM
 */

#ifndef CLIPPER_H
#define	CLIPPER_H

class Clipper
{
public:
	Clipper(float hitbox[4]);
	virtual ~Clipper();

private:
	float hitbox[4];
	static int stack;

};

#endif	/* CLIPPER_H */

