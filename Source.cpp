#include "pch.h"
#include "Header.h"


using namespace std;

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
		chl[i] = (chl[i] + chr[i]) | ((cf >> 8) * 255);
	}
	memcpy(&sum, chl, 4);
	return sum;
}

__m256i avx_pixel_sum(__m256i left, __m256i right) {
	__m256i res, xres;
	__m256i mask_f, mask_ff;
	for (int i = 0; i < 8; i++) {
		mask_f.m256i_u32[i] = 0x808080;
		mask_ff.m256i_u32[i] = 0xff;
	}
	res = _mm256_add_epi32(left, right);
	xres = _mm256_or_si256(_mm256_and_si256(left, right), _mm256_andnot_si256(res, _mm256_or_si256(left, right)));
	xres = _mm256_and_si256(xres, mask_f);
	xres = _mm256_slli_epi32(xres, 1);
	res = _mm256_xor_si256(res, xres);
	xres = _mm256_mullo_epi32(xres, mask_ff);
	xres = _mm256_srli_epi32(xres, 8);
	res = _mm256_or_si256(res, xres);
	return res;

}

__m256i yet_another_avx_pixel_sum(__m256i left, __m256i right) {
	return _mm256_adds_epu8(left,right);
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
	xres = _mm256_and_si256(xres, mask_f);
	ffres = _mm256_mullo_epi32(xres, mask_ff);
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
	uint32_t color = (left & 0xff0000) >> 16;
	cout << "R " << color << "	";
	color = (left & 0xff00) >> 8;
	cout << "G " << color << "	";
	color = left & 0xff;
	cout << "B " << color << "	";
	cout << endl;

}

void show_pixel(__m256i left) {
	for (int i = 0; i < 8; i++) {
		uint32_t color = (left.m256i_u32[i] & 0xff0000) >> 16;
		cout << "R " << color << "	";
		color = (left.m256i_u32[i] & 0xff00) >> 8;
		cout << "G " << color << "	";
		color = left.m256i_u32[i] & 0xff;
		cout << "B " << color << "	";
		cout << endl;
	}
}