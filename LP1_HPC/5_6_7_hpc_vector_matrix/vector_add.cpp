#include <iostream>
#include <omp.h>
#include <time.h>

using namespace std;

int main() {
	int n_max_threads = omp_get_max_threads();
	cout << "Using " << n_max_threads << " (max) threads.\n\n";
	
	int n;
	cout << "Enter size of vectors: ";
	cin >> n;
	int v1[n], v2[n], v3[n];
	
	omp_set_num_threads(n_max_threads);
	
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		v1[i] = rand() % 1000;
		v2[i] = rand() % 1000;
	}
	
	clock_t start = clock();
	
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		v3[i] = v1[i] + v2[i];
	}
	
	clock_t end = clock();
	
	cout << "Vector after addition: ";
	for (int i = 0; i < n; i++) {
		cout << v3[i] << " ";
	}
	
	cout << "\n\nExecution time: " << 1000 * (end - start) / CLOCKS_PER_SEC << " msecs\n";
	
	return 0;
}
