/* 
 * File:   MemoryGuard.h
 * Author: wouter
 *
 * Created on April 4, 2011, 1:06 PM
 */

#ifndef MEMORYGUARD_H
#define	MEMORYGUARD_H

template <typename T>
class MemoryGuard
{
public:
	MemoryGuard(int size = 1) : data(0), size(size) {  }
	virtual ~MemoryGuard()
	{
		if (this->data != 0)
		{
			if (this->size > 1)
				delete []this->data;
			else
				delete this->data;
		}
	}

	T* data;
	int size;

};

#endif	/* MEMORYGUARD_H */

