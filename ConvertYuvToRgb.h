#ifndef _CONVERT_YUV_TO_RGB_H_
#define _CONVERT_YUV_TO_RGB_H_

#include "types.h"

class CConvertYuvToRgb
{
protected:
	struct YUV_TO_RGB
	{
		int   kArrayR1[256];
		int   kArrayG1[256];
		int   kArrayG2[256];
		int   kArrayB1[256];
	};

public:
	CConvertYuvToRgb();
	~CConvertYuvToRgb();

	//rPos,gPos, bPos - определение порядка RGB в пикселе (BGR, RGB и тд)
	//dwYStride - если в плоскости Y есть выравнивание (align) и реальная строка (Stride) больше чем Width, то Stride указывается. В противном случае 0
	//То же самое и для dwUVStride
	void Convert(
		DWORD dwWidth, DWORD dwHeight,
		DWORD dwYStride, DWORD dwUVStride,
		LPBYTE lpY, LPBYTE lpU, LPBYTE lpV,
		LPBYTE lpOutput,
		int rPos=2,int gPos=1,int bPos=0);

protected:
	void InitRgbConvertArray();
	inline BYTE Clip(int nValue);


protected:
   YUV_TO_RGB   m_yuvToRgb;

};

#endif //_CONVERT_YUV_TO_RGB_H_
