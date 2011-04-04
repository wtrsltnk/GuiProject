/* 
 * File:   tokenizer.cpp
 * Author: wouter
 * 
 * Created on August 6, 2010, 3:37 PM
 */

#include "Tokenizer.h"
#include <stdlib.h>
#include <string.h>

namespace common
{

Tokenizer::Tokenizer(const char* data, int size)
	: data(data), dataSize(size), cursor(0), token(0)
{
}

Tokenizer::Tokenizer(const Tokenizer& orig)
	: data(orig.data), dataSize(orig.dataSize), cursor(0), token(0)
{
}

Tokenizer::~Tokenizer()
{
	if (this->token)
		delete []this->token;
}

const char* Tokenizer::getToken()
{
	return this->token;
}

const char* Tokenizer::getNextToken()
{
	if (this->nextToken())
		return this->getToken();

	return 0;
}

bool Tokenizer::nextToken()
{
	// Check if we are at the end
	if (cursor >= this->dataSize)
		return false;
	
	// Reset the token
	if (this->token != 0)
		delete []this->token;
	this->token = 0;

	// Trim to the token
	while (Tokenizer::isSeperator(data[cursor]))
		cursor++;

	int c = 0;

	// Are we at a quoted token?
	if (Tokenizer::isQuote(data[cursor]))
	{
		while (data[cursor] != data[cursor + c + 1] && data[cursor + c + 1] != 0)
			c++;
		this->token = new char[c + 2];
		memcpy(this->token, data + cursor + 1, c);
		this->token[c] = 0;
		cursor+= c + 2;
	}
	else
	{
		while (Tokenizer::isSeperator(data[cursor + c]) == false)
			c++;
		this->token = new char[c + 2];
		memcpy(this->token, data + cursor, c);
		this->token[c] = 0;
		cursor+= c;
	}

	return true;
}

bool Tokenizer::isSeperator(char c)
{
	if (c <= ' ') return true;

	return false;
}

bool Tokenizer::isQuote(char c)
{
	if (c == '\"') return true;
	if (c == '\'') return true;

	return false;
}

}
