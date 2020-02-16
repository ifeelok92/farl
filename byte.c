// #include "stdafx.h"
#include "byte.h"

/**
 * input a polynomial, each entry is within {0,...,7}
 * output a byte array, every 3 bit corrresponds to an entry
 * needs 3*1024/8 = 384 bytes
 * every eight coeff comprise 3 bytes
 * 1024 / 8 = 128 "eight coeff"
 */
void coeff3bit_to_byte_array(unsigned char *octet, int *poly)
{
	int i, t=0;
	for (i = 0; i < 1024; i = i + 8, t = t+3 ){
		octet[t] = poly[i] | (poly[i + 1] << 3) | ((poly[i+2])<<6);  // 3 3 2
        octet[t + 1] = (poly[i + 2] >> 2) | (poly[i+3]<<1) | (poly[i+4]<<4) | ((poly[ i + 5] )<<7);  // 1 3 3 1
        octet[t + 2] = (poly[i + 5] >> 1) | (poly[i+6]<<2) | (poly[i+7]<<5);  // 2 3 3
    }
}

/**
 * store poly by in byte array octet
 * coef of 20 bit 
 * every two coef makes 5 bytes
 * 512 * 5 = 2560 bytes
 */
void poly_tobytes(unsigned char *octet, const int32_t *poly) 
{
	int i, t1, t2;
    t1 = 0;
    for (i = 0; i < N; i = i + 2, t1 = t1+5)
    {
        
		octet[t1] = poly[i];
        octet[t1 + 1] = (poly[i] >> 8); //8
		octet[t1+ 2] = (poly[i] >> 16) | ((poly[i +1]&15) << 4);
		octet[t1 + 3] = (poly[i + 1] >> 4);//8
		octet[t1 + 4] = (poly[i + 1] >> 12);//8
	}
}

/**
 * recover poly from a byte array octet
 */
void poly_frombytes(int32_t *p, const unsigned char *octet) {
	int i, t1, t2;
    t2 = 0;
    memset(p,0,1024*sizeof(int));
    for (i = 0; i < N; i = i + 2, t2=t2+5)
    {
		p[i] = octet[t2] | (octet[t2 + 1] << 8) | ((octet[t2 + 2] & 15) << 16); // 8 + 8 + 4
		p[i + 1] = (octet[t2 + 2] >> 4) | (octet[t2 + 3] << 4) | (octet[t2 + 4] << 12);  //4 + 8 + 8	
	}
}




//the transform is special for Q(w)
void poly_tobytes_qw(unsigned char *octet, uint16_t *p) {
	int i = 0;
	for (i = 0; i < N2; i++)
	{
		octet[i] = (p[2 * i]) | (p[2 * i + 1] << 2);
	}
}

void poly_frombytes_qw(uint16_t *p, const unsigned char *octet)
{
	int i = 0;
	for (i = 0; i < N2; i++)
	{
		p[2*i] = octet[i]&7;
		p[2 * i + 1] = (octet[i] >> 4);
	}

}




/*the transform is special for q<2^10
here special for the transformation of hash output*/
void poly_tobytes_ten(unsigned char *a, const uint16_t *f, const int32_t polylen) {
	int i;
	uint16_t t0, t1, t2, t3;
	for (i = 0; i < (polylen>>2); i++)
	{
		t0 = f[4 * i + 0];
		t1 = f[4 * i + 1];
		t2 = f[4 * i + 2];
		t3 = f[4 * i + 3];

		a[5 * i + 0] = t0 & 0xff;//8
		a[5 * i + 1] = (t0 >> 8) | (t1 << 2) & 0xff;//2+6
		a[5 * i + 2] = (t1 >> 6) | (t2 << 4) & 0xff;//4+4
		a[5 * i + 3] = (t2 >> 4) & 0xff | (t3 << 6) & 0xff;//6+2
		a[5 * i + 4] = (t3 >> 2) & 0xff;//8
	}
}

void poly_frombytes_ten(uint16_t * f, const unsigned char *a, const int32_t polylen) {
	int i;
	for (i = 0; i < polylen / 4; i++)
	{
		f[4 * i + 0] = a[5 * i + 0] | ((a[5 * i + 1] & 0x3) << 8);//8+2
		f[4 * i + 1] = (a[5 * i + 1] >> 2) | ((a[5 * i + 2] & 0xf) << 6);//6+4
		f[4 * i + 2] = (a[5 * i + 2] >> 4) | ((a[5 * i + 3] & 0x3f) << 4);//4+6
		f[4 * i + 3] = (a[5 * i + 3] >> 6) | (a[5 * i + 4] << 2);//2+8
	}
}

/* transform for sk */
void trinary2byteArray(unsigned char *b, const int32_t *f)
{
	int i, temp;

	for(i =0; i<N4; i++)
	{
		temp = i<<2;
		b[i] = (f[temp]+1)|((f[temp+1]+1)<<2)|((f[temp+2]+1)<<4)|((f[temp+3]+1)<<6);

	}
}

void byteArray2trinary(int32_t *f, unsigned char *b)
{
	unsigned char temp;
	int i,j;
	for (i = 0; i < N4; i++)
	{
		temp = b[i];
		for (j = 0; j < 4; j++) 
		{
			f[(i<<2) + j] = (temp & 3) - 1;
			temp = (temp >> 2);
		}
	}
}


void byteArray2binary(int32_t *m, const unsigned char *b, const int32_t n)
{
	unsigned char temp;
	int i,j;
	for (i = 0; i < (n>>3); i++)
	{
		temp = b[i];
		for (j = 0; j < 8; j++)
		{
			m[(i<<3) + j] = temp & 1;
			temp = (temp >> 1);
		}
	}

}

/*the transform is special for q<2^16*/
void poly_tobytes_u(unsigned char *a, const int32_t *f) {
	int32_t i;
	int32_t t0;
	for (i = 0; i < N; i++)
	{
		t0 = f[i] + (ALPHA >> 1);

		a[2 * i + 0] = t0 & 0xff;//8
		a[2 * i + 1] = (t0 >> 8);//8
	}
}

void poly_frombytes_u(int32_t * f, const unsigned char *a) {
	int32_t i;
	for (i = 0; i < N; i++)
	{
		f[i] = a[2 * i + 0] | (a[2 * i + 1] << 8);//8+8
		f[i] = f[i]-(ALPHA >> 1);
	}
}
