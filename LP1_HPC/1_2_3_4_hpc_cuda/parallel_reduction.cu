#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

__global__ void par_min(float* input) {
	const int tid = threadIdx.x;
	int no_threads = blockDim.x;
	int step = 1;
	
	while (no_threads > 0) {
		if (tid < no_threads) {
			int i1 = tid * step * 2;
			int i2 = i1 + step;
			if (input[i1] > input[i2])
				input[i1] = input[i2];
		}
		no_threads >>= 1;
		step <<= 1;
	}
}

__global__ void par_max(float* input) {
	const int tid = threadIdx.x;
	int no_threads = blockDim.x;
	int step = 1;
	
	while (no_threads > 0) {
		if (tid < no_threads) {
			int i1 = tid * step * 2;
			int i2 = i1 + step;
			if (input[i1] < input[i2])
				input[i1] = input[i2];
		}
		step <<= 1;
		no_threads >>= 1;
	}
}

__global__ void par_sum(float* input) {
	const int tid = threadIdx.x;
	int no_threads = blockDim.x;
	int step = 1;
	
	while (no_threads > 0) {
		if (tid < no_threads) {
			int i1 = tid * step * 2;
			int i2 = i1 + step;
			input[i1] += input[i2];
		}
		step <<= 1;
		no_threads >>= 1;
	}
}

__global__ void par_std(float* input, float avg) {
	const int tid = threadIdx.x;
	int no_threads = blockDim.x;
	int step = 1;
	
	while (no_threads > 0) {
		if (tid < no_threads) {
			int i1 = tid * step * 2;
			int i2 = i1 + step;
			input[i1] = (input[i1] - avg) * (input[i1] - avg);
			input[i2] = (input[i2] - avg) * (input[i2] - avg);
			input[i1] += input[i2];
		}
		step <<= 1;
		no_threads >>= 1;
	}
}

int main() {
	srand(time(NULL));
	const int N = 1<<7;
	float *a, *dev_min, *dev_max, *dev_sum, *dev_std;
	const int size = N * sizeof(float);
	clock_t t;
	float result;
	
	a = (float*) malloc(size);
	printf("Array: of %d", N);
	for (int i = 0; i < N; i++) {
		a[i] = rand() % N + 1;
		printf("%f ", a[i]);
	}
	
	//-------------------Min----------------------
	cudaMalloc(&dev_min, size);
	cudaMemcpy(dev_min, a, size, cudaMemcpyHostToDevice);
	t = clock();
	par_min<<<1, N/2>>>(dev_min);
	cudaMemcpy(&result, dev_min, sizeof(float), cudaMemcpyDeviceToHost);
	t = clock() - t;
	printf("\n\nMinimum value: %f\ttime taken: %f milliseconds\n", result, (1000 * (double) t / CLOCKS_PER_SEC));
	
	//-------------------Max----------------------
	cudaMalloc(&dev_max, size);
	cudaMemcpy(dev_max, a, size, cudaMemcpyHostToDevice);
	t = clock();
	par_max<<<1, N/2>>>(dev_max);
	cudaMemcpy(&result, dev_max, sizeof(float), cudaMemcpyDeviceToHost);
	t = clock() - t;
	printf("\nMaximum value: %f\ttime taken: %f milliseconds\n", result, (1000 * (double) t / CLOCKS_PER_SEC));
	
	//-------------------Sum----------------------
	cudaMalloc(&dev_sum, size);
	cudaMemcpy(dev_sum, a, size, cudaMemcpyHostToDevice);
	t = clock();
	par_sum<<<1, N/2>>>(dev_sum);
	cudaMemcpy(&result, dev_sum, sizeof(float), cudaMemcpyDeviceToHost);
	t = clock() - t;
	printf("\nSum: %f\ttime taken: %f milliseconds\n", result, (1000 * (double) t / CLOCKS_PER_SEC));
	
	//-------------------Avg----------------------
	cudaMalloc(&dev_sum, size);
	cudaMemcpy(dev_sum, a, size, cudaMemcpyHostToDevice);
	t = clock();
	par_sum<<<1, N/2>>>(dev_sum);
	cudaMemcpy(&result, dev_sum, sizeof(float), cudaMemcpyDeviceToHost);
	result = result / N;
	t = clock() - t;
	printf("\nAverage: %f\ttime taken: %f milliseconds\n", result, (1000 * (double) t / CLOCKS_PER_SEC));
	
	//-------------------Std----------------------
	cudaMalloc(&dev_std, size);
	cudaMemcpy(dev_std, a, size, cudaMemcpyHostToDevice);
	t = clock();
	par_std<<<1, N/2>>>(dev_std, result);
	cudaMemcpy(&result, dev_std, sizeof(float), cudaMemcpyDeviceToHost);
	result = sqrt(result / N);
	t = clock() - t;
	printf("\nStandard deviation: %f\ttime taken: %f milliseconds\n", result, (1000 * (double) t / CLOCKS_PER_SEC));
	
	// clean up
	cudaFree(dev_min);
	cudaFree(dev_max);
	cudaFree(dev_sum);
	cudaFree(dev_std);
	delete[] a;
	
	return 0;
}

