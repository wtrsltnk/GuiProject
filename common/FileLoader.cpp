/* 
 * File:   FileLoader.cpp
 * Author: wouter
 * 
 * Created on November 28, 2010, 10:34 AM
 */

#include "FileLoader.h"
#include <stdio.h>
#include <string.h>

FileLoader::FileLoader()
{
}

FileLoader::~FileLoader()
{
}

bool FileLoader::loadBinaryFile(const char* filename, byte*& out, int& len)
{
	FILE* file = fopen(filename, "r");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		len = ftell(file);
		fseek(file, 0, SEEK_SET);
		out = new byte[len];
		fread(out, 1, len, file);
		fclose(file);
		return true;
	}
	return false;
}

const char* FileLoader::getExtension(const char* filename)
{
	return strrchr(filename, '.');
}
