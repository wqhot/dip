#include "FreCal.h"
#include <math.h>
#define SQRT2 1.41421356f

FreCal::FreCal() {
}

FreCal::~FreCal() {
}

Matrix FreCal::fre2im(Complex* src, int size_w, int size_h) {
	//Image dest;
	//dest.channel = 1;
	int base2_w = size_w;
	if (isBase2(size_w) == -1) {
		base2_w = 1 << getBase2(size_w);
	}
    Matrix dst;
    dst.create(size_w, size_h);
	//dest.matrixB.create(size_w, size_h);
	for (int i = 0; i < size_h; i++) {
		for (int j = 0; j < size_w; j++) {
			Complex temp = *(src + i*base2_w + j);
			float t = sqrt(temp.real*temp.real + temp.imagin*temp.imagin);
            dst.setPixel(i + 1, j + 1, /*log(t) / log((float)DIP_E)*/t);
		}
	}
    dst = dst.normalization(255, 0);
	return dst;
}


int FreCal::addComplex(Complex * src1, Complex * src2, Complex * dst) {
	dst->imagin = src1->imagin + src2->imagin;
	dst->real = src1->real + src2->real;
	return 0;
}


int FreCal::subComplex(Complex * src1, Complex * src2, Complex * dst) {
	dst->imagin = src1->imagin - src2->imagin;
	dst->real = src1->real - src2->real;
	return 0;
}


int FreCal::mulComplex(Complex * src1, Complex * src2, Complex * dst) {
	float r1 = 0.0, r2 = 0.0;
	float i1 = 0.0, i2 = 0.0;
	r1 = src1->real;
	r2 = src2->real;
	i1 = src1->imagin;
	i2 = src2->imagin;
	dst->imagin = r1*i2 + r2*i1;
	dst->real = r1*r2 - i1*i2;
	return 0;
}


int FreCal::copyComplex(Complex * src, Complex * dst) {
	dst->real = src->real;
	dst->imagin = src->imagin;
	return 0;
}


int FreCal::getW(float n, float size_n, Complex * dst) {
	float x = 2.0*DIP_PI*n / size_n;
	dst->imagin = -sin(x);
	dst->real = cos(x);
   
	return 0;
}

int FreCal::FFTComplexRemap(Complex * src, int size_n) {
    int M = 1;
    for (int i = size_n; (i = i / 2) != 1; M++);
    int j = size_n / 2;
    Complex t;
    for (int i = 1; i <= size_n - 2; i++) {
        if (i<j) {
            copyComplex(&src[j],&t);
            copyComplex(&src[i], &src[j]);
            copyComplex(&t, &src[i]);
        }
        int k = size_n / 2;
        while (k <= j) {
            j = j - k;
            k = k / 2;
        }
        j = j + k;
    }
	return 0;
}


int FreCal::FFT(Complex * src, Complex * dst, int size_n) {
	int k = size_n;
	int z = 0;
	while (k /= 2) {
		z++;
	}
	k = z;
	if (size_n != (1 << k))
		return -1;
	Complex * src_com = (Complex*)malloc(sizeof(Complex)*size_n);

	for (int i = 0; i<size_n; i++) {
		copyComplex(&src[i], &src_com[i]);
	}
	FFTComplexRemap(src_com, size_n);
	for (int i = 0; i<k; i++) {//一级蝶形运算
		z = 0;
		for (int j = 0; j<size_n; j++) {
			if ((j / (1 << i)) % 2 == 1) {
				//Complex wn;
				//getW(z, size_n, &wn);
				mulComplex(&src_com[j], wn+z, &src_com[j]);
				z += 1 << (k - i - 1);
				Complex temp;
				int neighbour = j - (1 << (i));
				temp.real = src_com[neighbour].real;
				temp.imagin = src_com[neighbour].imagin;
				addComplex(&temp, &src_com[j], &src_com[neighbour]);
				subComplex(&temp, &src_com[j], &src_com[j]);
			}
			else
				z = 0;
		}
	}
	for (int i = 0; i<size_n; i++) {
		copyComplex(&src_com[i], &dst[i]);
	}
	free(src_com);
	return 0;
}


int FreCal::reaFFT(float * src, Complex * dst, int size_n) {
	int k = size_n;
	int z = 0;
	while (k /= 2) {
		z++;
	}
	k = z;
	if (size_n != (1 << k))
		return -1;
	Complex * src_com = (Complex*)malloc(sizeof(Complex)*size_n);
	for (int i = 0; i<size_n; i++) {
		src_com[i].real = src[i];
		src_com[i].imagin = 0;
	}
	FFTComplexRemap(src_com, size_n);
	for (int i = 0; i<k; i++) {
		z = 0;
		for (int j = 0; j<size_n; j++) {
			if ((j / (1 << i)) % 2 == 1) {
				//Complex wn;
				//getW(z, size_n, &wn);
				mulComplex(&src_com[j], (wn+z), &src_com[j]);
				z += 1 << (k - i - 1);
				Complex temp;
				int neighbour = j - (1 << (i));
				temp.real = src_com[neighbour].real;
				temp.imagin = src_com[neighbour].imagin;
				addComplex(&temp, &src_com[j], &src_com[neighbour]);
				subComplex(&temp, &src_com[j], &src_com[j]);
			}
			else
				z = 0;
		}
	}
	for (int i = 0; i<size_n; i++) {
		copyComplex(&src_com[i], &dst[i]);
	}
	free(src_com);
	return 0;
}


int FreCal::IFFT(Complex * src, Complex * dst, int size_n) {
	for (int i = 0; i<size_n; i++)
		src[i].imagin = -src[i].imagin;
	FFTComplexRemap(src, size_n);
	int z, k;
	if ((z = isBase2(size_n)) != -1)
		k = isBase2(size_n);
	else
		return -1;
	for (int i = 0; i<k; i++) {
		z = 0;
		for (int j = 0; j<size_n; j++) {
			if ((j / (1 << i)) % 2 == 1) {
				//Complex wn;
				//getW(z, size_n, &wn);
				mulComplex(&src[j], wn+z, &src[j]);
				z += 1 << (k - i - 1);
				Complex temp;
				int neighbour = j - (1 << (i));
				copyComplex(&src[neighbour], &temp);
				addComplex(&temp, &src[j], &src[neighbour]);
				subComplex(&temp, &src[j], &src[j]);
			}
			else
				z = 0;
		}
	}
	for (int i = 0; i<size_n; i++) {
		dst[i].imagin = (1. / size_n)*src[i].imagin;
		dst[i].real = (1. / size_n)*src[i].real;
	}
	return 0;
}


int FreCal::isBase2(int n) {
	int k = n;
	int z = 0;
	while (k /= 2) {
		z++;
	}
	k = z;
	if (n != (1 << k))
		return -1;
	else
		return k;
	return 0;
}


Complex * FreCal::DFT2D(Matrix& mat/*, Complex * dst*/) {
    //Matrix mat = _mat.copy();
	int size_w = mat.width;
	int size_h = mat.height;

	if (isBase2(size_w) == -1 || isBase2(size_h) == -1) {
		int newWidth = 1 << getBase2(size_w);
		int newHeight = 1 << getBase2(size_h);
		size_h = newHeight;
		size_w = newWidth;
		mat = mat.expand(newWidth, newHeight);
	}
    wn = (Complex*)malloc(sizeof(Complex)*size_w);
    for (int i = 0; i < size_w/2; i++) {
        getW(i, size_w, wn + i);
    }
	float* src = (float*)malloc(sizeof(float)*size_w*size_h);
	Complex * dst = (Complex*)malloc(sizeof(Complex)*size_w*size_h);
	memcpy(src, mat.data, sizeof(float)*size_w*size_h);
	for (int i = 0; i < mat.height; i++) {
		for (int j = 0; j < mat.width; j++) {
			*(src + i*mat.width + j) *= (i + j) % 2 == 0 ? 1 : -1;
		}
	}
	Complex *temp = (Complex *)malloc(sizeof(Complex)*size_h*size_w);
	for (int i = 0; i<size_h; i++) {
		reaFFT(&src[size_w*i], &temp[size_w*i], size_w);
	}
    free(wn);
    wn = (Complex*)malloc(sizeof(Complex)*size_h);
    for (int i = 0; i < size_h / 2; i++) {
        getW(i, size_h, wn+i);
    }
	Complex *Column = (Complex *)malloc(sizeof(Complex)*size_h);

	for (int i = 0; i<size_w; i++) {
		columnVector(&temp[i], Column, size_w, size_h);
		FFT(Column, Column, size_h);
		iColumnVector(Column, &temp[i], size_w, size_h);

	}
	for (int i = 0; i<size_h*size_w; i++)
		copyComplex(&temp[i], &dst[i]);
	free(temp);
    free(wn);
	free(Column);
	free(src);
	return dst;
}


int FreCal::IDFT2D(Complex * src, Matrix& dest, int size_w, int size_h, int old_w,int old_h) {
	if (isBase2(size_w) == -1 || isBase2(size_h) == -1)
		return -1;

	Complex *temp = (Complex *)malloc(sizeof(Complex)*size_h*size_w);
	if (temp == NULL)
		return -1;
	Complex *Column = (Complex *)malloc(sizeof(Complex)*size_h);
	if (Column == NULL)
		return -1;
    wn = (Complex*)malloc(sizeof(Complex)*size_w);
    for (int i = 0; i < size_w / 2; i++) {
        getW(i, size_w, wn + i);
    }
	for (int i = 0; i<size_w; i++) {
		columnVector(&src[i], Column, size_w, size_h);
		IFFT(Column, Column, size_h);
		iColumnVector(Column, &src[i], size_w, size_h);

	}
	for (int i = 0; i<size_w*size_h; i++)
		src[i].imagin = -src[i].imagin;
    free(wn);
    wn = (Complex*)malloc(sizeof(Complex)*size_h);
    for (int i = 0; i < size_h / 2; i++) {
        getW(i, size_h, wn + i);
    }
    for (int i = 0; i<size_h; i++) {
		IFFT(&src[size_w*i], &temp[size_w*i], size_w);

	}
	Complex* dst = (Complex*)malloc(sizeof(Complex)*size_w*size_h);
	dest.destory();
	dest.create(size_w, size_h);
	for (int i = 0; i<size_h*size_w; i++){
		copyComplex(&temp[i], dst + i);
		Complex c = *(dst + i);
		*(dest.data + i) = c.real;
	}	
    dest = dest.cut(1, old_h, 1, old_w);
	for (int i = 0; i < old_h; i++) {
		for (int j = 0; j < old_w; j++) {
			dest.setPixel(i + 1, j + 1, dest.getPixel(i + 1, j + 1)* ((i + j) % 2 == 0 ? 1 : -1));
		}
	}
    
    free(wn);
	free(temp);
	free(Column);
	free(dst);
	return 0;
}


int FreCal::columnVector(Complex * src, Complex * dst, int size_w, int size_h) {
	for (int i = 0; i<size_h; i++)
		copyComplex(&src[size_w*i], &dst[i]);
	return 0;
}


int FreCal::iColumnVector(Complex * src, Complex * dst, int size_w, int size_h) {
	for (int i = 0; i<size_h; i++)
		copyComplex(&src[i], &dst[size_w*i]);
	return 0;
}


int FreCal::getBase2(int n) {
	int k = n;
	int z = 0;
	while (k /= 2) {
		z++;
	}
	k = z;
	return k+1;
}




//Matrix FreCal::FDCT2(Matrix mat) {
//    Matrix tmat = mat.copy();
//    int size_w = tmat.width;
//    int size_h = tmat.height;
//    int old_size_w = size_w;
//    int old_size_h = size_h;
//
//    if (isBase2(size_w) == -1 || isBase2(size_h) == -1) {
//        int newWidth = 1 << getBase2(size_w);
//        int newHeight = 1 << getBase2(size_h);
//        size_h = newHeight;
//        size_w = newWidth;
//        tmat = tmat.expand(newWidth, newHeight);
//    }
//    mn = (float*)malloc(sizeof(float)*size_w);
//    float sqrt2w= sqrt(2.0 / size_w);
//    for (int i = 0; i < size_w / 2; i++) {
//        mn[i] = cos((2 * i + 1)*DIP_PI / 2 / size_w)*sqrt2w;
//    }
//    float* src = (float*)malloc(sizeof(float)*size_w*size_h);
//    memcpy(src, tmat.data, sizeof(float)*size_w*size_h);
//   
//    float *temp = (float *)malloc(sizeof(float)*size_h*size_w);
//    for (int i = 0; i<size_h; i++) {
//        DCT(&src[size_w*i], &temp[size_w*i], size_w);
//    }
//    free(mn);
//    mn = (float*)malloc(sizeof(float)*size_h);
//    sqrt2w = sqrt(2.0 / size_h);
//    for (int i = 0; i < size_h / 2; i++) {
//        mn[i] = cos((2 * i + 1)*DIP_PI / 2 / size_h)*sqrt2w;
//    }
//    float *Column = (float *)malloc(sizeof(float)*size_h);
//
//    for (int i = 0; i<size_w; i++) {
//        realColumnVector(&temp[i], Column, size_w, size_h);
//        DCT(Column, Column, size_h);
//        irealColumnVector(Column, &temp[i], size_w, size_h);
//
//    }
//    Matrix result(size_h, size_w, temp);
//    free(temp);
//    free(mn);
//    free(Column);
//    free(src);
//    return result;
//}


//int FreCal::DCT(float* src, float* dst, int size_n) {
//    int k = size_n;
//    int z = 0;
//    while (k /= 2) {
//        z++;
//    }
//    k = z;
//    if (size_n != (1 << k))
//        return -1;
//    float * src_com = (float*)malloc(sizeof(float)*size_n);
//    memcpy(src_com, src, sizeof(float)*size_n);
//    src_com[0] = src[0] / SQRT2;
//    DCTRealRemap(src_com, size_n);
//    for (int i = 0; i<k; i++) {
//        z = 0;
//        for (int j = 0; j<size_n; j++) {
//            if ((j / (1 << i)) % 2 == 1) {
//                src_com[j] *= mn[z];
//                z += 1 << (k - i - 1);
//                float temp;
//                int neighbour = j - (1 << (i));
//                temp = src_com[neighbour];
//                src_com[neighbour] = src_com[j] + temp;
//                src_com[j] = temp - src_com[j];
//            }
//            else
//                z = 0;
//        }
//    }
//    memcpy(dst, src_com, sizeof(float)*size_n);
//    free(src_com);
//    return 0;
//}


//int FreCal::DCTRealRemap(float* src, int size_n) {
//    int M = 1;
//    for (int i = size_n; (i = i / 2) != 1; M++);
//    int j = size_n / 2;
//    float t;
//    for (int i = 1; i <= size_n - 2; i++) {
//        if (i<j) {
//            t = src[j];
//            src[j] = src[i];
//            src[i] = t;
//        }
//        int k = size_n / 2;
//        while (k <= j) {
//            j = j - k;
//            k = k / 2;
//        }
//        j = j + k;
//    }
//    return 0;
//}


//int FreCal::realColumnVector(float* src, float* dst, int size_w, int size_h) {
//    for (int i = 0; i < size_h; i++)
//        dst[i] = src[size_w*i];
//    return 0;
//}


//int FreCal::irealColumnVector(float* src, float* dst, int size_w, int size_h) {
//    for (int i = 0; i < size_h; i++)
//        dst[size_w*i] = src[i];
//    return 0;
//}


//Matrix FreCal::IFDCT2(Matrix mat, int size_h, int size_w) {
//    int old_h = size_h;
//    int old_w = size_w;
//    size_h = mat.height;
//    size_w = mat.width;
//    if (isBase2(size_w) == -1 || isBase2(size_h) == -1)
//        return Matrix();
//
//    float *temp = (float *)malloc(sizeof(float)*size_h*size_w);
//
//    float *Column = (float *)malloc(sizeof(float)*size_h);
//    float sqrt2w = sqrt(2.0 / size_w);
//    float *src = (float *)malloc(sizeof(float)*size_h*size_w);
//    memcpy(src, mat.data, size_h*size_w * sizeof(float));
//    mn = (float*)malloc(sizeof(float)*size_w);
//    for (int i = 0; i < size_w / 2; i++) {
//        mn[i] = cos((2 * i + 1)*DIP_PI / 2 / size_w)*sqrt2w;
//    }
//    for (int i = 0; i<size_w; i++) {
//        realColumnVector(&src[i], Column, size_w, size_h);
//        IDCT(Column, Column, size_h);
//        irealColumnVector(Column, &src[i], size_w, size_h);
//    }
//   
//    free(mn);
//    mn = (float*)malloc(sizeof(float)*size_h);
//    sqrt2w = sqrt(2.0 / size_h);
//    for (int i = 0; i < size_h / 2; i++) {
//        mn[i] = cos((2 * i + 1)*DIP_PI / 2 / size_w)*sqrt2w;
//    }
//    for (int i = 0; i<size_h; i++) {
//        IDCT(&src[size_w*i], &temp[size_w*i], size_w);
//
//    }
//
//    Matrix result(size_h, size_w, temp);
//    result = result.cut(1, old_h, 1, old_w);
//    free(mn);
//    free(temp);
//    free(Column);
//    return result;
//}


//int FreCal::IDCT(float* src, float* dst, int size_n) {
//    
//    int z, k;
//    if ((z = isBase2(size_n)) != -1)
//        k = isBase2(size_n);
//    else
//        return -1;
//    float*src_com = (float*)malloc(sizeof(float)*size_n);
//    memcpy(src_com, src, sizeof(float)*size_n);
//    DCTRealRemap(src_com, size_n);
//    for (int i = 0; i<k; i++) {
//        z = 0;
//        for (int j = 0; j<size_n; j++) {
//            if ((j / (1 << i)) % 2 == 1) {
//                src_com[j] *= mn[z];
//                z += 1 << (k - i - 1);
//                float temp;
//                int neighbour = j - (1 << (i));
//                temp = src_com[neighbour];
//                src_com[neighbour] = src_com[j] + temp;
//                src_com[j] = temp - src_com[j];
//            }
//            else
//                z = 0;
//        }
//    }
//    memcpy(dst, src_com, sizeof(float)*size_n);
//    free(src_com);
//    return 0;
//}


Matrix FreCal::dct2d(Matrix _src,int block) {
    Matrix src = _src.copy();
    if (block > 0) {
        if (src.width%block != 0 || src.height%block != 0) {
            int w_size = block - src.width%block;
            int h_size = block - src.height%block;
            src = src.expand(src.width + w_size, src.height + h_size);
        }
        int size_n = block;
        dctA.create(size_n, size_n);
        dctAT.create(size_n, size_n);
        float sqrt2dN = sqrt(2.0f / size_n);
        float sqrt1dN = sqrt(1.0f / size_n);
        mn = (float*)malloc(sizeof(float)*size_n * 2);
        float t = DIP_PI / 2 / size_n;
        for (int i = 0; i < size_n + 1; i++) {
            mn[i] = cos(i * t);
        }
        for (int i = size_n + 1; i < 2 * size_n; i++) {
            mn[i] = 0.0f - mn[2 * size_n - i];
        }
        for (int i = 1; i < size_n; i++) {
            for (int j = 0; j < size_n; j++) {
                int kk = (int)((float)i*(2 * j + 1) / size_n / 2);
                int k = i*(2 * j + 1) - 2 * size_n*kk;
                dctA.setPixel(i + 1, j + 1, mn[k] * sqrt2dN*(kk % 2 == 0 ? 1 : -1));
            }
        }
        for (int i = 0; i < size_n; i++) {
            dctA.setPixel(1, i + 1, sqrt1dN);
        }
        dctAT = dctA.transpose();
        Matrix MatTemp;
        for (int i = 0; i < src.height / block; i++) {
            for (int j = 0; j < src.width / block; j++) {
                MatTemp = src.cut(i*block + 1, i*block + block, j*block + 1, j*block + block);
                MatTemp= dctA.mul(MatTemp).mul(dctAT);
                for (int ii = 0; ii < block; ii++) {
                    memcpy(src.data + (i*block + ii)*src.width + j*block, MatTemp.data + ii*block, block * sizeof(float));
                }
            }
        }
        MatTemp.destory();
    }
    else {
        if (src.width > src.height) {
            src = src.expand(src.width, src.width);
        }
        else if (src.height > src.width) {
            src = src.expand(src.height, src.height);
        }

        int size_n = src.width;
        dctA.create(size_n, size_n);
        dctAT.create(size_n, size_n);
        float sqrt2dN = sqrt(2.0f / size_n);
        mn = (float*)malloc(sizeof(float)*size_n * 2);
        float t = DIP_PI / 2 / size_n;
        for (int i = 0; i < size_n + 1; i++) {
            mn[i] = cos(i * t);
        }
        for (int i = size_n + 1; i < 2 * size_n; i++) {
            mn[i] = 0.0f - mn[2 * size_n - i];
        }
        for (int i = 1; i < size_n; i++) {
            for (int j = 0; j < size_n; j++) {
                int kk = (int)((float)i*(2 * j + 1) / size_n / 2);
                int k = i*(2 * j + 1) - 2 * size_n*kk;
                dctA.setPixel(i + 1, j + 1, mn[k] * sqrt2dN*(kk % 2 == 0 ? 1 : -1));
            }
        }
        float sqrt1dN = sqrt(1.0f / size_n);
        for (int i = 0; i < size_n; i++) {
            dctA.setPixel(1, i + 1, sqrt1dN);
        }
        dctAT = dctA.transpose();
        src = dctA.mul(src).mul(dctAT);
        
    }
    dctA.destory();
    dctAT.destory();
    free(mn);
    return src;
}


Matrix FreCal::idct2d(Matrix _src,int block) {
    Matrix src = _src.copy();
    if (block > 0) {
        if (src.width%block != 0 || src.height%block != 0) {
            int w_size = block - src.width%block;
            int h_size = block - src.height%block;
            src = src.expand(src.width + w_size, src.height + h_size);
        }
        int size_n = block;
        dctA.create(size_n, size_n);
        dctAT.create(size_n, size_n);
        float sqrt2dN = sqrt(2.0f / size_n);
        float sqrt1dN = sqrt(1.0f / size_n);
        mn = (float*)malloc(sizeof(float)*size_n * 2);
        float t = DIP_PI / 2 / size_n;
        for (int i = 0; i < size_n + 1; i++) {
            mn[i] = cos(i * t);
        }
        for (int i = size_n + 1; i < 2 * size_n; i++) {
            mn[i] = 0.0f - mn[2 * size_n - i];
        }
        for (int i = 1; i < size_n; i++) {
            for (int j = 0; j < size_n; j++) {
                int kk = (int)((float)i*(2 * j + 1) / size_n / 2);
                int k = i*(2 * j + 1) - 2 * size_n*kk;
                dctA.setPixel(i + 1, j + 1, mn[k] * sqrt2dN*(kk % 2 == 0 ? 1 : -1));
            }
        }
        for (int i = 0; i < size_n; i++) {
            dctA.setPixel(1, i + 1, sqrt1dN);
        }
        dctAT = dctA.transpose();
        Matrix MatTemp;
        for (int i = 0; i < src.height / block; i++) {
            for (int j = 0; j < src.width / block; j++) {
                MatTemp = src.cut(i*block + 1, i*block + block, j*block + 1, j*block + block);
                MatTemp = dctAT.mul(MatTemp).mul(dctA);
                for (int ii = 0; ii < block; ii++) {
                    memcpy(src.data + (i*block + ii)*src.width + j*block, MatTemp.data + ii*block, block * sizeof(float));
                }
            }
        }
        MatTemp.destory();
    }
    else {
        if (src.width > src.height) {
            src = src.expand(src.width, src.width);
        }
        else if (src.height > src.width) {
            src = src.expand(src.height, src.height);
        }

        int size_n = src.width;
        dctA.create(size_n, size_n);
        dctAT.create(size_n, size_n);
        float sqrt2dN = sqrt(2.0f / size_n);
        mn = (float*)malloc(sizeof(float)*size_n * 2);
        float t = DIP_PI / 2 / size_n;
        for (int i = 0; i < size_n + 1; i++) {
            mn[i] = cos(i * t);
        }
        for (int i = size_n + 1; i < 2 * size_n; i++) {
            mn[i] = 0.0f - mn[2 * size_n - i];
        }
        for (int i = 1; i < size_n; i++) {
            for (int j = 0; j < size_n; j++) {
                int kk = (int)((float)i*(2 * j + 1) / size_n / 2);
                int k = i*(2 * j + 1) - 2 * size_n*kk;
                dctA.setPixel(i + 1, j + 1, mn[k] * sqrt2dN*(kk % 2 == 0 ? 1 : -1));
            }
        }
        float sqrt1dN = sqrt(1.0f / size_n);
        for (int i = 0; i < size_n; i++) {
            dctA.setPixel(1, i + 1, sqrt1dN);
        }
        dctAT = dctA.transpose();
        src = dctAT.mul(src).mul(dctA);

    }
    dctA.destory();
    dctAT.destory();
    free(mn);
    return src;
}
