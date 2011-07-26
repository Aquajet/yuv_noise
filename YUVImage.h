#pragma once
#include <QVector>
#include "types.h"

class QImage;

class YUVImage
{
private:
	int width_;
	int height_;
	int uvWidth_;
	int uvHeight_;
	int yStride_;
	int uvStride_;

	QVector<BYTE> y_;
	QVector<BYTE> u_;
	QVector<BYTE> v_;

	void init (int _width, int _height);

public:

	LPBYTE y() { return y_.data(); }
	LPBYTE u() { return u_.data(); }
	LPBYTE v() { return v_.data(); }

	inline BYTE & y( int x, int y) { return y_[y*width_ + x]; }
	inline BYTE & u( int x, int y) { return u_[(y/2)*uvWidth_ + x/2]; }
	inline BYTE & v( int x, int y) { return v_[(y/2)*uvWidth_ + x/2]; }
	
	int width() const { return width_; }
	int height() const { return height_; }

	YUVImage();
	YUVImage( const QImage &);
};
