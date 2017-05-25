#include "stdafx.h"
#include "MyImageFunc.h"


int RGB2HSV(const CByteImage& imageIn , CByteImage& imageOut)
{
   ASSERT(imageIn.GetChannel() == 3);

   int nWidth  = imageIn.GetWidth();
   int nHeight = imageIn.GetHeight();
   
   imageOut = imageIn;

   
   int sum = 0;


   /****************색 찾는 변수들********************/
   int threshHold = 500;

   int red = 0;
   int yellow = 0;
   int blue = 0;
   int green = 0;
   int maxColor;
   /****************************************************/
   
   double vR, vG, vB;
   double vMin, vMax, delta, H ,S ,V;
   for (int r=0 ; r<nHeight ; r++)
   {
      //TB*      pSrc = src.GetPtr(r);
      //double* pDst = dst.GetPtr(r);
      BYTE* pIn = imageIn.GetPtr(r);
      BYTE* pOut = imageOut.GetPtr(r);

      int pos = 0;
      for (int c=0 ; c<nWidth ; c++)
      {
         sum =( pIn[pos] - '0') + 48;
         vB = (double)sum / 255.0;
      
         sum = (pIn[pos+1] - '0') + 48;
         vG = (double)sum / 255.0;
      
         sum = (pIn[pos+2] - '0') + 48;
         vR = (double)sum / 255.0;
      
         
         vMax = MAX(MAX(vR, vG), vB);
         vMin = MIN(MIN(vR, vG), vB);
         delta = vMax - vMin;
         V = vMax; // V

         if (delta==0) // GrayScale
         {
            S = 0;
            H = 0;
         }
         else 
         {
            S = delta / vMax; // S

            if (vR==vMax)
				H = (vG-vB)/delta; // 노란색과 자홍색 사이
            else if (vG==vMax)
				H = 2 + (vB-vR)/delta; // 하늘색과 노란색 사이
            else
				H = 4 + (vR-vG)/delta; // 자홍색과 하늘색 사이

				H *= 60.0;
            if (H<0)
				H += 360.0; // H
         }

		if ((H >356 || H < 8.4) && ((S>0.63) && (S<0.88)) && (V>0.38)) //빨강
			red++; 
		if ((H > 213 && H < 223) && (S>0.4) && (S < 6) && (V<0.52) && (V>0.4)) // 파랑
			blue++;
		if ((H >75 && H < 88) && ((S>0.45) && (S<0.8)) && (V>0.57)) // 초록
			green++;
		if ((H > 52 && H < 58) && ((S>0.65) && (S<0.8)) && (V>0.5)) // 노랑
			yellow++;
   
         pos += 3;
      }
   }

   maxColor = MAX((MAX((MAX(red, blue)), yellow)), green); //max값 찾기

   //빨강
   if (maxColor == red){
      for (int r = 0; r<nHeight; r++)
      {
         //TB*      pSrc = src.GetPtr(r);
         //double* pDst = dst.GetPtr(r);
         BYTE* pIn = imageIn.GetPtr(r);
         BYTE* pOut = imageOut.GetPtr(r);

        int pos = 0;
         for (int c = 0; c<nWidth; c++)
         {
            sum = (pIn[pos] - '0') + 48;
            vB = (double)sum / 255.0;



            sum = (pIn[pos + 1] - '0') + 48;
            vG = (double)sum / 255.0;

            sum = (pIn[pos + 2] - '0') + 48;
            vR = (double)sum / 255.0;


            vMax = MAX(MAX(vR, vG), vB);
            vMin = MIN(MIN(vR, vG), vB);
            delta = vMax - vMin;
            V = vMax;            // V

            if (delta == 0)               // Gray
            {
               S = 0;
               H = 0;
            }
            else
            {
               S = delta / vMax;   // S

               if (vR == vMax)
                  H = (vG - vB) / delta;      // 노란색과 자홍색 사이
               else if (vG == vMax)
                  H = 2 + (vB - vR) / delta;   // 하늘색과 노란색 사이
               else
                  H = 4 + (vR - vG) / delta;   // 자홍색과 하늘색 사이

               H *= 60.0;
               if (H<0)
                  H += 360.0;
               // H
            }

            if ((H >356 || H < 8.4) && ((S>0.63) && (S<0.88)) && (V>0.38)){  //빨강
               pOut[pos] = 0;
               pOut[pos + 1] = 0;
               pOut[pos + 2] = 0;
               

            }
            else{
               pOut[pos] = 255;
               pOut[pos + 1] = 255;
               pOut[pos + 2] = 255;

            }

            pos += 3;
         }
      }

      return 1;
   }

   if (maxColor == yellow){
      for (int r = 0; r<nHeight; r++)
      {
         //TB*      pSrc = src.GetPtr(r);
         //double* pDst = dst.GetPtr(r);
         BYTE* pIn = imageIn.GetPtr(r);
         BYTE* pOut = imageOut.GetPtr(r);

         int pos = 0;
         for (int c = 0; c<nWidth; c++)
         {
            sum = (pIn[pos] - '0') + 48;
            vB = (double)sum / 255.0;



            sum = (pIn[pos + 1] - '0') + 48;
            vG = (double)sum / 255.0;

            sum = (pIn[pos + 2] - '0') + 48;
            vR = (double)sum / 255.0;


            vMax = MAX(MAX(vR, vG), vB);
            vMin = MIN(MIN(vR, vG), vB);
            delta = vMax - vMin;
            V = vMax;            // V

            if (delta == 0)               // Gray
            {
               S = 0;
               H = 0;
            }
            else
            {
               S = delta / vMax;   // S

               if (vR == vMax)
                  H = (vG - vB) / delta;      // 노란색과 자홍색 사이
               else if (vG == vMax)
                  H = 2 + (vB - vR) / delta;   // 하늘색과 노란색 사이
               else
                  H = 4 + (vR - vG) / delta;   // 자홍색과 하늘색 사이

               H *= 60.0;
               if (H<0)
                  H += 360.0;
               // H
            }

            if ((H >52 && H < 58) && ((S>0.65) && (S<0.8)) && (V>0.5)){  // 노랑
               pOut[pos] = 0;
               pOut[pos + 1] = 0;
               pOut[pos + 2] = 0;
               

            }
            else{
               pOut[pos] = 255;
               pOut[pos + 1] = 255;
               pOut[pos + 2] = 255;

            }

            pos += 3;
         }
      }

      return 2;

   }
   if (maxColor == green){
      for (int r = 0; r<nHeight; r++)
      {
         //TB*      pSrc = src.GetPtr(r);
         //double* pDst = dst.GetPtr(r);
         BYTE* pIn = imageIn.GetPtr(r);
         BYTE* pOut = imageOut.GetPtr(r);

         int pos = 0;
         for (int c = 0; c<nWidth; c++)
         {
            sum = (pIn[pos] - '0') + 48;
            vB = (double)sum / 255.0;



            sum = (pIn[pos + 1] - '0') + 48;
            vG = (double)sum / 255.0;

            sum = (pIn[pos + 2] - '0') + 48;
            vR = (double)sum / 255.0;


            vMax = MAX(MAX(vR, vG), vB);
            vMin = MIN(MIN(vR, vG), vB);
            delta = vMax - vMin;
            V = vMax;            // V

            if (delta == 0)               // Gray
            {
               S = 0;
               H = 0;
            }
            else
            {
               S = delta / vMax;   // S

               if (vR == vMax)
                  H = (vG - vB) / delta;      // 노란색과 자홍색 사이
               else if (vG == vMax)
                  H = 2 + (vB - vR) / delta;   // 하늘색과 노란색 사이
               else
                  H = 4 + (vR - vG) / delta;   // 자홍색과 하늘색 사이

               H *= 60.0;
               if (H<0)
                  H += 360.0;
               // H
            }
            if ((H >75 && H < 88) && ((S>0.45) && (S<0.8)) && (V>0.57)){  // 초록
               pOut[pos] = 0;
               pOut[pos + 1] = 0;
               pOut[pos + 2] = 0;
               

            }
            else{
               pOut[pos] = 255;
               pOut[pos + 1] = 255;
               pOut[pos + 2] = 255;

            }

            pos += 3;
         }
      }

      return 3;

   }

   if (maxColor == blue){
      for (int r = 0; r<nHeight; r++)
      {
         //TB*      pSrc = src.GetPtr(r);
         //double* pDst = dst.GetPtr(r);
         BYTE* pIn = imageIn.GetPtr(r);
         BYTE* pOut = imageOut.GetPtr(r);

         int pos = 0;
         for (int c = 0; c<nWidth; c++)
         {
            sum = (pIn[pos] - '0') + 48;
            vB = (double)sum / 255.0;



            sum = (pIn[pos + 1] - '0') + 48;
            vG = (double)sum / 255.0;

            sum = (pIn[pos + 2] - '0') + 48;
            vR = (double)sum / 255.0;


            vMax = MAX(MAX(vR, vG), vB);
            vMin = MIN(MIN(vR, vG), vB);
            delta = vMax - vMin;
            V = vMax;            // V

            if (delta == 0)               // Gray
            {
               S = 0;
               H = 0;
            }
            else
            {
               S = delta / vMax;   // S

               if (vR == vMax)
                  H = (vG - vB) / delta;      // 노란색과 자홍색 사이
               else if (vG == vMax)
                  H = 2 + (vB - vR) / delta;   // 하늘색과 노란색 사이
               else
                  H = 4 + (vR - vG) / delta;   // 자홍색과 하늘색 사이

               H *= 60.0;
               if (H<0)
                  H += 360.0;
               // H
            }

            if ((H >213 && H < 223) && (S>0.4) && (S<6) && (V<0.52) && (V>0.4)){ // 파랑
              
               pOut[pos] = 0;
               pOut[pos + 1] = 0;
               pOut[pos + 2] = 0;
               

            }
            else{
               pOut[pos] = 255;
               pOut[pos + 1] = 255;
               pOut[pos + 2] = 255;

            }

            pos += 3;
         }
      }

      return 4;

   }
   return 0;
}