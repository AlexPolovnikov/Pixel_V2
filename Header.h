#pragma once
#include <immintrin.h>
#include <iomanip>
#include <iostream>

using namespace std;

uint32_t naive_pixel_sum(uint32_t left, uint32_t right);
__m256i avx_pixel_sum(__m256i left, __m256i right);
uint32_t another_pixel_sum(uint32_t left, uint32_t right);
uint32_t pixel_sum(uint32_t left, uint32_t right);
uint32_t magic_add_pixels(uint32_t left, uint32_t right);
__m256i another_avx_pixel_sum(__m256i left, __m256i right);
__m256i yet_another_avx_pixel_sum(__m256i left, __m256i right);
void show_pixel(uint32_t left);
void show_pixel(__m256i left);