#include "stdafx.h"
#include "Segmentation.h"

void BinarizationBGCol(const CByteImage& imageIn, 
							 CByteImage& imageOut, int nThreshold)
{
	ASSERT(imageIn.GetChannel()==3); // ÄÃ·¯ ¿µ»ó È®ÀÎ

	nThreshold *= nThreshold; // ¹®ÅÎ°ª Á¦°ö
	//imageOut = imageIn;
	int nWidth  = imageIn.GetWidth();
	int nHeight = imageIn.GetHeight();

	int dB, dG, dR, dd;
	for (int r=0 ; r<nHeight ; r++)
	{
		BYTE* pIn	= imageIn.GetPtr(r);
		BYTE* pOut	= imageOut.GetPtr(r);

		int pos = 0;
		for (int c=0 ; c<nWidth ; c++)
		{
			dB = pIn[pos]; pos++;
			dG = pIn[pos]; pos++;
			dR = pIn[pos]; pos++;
			dd = dB*dB + dG*dG + dR*dR;
			if (dd > nThreshold) // Á¦°ö³¢¸® ºñ±³
				pOut[c] = 0;
			else
				pOut[c] = 255;
		}
	}
}