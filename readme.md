Test and Benchmark.
test(uint32_t,uint32_t, func) возвращает 1 если функция func считает неправильно и 0 если верно.
test(__m256i,__m256i,avx_func) возвращает количество неправильных сложений функцией avx_func.
timer(uint32_t,uint32_t,count,func) возвращает время выполнения count итераций функцией func.
timer(__m256i,__m256i,count,avx_func) возвращает время выполнения count итераций функцией func.

Realization.
naive_pixel_sum(uint32_t, uint32_t) наивная реализация с разбием и вествлением.
avx_pixel_sum(__m256i, __m256i) неисправная avx2 реализация.
another_pixel_sum(uint32_t, uint32_t) реализация с разбиением без ветвления.
pixel_sum(uint32_t , uint32_t ) неисправная реализация без ветвления.
magic_add_pixels(uint32_t , uint32_t ) реализация без ветвления.
another_avx_pixel_sum(__m256i , __m256i ) avx2 реализация. 
yet_another_avx_pixel_sum(__m256i, __m256i) avx2 реализация с использованием saturated арифметики.
show_pixel(uint32_t) Отображение
show_pixel(__m256i) Отображение