
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
//  CPU: ALL                                                //
//                                                          //
//////////////////////////////////////////////////////////////

#ifndef MY_CUDA_MATRIX_H
#define MY_CUDA_MATRIX_H

#include <cuda.h>
#include <cuda_runtime.h>
#include "stdio.h"

#ifndef BLOCK_SIZE
#    define BLOCK_SIZE 16
#endif

void matrixAdd(const dim3 *grid, const dim3 *threads, int *a, int *b, int *c, size_t n);

void matrixMultiplyShared(const dim3 *grid, const dim3 *threads, float *a, float *b, float *c, int n);

void sharedABMultiply(const dim3 *grid, const dim3 *threads, float *a, float *b, float *c, int n);

void MatrixMulCUDA(const dim3 *grid, const dim3 *threads, float *A, float *B, float *C, size_t wA, size_t wB);

void matFill(const dim3 *grid, const dim3 *threads, int *matA, int value, size_t sizeAX, size_t sizeAY);
#endif