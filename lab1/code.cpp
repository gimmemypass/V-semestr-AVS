#include <mmintrin.h>
#include <cstdlib>
#include <time.h>
#include <iostream>

//F[i]=A[i] *B[i] +C[i] -D[i] , i=1...8.

/*
Используются следующие массивы:
    A, B и С – 8 разрядные целые знаковые числа (_int8);
    D – 16 разрядные целые знаковые числа (_int16).
    Полученный результат отобразить на форме с использованием соответствующих элементов. 
    При распаковке знаковых чисел совместно с командами распаковки использовать команды сравнения (сравнивать с нулём перед распаковкой).
*/

__m64* unp8to16(__m64*);

int main(){
    srand(time(0));
    int size = 8;
    int8_t* A = new int8_t[size];
    int8_t* B = new int8_t[size];
    int8_t* C = new int8_t[size];
    int16_t* D = new int16_t[size];
    int16_t* F = new int16_t[size];
    

    for(int i = 0; i < size; i++){
       A[i] = rand() % INT8_MAX - INT8_MAX/2; 
       B[i] = rand() % INT8_MAX - INT8_MAX/2; 
       C[i] = rand() % INT8_MAX - INT8_MAX/2; 
       D[i] = rand() % INT16_MAX - INT16_MAX/2; 
    }
    ///// 
    long long sum[8];
    for(int i = 0; i< 8; i++){
        sum[i] = A[i]*B[i] + C[i] - D[i];
    }
    /////
    __m64* ma = reinterpret_cast<__m64*>(A);
    __m64* mb = reinterpret_cast<__m64*>(B);
    __m64* mc = reinterpret_cast<__m64*>(C);
    __m64* md = reinterpret_cast<__m64*>(D);
    __m64* mf = reinterpret_cast<__m64*>(F); 
    mc = unp8to16(mc); 
    ma = unp8to16(ma); 
    mb = unp8to16(mb); 
    for(int i = 0; i < size; i+=4){
        auto mul_lo = _mm_mullo_pi16(*ma++, *mb++);
        auto msum = _mm_adds_pi16(mul_lo,*mc++);
        msum = _mm_sub_pi16(msum,*md++);
        *mf++ = msum;
    }
   
    std::cout<< "mmx" << '\t' << "cycle" <<'\n';
   for(int i = 0; i < 8; i++){
       std::cout << F[i] << '\t' << sum[i] << '\n';
   }
   std::cout << '\n';
    
}

__m64* unp8to16(__m64* arr8){
    __m64 zero = _mm_setzero_si64(); 
    __m64 mask = _mm_cmpgt_pi8(zero, *arr8);
    __m64 low = _mm_unpacklo_pi8(*arr8, mask);
    __m64 high = _mm_unpackhi_pi8(*arr8, mask);
    __m64* arr16 = new __m64[2];
    arr16[0] = low;
    arr16[1] = high;
    delete(arr8);
    return arr16;
}