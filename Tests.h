#pragma once
#include "Header.h"

typedef uint32_t(*func)(uint32_t a, uint32_t b);
typedef __m256i(*avx_func)(__m256i a, __m256i b);

int test(__m256i a, __m256i b,avx_func f);
int test(uint32_t a, uint32_t b, func f);
void timer(__m256i a, __m256i b, uint32_t count, avx_func f);
void timer(uint32_t a, uint32_t b, uint32_t count, func f);