/* 
 * File:   platform.h
 * Author: wouter
 *
 * Created on May 7, 2011, 12:14 PM
 */

#ifndef PLATFORM_H
#define	PLATFORM_H

#ifdef WIN32
#define DIRECTORY_SEPARATOR "\\"
#define DIRECTORY_SEPARATOR_ALTERNATIVE "/"
#else
#define DIRECTORY_SEPARATOR "/"
#define DIRECTORY_SEPARATOR_ALTERNATIVE "\\"
#endif

#endif	/* PLATFORM_H */

