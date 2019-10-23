#include <iostream>
#include <omp.h>
#include <time.h>

using namespace std;

int main() {
	int n;
	cout << "Enter number of elements: ";
	cin >> n;
	int a[n];
	
	int n_max_threads = omp_get_max_threads();
	omp_set_num_threads(n_max_threads);
	
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		a[i] = rand() % 100;
	}
	
	cout << "\nArray: ";
	for (int i = 0; i < n; i++) {
		cout << a[i] << " ";
	}
	cout << endl;
	
	clock_t start = clock();
	
	for (int i = 0; i < n; i++) {
		#pragma omp parallel for
		for (int j = i%2; j < n-1; j += 2) {
			if (a[j] > a[j+1]) {
				int temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
			}
		}
	}
	
	clock_t end = clock();
	
	cout << "\nSorted values: ";
	for (int i = 0; i < n; i++) {
		cout << a[i] << " ";
	}
	cout << endl;
	
	cout << "\nExecution time: " << 1000 * (end - start) / CLOCKS_PER_SEC << " msecs\n";
	
	return 0;
}
