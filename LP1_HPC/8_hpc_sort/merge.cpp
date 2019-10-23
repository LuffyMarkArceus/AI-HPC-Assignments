#include <iostream>
#include <omp.h>
#include <time.h>

using namespace std;

void merge(int a[], int low, int mid, int high);
void mergesort(int a[], int low, int high);

int main() {
	int n_max_threads = omp_get_max_threads();
	cout << "Using " << n_max_threads << " (max) threads\n\n";
	int n;
	cout << "Enter number of elements: ";
	cin >> n;
	int a[n];
	
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
	
	mergesort(a, 0, n-1);
	
	clock_t end = clock();
	
	cout << "\nSorted values: ";
	for (int i = 0; i < n; i++) {
		cout << a[i] << " ";
	}
	cout << endl;
	
	cout << "\nExecution time: " << 1000 * (end - start) / CLOCKS_PER_SEC << " msecs\n";
	
	return 0;
}

void merge(int a[], int low, int mid, int high) {
	int n = high - low + 1;
	int temp[n];
	int i = low;
	int j = mid + 1;
	int k = 0;
	
	while (i <= mid && j <= high) {
		if (a[i] <= a[j]) {
			temp[k++] = a[i++];
		} else {
			temp[k++] = a[j++];
		}
	}
	
	while (i <= mid) {
		temp[k++] = a[i++];
	}
	
	while (j <= high) {
		temp[k++] = a[j++];
	}
	
	#pragma omp parallel for
	for (i = low; i <= high; i++) {
		a[i] = temp[i-low];
	}
}

void mergesort(int a[], int low, int high) {
	if (low < high) {
		int mid = (low + high) / 2;
		#pragma omp parallel sections 
		{
			#pragma omp section 
			{
				mergesort(a, low, mid);
			}
			#pragma omp section 
			{
				mergesort(a, mid+1, high);
			}
		}
		merge(a, low, mid, high);
	}
}

