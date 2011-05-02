/* 
 * File:   Texture.h
 * Author: wouter
 *
 * Created on May 14, 2010, 2:22 PM
 */

#ifndef _TEXTURE_H
#define	_TEXTURE_H

class Texture
{
public:
	Texture();
	Texture(const Texture& orig);
	virtual ~Texture();

	const Texture& operator = (const Texture& tex);
	void setData(int w, int h, int bpp, unsigned char* data = 0);

	unsigned int upload(bool repeat = true);
	void download();
	virtual void use();
	
	void flipHorizontally();
	void flipVertically();

	const char* getName() const;
	void setName(const char* name);
	
	char mName[256];
	unsigned char* mData;
	int mWidth;
	int mHeight;
	int mBpp;
	unsigned int mGlIndex;

};

#endif	/* _TEXTURE_H */

