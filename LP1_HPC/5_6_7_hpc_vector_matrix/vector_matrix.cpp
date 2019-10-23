#include <iostream>
#include <omp.h>
#include <time.h>

using namespace std;

int main() {
	int n_max_threads = omp_get_max_threads();
	cout << "Using " << n_max_threads << " (max) threads.\n\n";
	omp_set_num_threads(n_max_threads);
	
	int n;
	cout << "Enter size of vector: ";
	cin >> n;
	int v1[n], v2[n], mat[n][n];
	
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		v1[i] = rand() % 10;
		v2[i] = 0;
		for (int j = 0; j < n; j++) {
			mat[i][j] = rand() % 10;
		}
	}
	
	cout << "\nVector:\n[ ";
	for (int i = 0; i < n; i++) {
		cout << v1[i] << " ";
	}
	cout << "]\n\n";
	
	cout << "Matrix:\n";
	for (int i = 0; i < n; i++) {
		cout << "[ ";
		for (int j = 0; j < n; j++) {
			cout << mat[i][j] << " ";
		}
		cout << "]\n";
	}
	
	clock_t start = clock();
	
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			v2[i] += mat[i][j] * v1[j];
		}
	}
	
	clock_t end = clock();
	
	cout << "\nVector after multiplication:\n[ ";
	for (int i = 0; i < n; i++) {
		cout << v2[i] << " ";
	}
	cout << "]\n\n";
	
	cout << "Execution time: " << 1000 * (end - start) / CLOCKS_PER_SEC << " msecs\n";
	
	return 0;
}
