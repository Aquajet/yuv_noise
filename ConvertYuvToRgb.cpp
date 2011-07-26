#include "ConvertYuvToRgb.h"

#define GETR(Y,Cr)     Clip(Y + m_yuvToRgb.kArrayR1[Cr])
#define GETG(Y,Cr,Cb)  Clip(Y - m_yuvToRgb.kArrayG1[Cb] - m_yuvToRgb.kArrayG2[Cr])
#define GETB(Y,Cb)     Clip(Y + m_yuvToRgb.kArrayB1[Cb])


CConvertYuvToRgb::CConvertYuvToRgb()
{
	InitRgbConvertArray();
}

CConvertYuvToRgb::~CConvertYuvToRgb()
{

}

void CConvertYuvToRgb::InitRgbConvertArray()
{
	int i;

	for(i=0;i<=255;i++)
	{
		m_yuvToRgb.kArrayR1[i] = (int)(1.402*(i-128));
		m_yuvToRgb.kArrayG1[i] = (int)(0.344*(i-128));
		m_yuvToRgb.kArrayG2[i] = (int)(0.714*(i-128));
		m_yuvToRgb.kArrayB1[i] = (int)(1.772*(i-128));
	}
}

BYTE CConvertYuvToRgb::Clip(int nValue)
{
	if(nValue>255)
		return 255;

	if(nValue<0)
		return 0;

	return (nValue*220)/256;
}


void CConvertYuvToRgb::Convert(DWORD dwWidth,DWORD dwHeight,DWORD dwYStride,DWORD dwUVStride,LPBYTE lpY,LPBYTE lpU,LPBYTE lpV,LPBYTE lpOutput,int rPos,int gPos,int bPos)
{
	int       i,j,k,m;
	int       nWidthUV;
	int       nHeightUV;
	int       nPosUV;
	int       nWidth;
	int       nWidth3;
	BYTE      kY;
	BYTE      kU;
	BYTE      kV;
	LPBYTE    lpInPos;

	if(dwYStride==0)
		dwYStride = dwWidth;

	if(dwUVStride==0)
		dwUVStride = dwWidth/2;

	lpInPos   = lpOutput;
	nWidthUV  = dwWidth/2;
	nHeightUV = dwHeight/2;
	nWidth    = dwWidth;
	nWidth3   = 3*dwWidth;
	nPosUV    = 0;
	k         = 0;
	m         = 0;

	for(j=0;j<nHeightUV;j++)
	{
		for(i=0;i<nWidthUV;i++)
		{
			kV = lpV[nPosUV];
			kU = lpU[nPosUV];

			kY = lpY[k];
			lpInPos[m+bPos] = GETB(kY,kV);
			lpInPos[m+gPos] = GETG(kY,kU,kV);
			lpInPos[m+rPos] = GETR(kY,kU);

			kY = lpY[k+1];
			lpInPos[m+bPos+3] = GETB(kY,kV);
			lpInPos[m+gPos+3] = GETG(kY,kU,kV);
			lpInPos[m+rPos+3] = GETR(kY,kU);

			kY = lpY[k+dwYStride];
			lpInPos[m+nWidth3+bPos] = GETB(kY,kV);
			lpInPos[m+nWidth3+gPos] = GETG(kY,kU,kV);
			lpInPos[m+nWidth3+rPos] = GETR(kY,kU);

			kY = lpY[k+1+dwYStride];
			lpInPos[m+nWidth3+bPos+3] = GETB(kY,kV);
			lpInPos[m+nWidth3+gPos+3] = GETG(kY,kV,kV);
			lpInPos[m+nWidth3+rPos+3] = GETR(kY,kU);

			m += 6;
			k += 2;
			nPosUV++;
		}

		k      += ((dwYStride - nWidth) + dwYStride); //(dwYStride - nWidth) учитываем выравнивание в первой строке (которую прошли)
		nPosUV += (dwUVStride - nWidthUV); //Учитываем выравнивание, если оно было
		m      += nWidth3;
	}
}
