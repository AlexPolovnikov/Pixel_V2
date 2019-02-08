// PixelSum.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
//#include <xmmintrin.h>

using namespace std;

uint32_t pixel_sum(uint32_t left, uint32_t right);
void show_pixel(uint32_t left);

int main()
{
	uint32_t a=25543355;
	uint32_t b=2680;
	uint32_t sum =pixel_sum(a,b);

	show_pixel(a);
	show_pixel(b);
	show_pixel(sum);

	return 0;
}

uint32_t pixel_sum(uint32_t left, uint32_t right) {
	uint32_t sum = 0; 
	__asm {
		mov edx, left;
		mov ebx, right;
		mov al, UCHAR_MAX;
		add dl, bl;
		lahf;
		and ah, 1;
		mul ah;
		or dl, al;
		add dh, bh;
		mov al, UCHAR_MAX;
		lahf;
		and ah, 1;
		mul ah;
		or dh, al;
		bswap edx;
		bswap ebx;
		add dh, bh;
		mov al, UCHAR_MAX;
		lahf;
		and ah, 1;
		mul ah;
		or dh, al;
		bswap edx;
		mov sum, edx;
	}
	return sum;
}

void show_pixel(uint32_t left) {
	uint32_t tmp = 0;
	tmp = left & 0xff0000;
	tmp = tmp >> 16;
	cout << "R = " << (int)tmp << "	";
	tmp = left & 0xff00;
	tmp = tmp >> 8;
	cout << "G = " << (int)tmp << "	";
	tmp = left & 0xff;
	cout << "B = " << (int)tmp << "	";

	cout << endl;
}

