#ifndef __FFT_H__
#define __FFT_H__


struct compx    //定义一个复数结构体
{
  float real;
  float imag;
};
void BubbleSort(float pt[], int Cnt);
void FFT(struct compx *xin, int Num);

#endif      















