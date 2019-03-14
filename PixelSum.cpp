#include "pch.h"
#include "Tests.h"

using namespace std;

int main()
{
	uint32_t a,b;
	__m256i a_256, b_256;
	uint32_t count = 0xffffff;

	_rdrand32_step(&a);
	_rdrand32_step(&b);

	for (int i = 0; i < 8; i++) {
		_rdrand32_step(&a_256.m256i_u32[i]);
		_rdrand32_step(&b_256.m256i_u32[i]);
	}

	cout << test(a, b,&magic_add_pixels) << endl;
	cout << test(a_256, b_256,&yet_another_avx_pixel_sum) << endl;

	timer(a, b, count,&magic_add_pixels);
	timer(a_256, b_256, count,&yet_another_avx_pixel_sum);

	return 0;
}


