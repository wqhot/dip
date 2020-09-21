#ifndef FRECAL_H
#define FRECAL_H
#include <Matrix.h>
#include <ImagProc.h>

namespace dip
{
	typedef struct COMPLEX
	{
		float imagin;
		float real;
	} Complex;

	class FreCal
	{
	public:
		FreCal();
		~FreCal();
		Matrix fre2im(Complex *src, int size_w, int size_h);

	private:
		Complex *wn;
		float *mn;
		Matrix dctA;
		Matrix dctAT;
		int addComplex(Complex *src1, Complex *src2, Complex *dst);
		int subComplex(Complex *src1, Complex *src2, Complex *dst);
		int mulComplex(Complex *src1, Complex *src2, Complex *dst);
		int copyComplex(Complex *src, Complex *dst);
		int getW(float n, float size_n, Complex *dst);
		//	int FFTRealRemap(float* src, int size_n);
		int FFTComplexRemap(Complex *src, int size_n);
		int FFT(Complex *src, Complex *dst, int size_n);
		int reaFFT(float *src, Complex *dst, int size_n);
		int IFFT(Complex *src, Complex *dst, int size_n);

		int isBase2(int n);

	public:
		Complex *DFT2D(Matrix &mat /*, Complex * dst*/);
		int IDFT2D(Complex *src, Matrix &dest, int size_w, int size_h, int old_w, int old_h);

	private:
		int columnVector(Complex *src, Complex *dst, int size_w, int size_h);
		int iColumnVector(Complex *src, Complex *dst, int size_w, int size_h);
		int getBase2(int n);

	public:
		Matrix dct2d(Matrix src, int block = 0);
		Matrix idct2d(Matrix _src, int block = 0);
	};
}; // namespace dip

#endif
