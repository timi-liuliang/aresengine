#pragma once

#include <QtGui/QApplication.h>
#include <qlabel.h>
#include <qpixmap.h>

namespace Ares
{
	//--------------------------------------
	// Tga Header 2012-4-18 帝林
	//--------------------------------------
	#pragma pack(push, 1)
	struct TgaHeader
	{
		char  identsize;          // size of ID field that follows 18 byte header (0 usually)
		char  colourmaptype;      // type of colour map 0=none, 1=has palette
		char  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
		
		short colourmapstart;     // first colour map entry in palette
		short colourmaplength;    // number of colours in palette
		char  colourmapbits;      // number of bits per palette entry 15,16,24,32
	
		short xstart;             // image x origin
		short ystart;             // image y origin

		short width;              // image width in pixels
		short height;             // image height in pixels

		char  bits;               // image bits per pixel 8,16,24,32
		char  descriptor;         // image descriptor bits (vh flip bits)
	};
	#pragma pack(pop)
	
	//--------------------------------------
	// 增加对tga图形文件支持 2012-4-18 帝林
	//--------------------------------------							
	QPixmap LoadTga(const char *filename);
}