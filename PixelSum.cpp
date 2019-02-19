// PixelSum.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

uint32_t naive_pixel_sum(uint32_t left, uint32_t right);
uint32_t pixel_sum(uint32_t left, uint32_t right);
void show_pixel(uint32_t left);

int main()
{
	uint32_t a=35533445;
	uint32_t b=2680;
	uint32_t sum = 0;
	uint32_t stime=0;
	uint32_t etime=0;

	//auto t1 = chrono::steady_clock::now();
	_asm rdtsc;
	_asm mov stime, eax;
	sum = pixel_sum(a, b);
	//auto t2 = chrono::steady_clock::now();
	//cout << chrono::duration<double, std::nano>(t2 - t1).count()<< " ns" << endl;
	_asm rdtsc;
	_asm mov etime, eax;
	cout << etime - stime << endl;
	show_pixel(sum);

	//t1 = chrono::steady_clock::now();
	_asm rdtsc;
	_asm mov stime, eax;
	sum = pixel_sum(a, b);
	//t2 = chrono::steady_clock::now();
	//cout << chrono::duration<double, std::nano>(t2 - t1).count() << " ns" << endl;
	_asm rdtsc;
	_asm mov etime, eax;
	cout << etime - stime << endl;
	show_pixel(sum);
	
	//show_pixel(a);
	//show_pixel(b);
	//show_pixel(sum);

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

uint32_t naive_pixel_sum(uint32_t left, uint32_t right) {
	uint32_t sum = 0;
	int tmp1 = 0;
	int tmp2 = 0;

	tmp1 = left & 0xff0000;
	tmp1 = tmp1 >> 16;
	tmp2 = right & 0xff0000;
	tmp2 = tmp2 >> 16;

	if ((tmp1 + tmp2) >= 255) {
		sum &= 0xff0000;
	}
	else {
		tmp1 += tmp2;
		tmp1 = tmp1 << 16;
		sum = sum | tmp1;
	}

	tmp1 = left & 0xff00;
	tmp1 = tmp1 >> 8;
	tmp2 = right & 0xff00;
	tmp2 = tmp2 >> 8;

	if ((tmp1 + tmp2) >= 255) {
		sum &= 0xff00;
	}
	else {
		tmp1 += tmp2;
		tmp1 = tmp1 << 8;
		sum = sum | tmp1;
	}

	tmp1 = left & 0xff;
	//tmp1 = tmp1 >> 16;
	tmp2 = right & 0xff;
	//tmp2 = tmp2 >> 16;

	if ((tmp1 + tmp2) >= 255) {
		sum = 0xff;
	}
	else {
		tmp1 += tmp2;
		//tmp1 = tmp1 << 16;
		sum = sum | tmp1;
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

