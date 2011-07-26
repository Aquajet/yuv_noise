#include <QImage>

#include "YUVImage.h"

// С википедии
// Y = 0.299 * R + 0.587 * G + 0.114 * B;
// U = -0.14713 * R - 0.28886 * G + 0.436 * B;
// V = 0.615 * R - 0.51499 * G - 0.10001 * B;

BYTE getY( QRgb rgb) { return  0.299*qRed(rgb)		+ 0.587*qGreen(rgb)		+ 0.114*qBlue(rgb); }
BYTE getU( QRgb rgb) { return  -0.14713*qRed(rgb)	- 0.28886*qGreen(rgb)	+ 0.436*qBlue(rgb); }
BYTE getV( QRgb rgb) { return  0.615*qRed(rgb)		- 0.51499*qGreen(rgb)	- 0.10001*qBlue(rgb); }

YUVImage::YUVImage()
{
	width_ = 0;
	height_ = 0;
	yStride_ = 0;
	uvStride_ = 0;
}

YUVImage::YUVImage( const QImage & image)
{
	init( image.width(), image.height() );

	int cx;
	int cy;

	QRgb pix;

	for ( cy = 0; cy < height_; ++cy )
	{
		for ( cx = 0; cx < width_; ++cx )
		{
			pix = image.pixel(cx, cy);
			y(cx,cy) = getY( pix );
			
			if ( (cy%2 == 0) && (cx%2 == 0) )
			{
				u(cx,cy) = getU( pix );
				v(cx,cy) = getV( pix );
			}
		}
	}
}


//void rgb2yuv420p(std::vector<BYTE>& rgb, std::vector<BYTE>& yuv420p)
//{
//  unsigned int i = 0;
//  unsigned int numpixels = width * height;
//  unsigned int ui = numpixels;
//  unsigned int vi = numpixels + numpixels / 4;
//  unsigned int s = 0;
// 
//#define sR (BYTE)(rgb[s+2])
//#define sG (BYTE)(rgb[s+1])
//#define sB (BYTE)(rgb[s+0])
// 
//  yuv420p.resize(numpixels * 3 / 2);
// 
//  for (int j = 0; j < height; j++)
//    for (int k = 0; k < width; k++)
//    {
//      yuv420p[i] = (BYTE)( (66*sR + 129*sG + 25*sB + 128) >> 8) + 16;
// 
//      if (0 == j%2 && 0 == k%2)
//      {
//        yuv420p[ui++] = (BYTE)( (-38*sR - 74*sG + 112*sB + 128) >> 8) + 128;
//        yuv420p[vi++] = (BYTE)( (112*sR - 94*sG - 18*sB + 128) >> 8) + 128;
//      }
//      i++;
//      s += colors;
//    }
//}


void YUVImage::init (int _width, int _height)
{
	width_ = _width; 
	height_ = _height;

	int size = width_*height_;
	uvWidth_ = ((double)width_ / 2 + 0.5);
	uvHeight_ = ((double)height_ / 2 + 0.5);
	int halfSize = uvWidth_*uvHeight_;

	y_.resize( size);
	u_.resize( halfSize);
	v_.resize( halfSize);
}
