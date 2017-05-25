#pragma once

#include "MyImage.h"

#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#define MAX(a,b)  ((a) < (b) ? (b) : (a))

int RGB2HSV(const CByteImage& imageIn , CByteImage& imageOut);

template <typename T>
CMyImage<T> Gray2RGB(const CMyImage<T>& src)
{
	ASSERT(src.GetChannel() == 1);

	int nWidth  = src.GetWidth();
	int nHeight = src.GetHeight();

	CMyImage<T> dst(nWidth, nHeight, 3);

	for (int r=0 ; r<nHeight ; r++)
	{
		T* pSrc = src.GetPtr(r);
		T* pDst = dst.GetPtr(r);

		int pos = 0;
		for (int c=0 ; c<nWidth ; c++)
		{
			pDst[pos  ] = pSrc[c];
			pDst[pos+1] = pSrc[c];
			pDst[pos+2] = pSrc[c];
			pos += 3;
		}
	}

	return dst;
}