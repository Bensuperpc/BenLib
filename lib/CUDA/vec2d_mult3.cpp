//////////////////////////////////////////////////////////////
//   ____                                                   //
//  | __ )  ___ _ __  ___ _   _ _ __   ___ _ __ _ __   ___  //
//  |  _ \ / _ \ '_ \/ __| | | | '_ \ / _ \ '__| '_ \ / __| //
//  | |_) |  __/ | | \__ \ |_| | |_) |  __/ |  | |_) | (__  //
//  |____/ \___|_| |_|___/\__,_| .__/ \___|_|  | .__/ \___| //
//                             |_|             |_|          //
//////////////////////////////////////////////////////////////
//                                                          //
//  BenLib, 2021                                            //
//  Created: 19, March, 2021                                //
//  Modified: 20, March, 2021                               //
//  file: OpenCL_test.cpp                                   //
//  Crypto                                                  //
//  Source: https://www.techiedelight.com/dynamic-memory-allocation-in-c-for-2d-3d-array/
//          http://www.cplusplus.com/forum/general/263317/
//          https://stackoverflow.com/questions/18273370/the-correct-way-to-initialize-a-dynamic-pointer-to-a-multidimensional-array
//          https://data-flair.training/blogs/multi-dimensional-arrays-in-c-cpp/
//          https://www.geeksforgeeks.org/c-program-multiply-two-matrices/
//          https://www.daniweb.com/programming/software-development/threads/471477/equivalent-iteration-of-2d-and-3d-array-flattened-as-1d-array
//          https://stackoverflow.com/a/96419/10152334
//          https://forums.developer.nvidia.com/t/how-to-implement-calculation-pipeline-via-cuda-streams/27931
//          https://github.com/NVIDIA-developer-blog/code-samples/blob/master/series/cuda-cpp/overlap-data-transfers/async.cu
//  CPU: ALL                                                //
//                                                          //
//////////////////////////////////////////////////////////////

#include <cuda.h>
#include <cuda_runtime_api.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "matrix.tpp"
#include "matrix_ops.tpp"
extern "C"
{
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
}

#if defined(_OPENMP)
#    include <omp.h>
#endif

#define sizeZ 300
#define sizeY 300
#define sizeX 300

#define Min 0
#define Max 9

#if defined(_WIN32) || defined(_WIN64)
#    define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#    define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define RAISE_ERROR_STL(p_strMessage)                                                                                                                          \
    do {                                                                                                                                                       \
        try {                                                                                                                                                  \
            std::stringstream strBuffer;                                                                                                                       \
            strBuffer << p_strMessage;                                                                                                                         \
            auto strMessage = strBuffer.str();                                                                                                                 \
            std::cout << __FILENAME__ << __PRETTY_FUNCTION__ << __LINE__ << strBuffer.str() << std::endl;                                                      \
        }                                                                                                                                                      \
        catch (...) {                                                                                                                                          \
        }                                                                                                                                                      \
        {                                                                                                                                                      \
        }                                                                                                                                                      \
    } while (false)

void fillRand(int ****A_, size_t sizeX_, size_t sizeY_, size_t sizeZ_, size_t sizeW_);
void fillRand(int ****A_, size_t sizeX_, size_t sizeY_, size_t sizeZ_, size_t sizeW_)
{
    unsigned int seed;
#pragma omp parallel
    {
#pragma omp for collapse(3) schedule(auto) private(seed)
        for (size_t w = 0; w < sizeW_; w++) {
            for (size_t z = 0; z < sizeZ_; z++) {
                for (size_t y = 0; y < sizeY_; y++) {
                    seed = 25234U + 16U * (unsigned int)omp_get_thread_num(); // 17U
                    for (size_t x = 0; x < sizeX_; x++) {
                        A_[w][z][y][x] = (rand_r(&seed) % (Max - Min + 1)) + Min;
                    }
                }
            }
        }
    }
}

void fillRand(int ***A_, size_t sizeX_, size_t sizeY_, size_t sizeZ_);
void fillRand(int ***A_, size_t sizeX_, size_t sizeY_, size_t sizeZ_)
{
    unsigned int seed;
#pragma omp parallel
    {
#pragma omp for collapse(2) schedule(auto) private(seed)
        for (size_t i = 0; i < sizeZ_; i++) {
            for (size_t j = 0; j < sizeY_; j++) {
                seed = 25234U + 16U * (unsigned int)omp_get_thread_num(); // 17U
                for (size_t k = 0; k < sizeX_; k++) {
                    A_[i][j][k] = (rand_r(&seed) % (Max - Min + 1)) + Min;
                }
            }
        }
    }
}

void fillRand(int **A_, size_t sizeX_, size_t sizeY_);
void fillRand(int **A_, size_t sizeX_, size_t sizeY_)
{
    unsigned int seed;
#pragma omp parallel
    {
#pragma omp for collapse(1) schedule(auto) private(seed)
        for (size_t j = 0; j < sizeY_; j++) {
            seed = 25234U + 16U * (unsigned int)omp_get_thread_num(); // 17U
            for (size_t k = 0; k < sizeX_; k++) {
                A_[j][k] = (rand_r(&seed) % (Max - Min + 1)) + Min;
            }
        }
    }
}

void fill(int **A_, size_t sizeX_, size_t sizeY_, int value_);
void fill(int **A_, size_t sizeX_, size_t sizeY_, int value_)
{

#pragma omp parallel for collapse(2) schedule(auto)
    for (size_t y = 0; y < sizeY_; y++) {
        for (size_t x = 0; x < sizeX_; x++) {
            A_[y][x] = value_;
        }
    }
}

void fill(int ***A_, size_t sizeX_, size_t sizeY_, size_t sizeZ_, int value_);
void fill(int ***A_, size_t sizeX_, size_t sizeY_, size_t sizeZ_, int value_)
{
#pragma omp parallel for collapse(3) schedule(auto)
    for (size_t z = 0; z < sizeZ_; z++) {
        for (size_t y = 0; y < sizeY_; y++) {
            for (size_t x = 0; x < sizeX_; x++) {
                A_[z][y][x] = value_;
            }
        }
    }
}

void fill(int ****A_, size_t sizeX_, size_t sizeY_, size_t sizeZ_, size_t sizeW_, int value_);
void fill(int ****A_, size_t sizeX_, size_t sizeY_, size_t sizeZ_, size_t sizeW_, int value_)
{
#pragma omp parallel for collapse(4) schedule(auto)
    for (size_t w = 0; w < sizeW_; w++) {
        for (size_t z = 0; z < sizeZ_; z++) {
            for (size_t y = 0; y < sizeY_; y++) {
                for (size_t x = 0; x < sizeX_; x++) {
                    A_[w][z][y][x] = value_;
                }
            }
        }
    }
}

int main()
{
    /*
        int ****D = aalloc(175, 175, 175, 175);
        adealloc(D, 175, 175, 175, 175);
    */
    /*
    int **M1 = my::cuda::aalloc<int>(3, 3);
    fillRand(M1, sizeX, sizeY);
    int **M2 = my::cuda::aalloc<int>(3, 3);
    my::cuda::copy(M2, M1, sizeX, sizeY);
    int **M3 = my::cuda::aalloc<int>(3, 3);
    matmult(M1, M2, M3, 3, 3, 3, 3);
    // multiply(M1, M2, M3);
    my::cuda::display<int>(M1, 3, 3);
    my::cuda::display<int>(M2, 3, 3);
    my::cuda::display<int>(M3, 3, 3);

    auto M4 = my::cuda::aalloc<int>(3 * 3);
    auto M5 = my::cuda::aalloc<int>(3 * 3);
    auto M6 = my::cuda::aalloc<int>(3 * 3);

    my::cuda::flatten1D<int>(M1, M4, (size_t)3, (size_t)3);

    my::cuda::flatten1D<int>(M2, M5, (size_t)3, (size_t)3);

    my::cuda::cpu_matrix_mult<int>(M4, M5, M6, 3);
    // my::cuda::cpu_matrix_mult<int>(M4, 3 ,3 ,M5, 3, 3, M6, 3, 3);

    my::cuda::display<int>(M4, 3 * 3);
    my::cuda::display<int>(M5, 3 * 3);
    my::cuda::display<int>(M6, 3 * 3);

    auto M7 = my::cuda::aalloc<int>(175 * 175 * 175 * 175);
    // memset(M7, 0, sizeof(M7) * 175*175*175*175);
    */

    int ***A = my::cuda::aalloc<int>(sizeX, sizeY, sizeZ);
    int ***B = my::cuda::aalloc<int>(sizeX, sizeY, sizeZ);
    int ***C = my::cuda::aalloc<int>(sizeX, sizeY, sizeZ);

// fillRand(A, sizeX, sizeY, sizeZ);
#pragma omp parallel for collapse(3) schedule(auto)
    for (size_t z = 0; z < sizeZ; z++) {
        for (size_t y = 0; y < sizeY; y++) {
            for (size_t x = 0; x < sizeX; x++) {
                A[z][y][x] = (x + y + z) / 2;
            }
        }
    }

// my::cuda::copy<int>(B, A, sizeX, sizeY, sizeZ);
#pragma omp parallel for collapse(3) schedule(auto)
    for (size_t z = 0; z < sizeZ; z++) {
        for (size_t y = 0; y < sizeY; y++) {
            for (size_t x = 0; x < sizeX; x++) {
                B[z][y][x] = (x + y + z) / 2;
            }
        }
    }
    RAISE_ERROR_STL("Hello... The following values " << 1 << " and " << 2 << " are wrong");
    // my::cuda::matMultFlat<int>(A, sizeX, sizeY, sizeZ, B, sizeX, sizeY, sizeZ, C, sizeX, sizeY, sizeZ);
    my::cuda::matMult<int>(A, sizeX, sizeY, sizeZ, B, sizeX, sizeY, sizeZ, C);
    // my::cuda::display<int>(A, sizeX, sizeY, sizeZ);
    // my::cuda::display<int>(C, sizeX, sizeY, sizeZ);

    my::cuda::adealloc<int>(A, sizeX, sizeY, sizeZ);
    my::cuda::adealloc<int>(B, sizeX, sizeY, sizeZ);
    my::cuda::adealloc<int>(C, sizeX, sizeY, sizeZ);
    print(1, ':', " Hello", ',', " ", "World!");
    return EXIT_SUCCESS;
}
