#include <iostream>
#include <omp.h>
#include <ctime>
#include <random>

void init(int** matrix, int* vector, int row, int col){
    for(int i = 0; i < row; i++){
        matrix[i] = new int[col];
        for(int j = 0; j < col; j++){
            matrix[i][j] = rand(); 
            vector[j] = rand();
        }
    }
}
int* mulOMP(int** matrix, int* vector, int row, int col){
    int* result = new int[row];
    #pragma omp parallel
    {
        #pragma omp for 
        for(int i =0; i < row; i++){
            result[i] = 0;
            for(int j = 0; j < col; j++ ){
                result[i] += vector[j] * matrix[i][j];
            }
        } 
    }
    return result;
}

int* mul(int** matrix, int* vector, int row, int col){
    int* result = new int[row];
    int i;
    for(i =0; i < row; i++){
        result[i] = 0;
        for(int j = 0; j < col; j++ ){
            result[i] += vector[j] * matrix[i][j];
        }
    } 
    return result;
}
int main(){
    srand(time(0));
    int row = 100000;
    int col = 10000;
    int** matrix = new int*[row];
    int* vector = new int[col];
    init(matrix, vector, row, col);
    float time_start, time_end;
    int* result;

    time_start = omp_get_wtime(); 
    result = mul(matrix, vector, row, col);
    time_end = omp_get_wtime();
    std::cout << "without omp\t" << time_end - time_start << '\n';

    delete result;

    time_start = omp_get_wtime(); 
    result = mulOMP(matrix, vector, row, col);
    time_end = omp_get_wtime();
    std::cout << "with omp\t" << time_end - time_start << '\n';

    for(int i =0; i < row; i++){
        delete matrix[i];
    }
    delete result, vector, matrix;

    return 0;
}


