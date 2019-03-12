// PixelSum.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <emmintrin.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <smmintrin.h>
//#include "benchmark/include/benchmark/benchmark.h"


using namespace std;

uint32_t naive_pixel_sum(uint32_t left, uint32_t right);
__m256i avx_pixel_sum(__m256i left, __m256i right);
uint32_t another_pixel_sum(uint32_t left, uint32_t right);
uint32_t pixel_sum(uint32_t left, uint32_t right);
uint32_t magic_add_pixels(uint32_t left, uint32_t right);
__m256i another_avx_pixel_sum(__m256i left, __m256i right);
void show_pixel(uint32_t left);

int main()
{
	uint32_t a,b,sum,nsum;
	__m256i a_256, b_256,sum_avx;
	uint32_t count = 1920 * 1200;
	int fail_counter = 0;
	
	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 8; j++) {
			_rdrand32_step(&a_256.m256i_u32[j]);
			_rdrand32_step(&b_256.m256i_u32[j]);
		}
		sum_avx = another_avx_pixel_sum(a_256, b_256);
		
		for (int j = 0; j < 8; j++) {
			if (sum_avx.m256i_u32[j]!= naive_pixel_sum(a_256.m256i_u32[j],b_256.m256i_u32[j])) {
				fail_counter++;
			}
		}
	}
	
	cout << "fails " << fail_counter << endl;
	fail_counter = 0;
	for (int i = 0; i < 10000; i++) {
		_rdrand32_step(&a);
		_rdrand32_step(&b);
		if (magic_add_pixels(a, b) != naive_pixel_sum(a, b)) {
			fail_counter++;
		}
	}

	cout << "fails " << fail_counter << endl;

	auto t1 = chrono::high_resolution_clock::now();
	for (int i = 0; i < count; i++) {
		_rdrand32_step(&a);
		_rdrand32_step(&b);
		sum = magic_add_pixels(a, b);
	}
	auto t2 = chrono::high_resolution_clock::now();
	cout <<"1920x1200 pixel without avx2 "<< chrono::duration<double, std::milli>(t2 - t1).count() << " ms" << endl;

	t1 = chrono::high_resolution_clock::now();
	for (int i = 0; i < count; i++) {
		_rdrand32_step(&a);
		_rdrand32_step(&b);
		sum = naive_pixel_sum(a, b);
	}
	t2 = chrono::high_resolution_clock::now();
	cout << "1920x1200 pixel without avx2 and magic " << chrono::duration<double, std::milli>(t2 - t1).count() << " ms" << endl;

	t1 = chrono::high_resolution_clock::now();
	for (int k = 0; k < count/8; k++) {
		for (int i = 0; i < 8; i++) {
			_rdrand32_step(&a_256.m256i_u32[i]);
			_rdrand32_step(&b_256.m256i_u32[i]);
		}
		sum_avx = another_avx_pixel_sum(a_256, b_256);
	}
	t2 = chrono::high_resolution_clock::now();
	cout << "1920x1200  pixel with avx2 " << chrono::duration<double, std::milli>(t2 - t1).count() << " ms" << endl;
	
	//system("PAUSE");
	return 0;
}

uint32_t pixel_sum(uint32_t left, uint32_t right) {
	uint32_t res, xres;
	res = left + right;
	xres = (((left & right) | ((left | right) & ~res)) & 0x808080);
	res ^= xres << 1;
	res |= (xres * 0xFF) >> 7;
	return res;
}

uint32_t another_pixel_sum(uint32_t left, uint32_t right) {
	uint32_t sum;
	uint16_t cf = 0;
	unsigned char chl[4];
	memcpy(chl, &left, 4);
	unsigned char chr[4];
	memcpy(chr, &right, 4);

	for (int i = 0; i < 4; i++) {
		cf = chl[i] + chr[i];
		chl[i] = (chl[i] + chr[i]) | ((cf>>8) * 255);
	}
	memcpy(&sum, chl, 4);
	return sum;
}

__m256i avx_pixel_sum( __m256i left, __m256i right) {
	__m256i res, xres;
	__m256i mask_f,mask_ff;

	for (int i = 0; i < 8; i++) {
		mask_f.m256i_u32[i] = 0x808080;
		mask_ff.m256i_u32[i] = 0xff;
	}

	//res = left + right;
	res = _mm256_add_epi32(left, right);

	//xres = (((left & right) | ((left | right) & ~res)) & 0x808080);
	xres = _mm256_or_si256(_mm256_and_si256(left, right), _mm256_andnot_si256(res, _mm256_or_si256(left, right)));
	xres = _mm256_and_si256(xres, mask_f);

	//res ^= xres << 1;
	xres = _mm256_slli_epi32(xres, 1);
	res = _mm256_xor_si256(res, xres);      

	//res |= (xres * 0xFF) >> 7;
	xres = _mm256_mullo_epi32(xres,mask_ff);
	xres = _mm256_srli_epi32(xres, 8);
	res = _mm256_or_si256(res, xres);
	return res;

}

__m256i another_avx_pixel_sum(__m256i left, __m256i right) {
	__m256i res, xres, ffres;
	__m256i mask_f, mask_ff;

	for (int i = 0; i < 8; i++) {
		mask_f.m256i_u32[i] = 0x1010100;
		mask_ff.m256i_u32[i] = 0xff;
	}

	res = _mm256_add_epi32(left, right);
	xres = _mm256_xor_si256(_mm256_xor_si256(res, left), right);
	xres = _mm256_and_si256(xres,mask_f);
	ffres = _mm256_mullo_epi32(xres,mask_ff);
	ffres = _mm256_srli_epi32(ffres, 8);
	res = _mm256_or_si256(res, ffres);
	res = _mm256_sub_epi32(res, xres);
	res = _mm256_or_si256(res, ffres);
	return res;
}

uint32_t naive_pixel_sum(uint32_t rgb1, uint32_t rgb2)
{
	unsigned long sum = rgb1 + rgb2;
	if ((rgb1 & 0x000000FF) + (rgb2 & 0x000000FF) > 0xFF)
		sum = (sum - 0x100) | 0xFF;
	if ((rgb1 & 0x0000FF00) + (rgb2 & 0x0000FF00) > 0xFF00)
		sum = (sum - 0x10000) | 0xFF00;
	if ((rgb1 & 0x00FF0000) + (rgb2 & 0x00FF0000) > 0xFF0000)
		sum = (sum - 0x1000000) | 0xFF0000;
	//sum &= 0x00ffffff;
	return sum;
}

uint32_t magic_add_pixels(uint32_t left, uint32_t right) {
	uint32_t res, xres, ffres;
	res = left + right;
	xres = (res ^ left ^ right) & 0x1010100;
	ffres = (xres * 0xFF) >> 8;
	res = (res | ffres) - xres;
	res |= ffres;
	return res;
}

void show_pixel(uint32_t left) {
	/*unsigned char ch[4];
	memcpy(ch, &left, 4);
	for (int i = 2; i >=0; i--)
		cout << (int)ch[i] << "	";
	cout << endl;
	*/
	uint32_t color = (left & 0xff0000)>>16;
	cout << "R" << color << "	";
	color = (left & 0xff00)>>8;
	cout << "G" << color << "	";
	color = left & 0xff;
	cout << "B" << color << "	";
	cout << endl;
	
	}



