
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
//  Created: 16, March, 2021                                //
//  Modified: 17, March, 2021                               //
//  file: kernel.h                                          //
//  Crypto                                                  //
//  Source: https://stackoverflow.com/questions/13553015/cuda-c-linker-error-undefined-reference                                                //
//          https://www.olcf.ornl.gov/tutorials/cuda-vector-addition/                                                //
//          https://gist.github.com/AndiH/2e2f6cd9bccd64ec73c3b1d2d18284e0
//          https://stackoverflow.com/a/14038590/10152334
//  CPU: ALL                                                //
//                                                          //
//////////////////////////////////////////////////////////////

#ifndef MY_CUDA_HPP
#define MY_CUDA_HPP

#include <cuda.h>
#include <cuda_runtime.h>
#include <iostream>
#include "stdio.h"

#ifndef BLOCK_SIZE
#    define BLOCK_SIZE 16
#endif

#define gpuErrchk(ans)                                                                                                                                         \
    {                                                                                                                                                          \
        gpuAssert((ans), __FILE__, __LINE__);                                                                                                                  \
    }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true)
{
    if (code != cudaSuccess) {
        fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort)
            exit(code);
    }
}

namespace my
{
namespace cuda
{

// 1D vector
void vecAdd(size_t gridSize, size_t blockSize, double *a, double *b, double *c, size_t n);
void vecSub(size_t gridSize, size_t blockSize, double *a, double *b, double *c, size_t n);
void vecMult(size_t gridSize, size_t blockSize, double *a, double *b, double *c, size_t n);
void vecDiv(size_t gridSize, size_t blockSize, double *a, double *b, double *c, size_t n);

// 2D vector
void matrixMultiplySimple(dim3 gridSize, dim3 blockSize, float *a, float *b, float *c, size_t n);
void matrixMultiplyOptimised(dim3 gridSize, dim3 blockSize, float *a, float *b, float *c, size_t n);
} // namespace cuda
} // namespace my

#endif