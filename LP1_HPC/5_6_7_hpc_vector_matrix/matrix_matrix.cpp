#include<iostream>
#include <omp.h>
#include <time.h>

using namespace std;

int main() {
	int n_max_threads = omp_get_max_threads();
	cout << "Using " << n_max_threads << " (max) threads.\n\n";
	omp_set_num_threads(n_max_threads);
	
	int n;
	cout << "Enter size of matrix: ";
	cin >> n;
	int mat1[n * n], mat2[n * n], mat3[n * n];
	
	cout<<"INITIALIZING MATRICES .....\n";
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		#pragma omp parallel for
		for (int j = 0; j < n; j++) {
			mat1[n * i + j] = rand() % 10;
			mat2[n * i + j] = rand() % 10;
		}
	}
	
	//---------------------------------PARELLEL----------------------------------------------------
	clock_t start = clock();
	cout<<"MULTIPLYING MATRICES .....\n";
	
	#pragma omp parallel for 
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int sum = 0;
			for (int k = 0; k < n; k++) {
				sum += mat1[n * i + k] * mat2[n * k + j];
			}
			mat3[n * i + j] = sum;
		}
	}
	
	clock_t end = clock();
	cout << "\nParallel execution time: " << 1000 * (end - start) / CLOCKS_PER_SEC << " msecs\n";
	
	//---------------------------------SERIAL----------------------------------------------------
	clock_t start1 = clock();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int sum = 0;
			for (int k = 0; k < n; k++) {
				sum += mat1[n * i + k] * mat2[n * k + j];
			}
			mat3[n * i + j] = sum;
		}
	}
	
	clock_t end1 = clock();
	cout << "\nSerial execution time: " << 1000 * (end1 - start1) / CLOCKS_PER_SEC << " msecs\n";	
	return 0;
}