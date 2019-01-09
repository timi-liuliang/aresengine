#include "QLoadTga.h"

namespace Ares
{
	// ¼ÓÔØtga¸ñÊ½Í¼Æ¬
	QPixmap LoadTga(const char *filename)
	{
		// check if file has tga extension
		bool isTga = false;
		int len = strlen(filename);
		if(len >= 3)
		{
			char ext[3];
			ext[0] = tolower(filename[len - 3]);
			ext[1] = tolower(filename[len - 2]);
			ext[2] = tolower(filename[len - 1]);
			if( ext[0]=='t' && ext[1]=='g' && ext[2]=='a')
				isTga = true;
		}

		if(!isTga)
			return QPixmap(filename);

		// stupid Qt doesn't know how to load tga, so we'll load it by ourselves
		QPixmap pixmap;

		FILE *fp; // file streams would be more c++  oh well :D
		fp = fopen(filename, "rb");

		if(!fp)
			return pixmap;

		// read header
		TgaHeader header;
		fread(&header, sizeof(TgaHeader), 1, fp);

		// we only deal with rgb images, unmapped and rle, and no additional header information
		if(header.imagetype != 2 && header.imagetype != 10 && !header.identsize && !header.colourmaptype)
		{
			fclose(fp);

			return pixmap;
		}

		int bytesPerPixel = header.bits/8;
		int imageSize = header.width * header.height * bytesPerPixel;
		uchar *imageData = new uchar[imageSize];
		fread(imageData, imageSize, 1, fp);
		fclose(fp);

		// decode rle, if any
		if(header.imagetype == 10)
		{
			uchar *rleData = new uchar[imageSize];
			memcpy(rleData, imageData, imageSize);

			int i = 0;
			int r = 0;
			while( i<imageSize && r<imageSize)
			{
				int num = (rleData[r]&0x7F) + 1;
				if(rleData[r] > 127) // is rle
				{
					for(int k = 0; k < num; ++k)
					{
						if(r + 1 + bytesPerPixel >= imageSize || i >= imageSize)
							return pixmap; // corrupt image data!

						memcpy(&imageData[i], &rleData[r + 1], bytesPerPixel);
						i += bytesPerPixel;
					}

					r += bytesPerPixel + 1;
				}
				else
				{
					if(r + num * bytesPerPixel + 1 >= imageSize)
						return pixmap; // corrupt image data!

					memcpy(&imageData[i], &rleData[r + 1], num * bytesPerPixel);
					i += num * bytesPerPixel;
					r += num * bytesPerPixel + 1;
				}
			}

			delete[] rleData;
		}

		QImage::Format format;
		switch(header.bits)
		{
		case 16 : format = QImage::Format_RGB16; break;
		case 24 : format = QImage::Format_RGB888; break;
		case 32 : format = QImage::Format_ARGB32; break;
		default : delete[] imageData; return pixmap; // any other than 16, 24 or 32 would be a broken image
		};

		QImage image( imageData, header.width, header.height, format);

		// deal with the TGA flipping of axis
		// bit5 == 0 : flip
		// bit5 == 1 : don't flip
		image = image.mirrored( 0, !(header.descriptor&16)); // copies the image. performance loss, but who cares ;-)

		pixmap = QPixmap::fromImage(image);

		delete[] imageData;

		return pixmap;
	}
}