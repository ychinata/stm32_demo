#ifndef __FFT_H__
#define __FFT_H__


struct compx    //����һ�������ṹ��
{
  float real;
  float imag;
};
void BubbleSort(float pt[], int Cnt);
void FFT(struct compx *xin, int Num);

#endif      















