/*
Author: Shu Gong
Class: ECE6122 QSZ
Last Date Modified: 11/12/2022

Description:

a C\C++ program using CUDA to solve the steady state temperature distribution in the thin plate.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include<iostream>
#include "device_launch_parameters.h"
#include<fstream>
#include<string>
#include <iomanip>

#include <unistd.h>
#include <sys/types.h>
using namespace std;

/******************************************************************/
// @brief: set up dimGrid
// @para: a-> nInterior, b->dimBlock
/******************************************************************/
inline int iDivUp(int a, int b)
{
    return (a % b != 0) ? (a / b + 1) : (a / b);
}

/******************************************************************/
// @brief: use CUDA to calculate temperature
// @para: g,h are device memory
/******************************************************************/
__global__ void temperatureCalc(double* g, double* h, int nInterior)
{
    int NX = nInterior + 2;
    const int i = blockIdx.x * blockDim.x + threadIdx.x;
    const int j = blockIdx.y * blockDim.y + threadIdx.y;
    //                         N 
    int P = i + j * NX;             // node (i,j)              |
    int N = i + (j + 1) * NX;       // node (i,j+1)            |
    int S = i + (j - 1) * NX;       // node (i,j-1)     W ---- P ---- E
    int E = (i + 1) + j * NX;       // node (i+1,j)            |
    int W = (i - 1) + j * NX;       // node (i-1,j)            |
    //                         S 

// --- Only update "interior" (not boundary) node points
    if (i > 0 && i < nInterior + 1 && j>0 && j < nInterior + 1)
        g[P] = 0.25 * (h[E] + h[W] + h[N] + h[S]);

}

int main(int argc, char* argv[])
{

    int nInterior;
    int nIteration;
    extern char* optarg;
    extern int optind, opterr, optopt;

    int c;
    const char* optstring = "n:I:";
    while ((c = getopt(argc, argv, optstring)) != -1)
    {
        switch (c)
        {
        case 'n':
            nInterior = atoi(optarg);
            if (nInterior <= 0 || !nInterior)
            {
                printf("Invalid parameters, please check your values.");
            }
            break;
        case 'I':
            nIteration = atoi(optarg);
            if (nIteration <= 0 || !nIteration)
            {
                printf("Invalid parameters, please check your values.");
            }
            break;
        case '?':
            printf("error optopt: %c\n", optopt);
            printf("error opterr: %d\n", opterr);
            break;
        }
    }


    int N = (nInterior + 2) * (nInterior + 2);

    double* g, * h, * out;
    double* d_g, * d_h;

    // Allocate memory
    g = (double*)calloc(N, sizeof(double));
    h = (double*)calloc(N, sizeof(double));
    out = (double*)malloc(sizeof(double) * N);

    // Initialize array
    for (int i = 0; i < N; i++)
    {
        if (i < 7 * (nInterior) / 10 && i>3 * (nInterior) / 10)
        {
            h[i] = 100.0;
            g[i] = 100.0;
        }
        else
        {
            h[i] = 20.0;
            g[i] = 20.0;
        }
    }

    // Allocate device memory 
    cudaMalloc((void**)&d_g, sizeof(double) * N);
    cudaMalloc((void**)&d_h, sizeof(double) * N);

    // Transfer data from host to device memory
    cudaMemcpy(d_g, g, sizeof(double) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(d_h, d_g, sizeof(double) * N, cudaMemcpyDeviceToDevice);

    // Executing kernel 
    dim3 dimBlock(15, 15);
    dim3 dimGrid(iDivUp((nInterior + 2), 15), iDivUp((nInterior + 2), 15));

    cudaEvent_t startEvent, stopEvent;
    cudaEventCreate(&startEvent);
    cudaEventCreate(&stopEvent);
    cudaEventRecord(startEvent, 0);

    for (int k = 0; k < nIteration; k = k + 2)
    {
        temperatureCalc << <dimGrid, dimBlock >> > (d_g, d_h, nInterior);
        temperatureCalc << <dimGrid, dimBlock >> > (d_h, d_g, nInterior);
    }

    cudaEventRecord(stopEvent, 0);
    cudaEventSynchronize(stopEvent);
    float time;
    cudaEventElapsedTime(&time, startEvent, stopEvent);

    std::cout << "Thin plate calculation took " << time << " milliseconds." << std::endl;

    // Transfer data back to host memory
    cudaMemcpy(out, d_g, sizeof(double) * N, cudaMemcpyDeviceToHost);

    fstream outFile;
    outFile.open("finalTemperatures.csv", ios::out);
    for (int i = 0; i < nInterior + 2; i++)
    {
        for (int j = 0; j < nInterior + 2; j++)
        {
            outFile << std::setprecision(15) << out[j + i * (nInterior + 2)] << ",";
        }
        outFile << endl;
    }
    outFile.close();


    // Deallocate device memory
    cudaFree(d_g);
    cudaFree(out);

    // Deallocate host memory
    free(g);
    free(h);
}
