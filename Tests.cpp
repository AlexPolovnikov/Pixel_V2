#include "pch.h"
#include "Tests.h"
#include <chrono>

int test(__m256i a,__m256i b, avx_func f) {
	__m256i sum;
	uint32_t fail = 0;
	sum = f(a, b);

	for (int j = 0; j < 8; j++) {
		if ((sum.m256i_u32[j] & 0x00ffffff) != (naive_pixel_sum(a.m256i_u32[j], b.m256i_u32[j]) & 0x00ffffff)) {
				fail++;
		}
	}
	return fail;

}

int test(uint32_t a,uint32_t b,func f) {
	if ((f(a, b) & 0x00ffffff) != (naive_pixel_sum(a, b) & 0x00ffffff)) {
		return 1;
	}
	else {
		return 0;
	}

}

void timer(uint32_t a, uint32_t b,uint32_t count, func f) {
	auto t1 = chrono::high_resolution_clock::now();
	for (int k = 0; k < count; k++)
		f(a, b);
	auto t2 = chrono::high_resolution_clock::now();
	cout << count << " pixels without avx2 " << chrono::duration<double, std::milli>(t2 - t1).count() << " ms" << endl;
}

void timer(__m256i a, __m256i b, uint32_t count, avx_func f) {
	auto t1 = chrono::high_resolution_clock::now();
	for (int k = 0; k < count / 8; k++)
		f(a, b);
	auto t2 = chrono::high_resolution_clock::now();
	cout <<count<< " pixels with avx2 " << chrono::duration<double, std::milli>(t2 - t1).count() << " ms" << endl;
}