/* 
 * File:   FileLoader.h
 * Author: wouter
 *
 * Created on November 28, 2010, 10:34 AM
 */

#ifndef FILELOADER_H
#define	FILELOADER_H

typedef unsigned char byte;

class FileLoader
{
public:
	FileLoader();
	virtual ~FileLoader();

	bool loadBinaryFile(const char* filename, byte*& out, int& len);

	static const char* getExtension(const char* filename);
};

#endif	/* FILELOADER_H */

