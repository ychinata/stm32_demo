#include "fft.h"
#include "stdio.h"
#include "math.h"


// 冒泡法排序函数
void BubbleSort(float pt[], int Cnt)
{
	int     k      = 0;
        float temp = 0;
	while (Cnt > 0)
	{
		for (k=0; k<Cnt-1; k++)
		{
			if (pt[k] < pt[k+1])
			{
				temp    = pt[k];
				pt[k]   = pt[k+1];
				pt[k+1] = temp;
			}
		}
		Cnt--;
	}
}

//复数乘法
struct compx EE(struct compx b1, struct compx b2)
{
	struct compx b3;
	b3.real = b1.real*b2.real - b1.imag*b2.imag;
	b3.imag = b1.real*b2.imag + b1.imag*b2.real;
	return (b3);
}

//快速傅立叶变换-FFT
void FFT(struct compx *xin, int Num)
{
	int f, m, LH, nm, i, k, j, L;
	double p, ps ;
	int le, B, ip;
	float pi;
	struct compx w, t;
	
	LH = Num / 2; 
	f  = Num;
	for (m=1; (f=f/2)!=1; m++);    // 2^m=N
	{
		for (L=m; L>=1; L--)          // 这里和时域的也有差别
		{ 
			le = pow(2, L);
			B  = le / 2;                // 每一级碟形运算间隔的点数
			pi = 3.1415;
			for (j=0; j<=B-1; j++)
			{
				p  = pow(2, m-L) * j;
				ps = 2*pi/Num*p;
				w.real = cos(ps);
				w.imag = -sin(ps);
				for (i=j; i<=Num-1; i=i+le)
				{
					ip = i+B;  
					t  = xin[i];
					xin[i].real = xin[i].real + xin[ip].real;
					xin[i].imag = xin[i].imag + xin[ip].imag;  
					xin[ip].real = xin[ip].real - t.real;
					xin[ip].imag = xin[ip].imag - t.imag;     
					xin[ip] = EE(xin[ip], w);
				}
			}
		}
	}
	// 变址运算
	nm = Num - 2;   
	j  = Num / 2;
	for(i=1;i<=nm;i++)
	{
	if(i<j){t=xin[j];xin[j]=xin[i];xin[i]=t;}
	k=LH;
	while(j>=k){j=j-k;k=k/2;}
	j=j+k;
	}
}




