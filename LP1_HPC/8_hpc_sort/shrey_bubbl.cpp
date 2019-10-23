#include<iostream>
#include<omp.h>
#include<time.h>
using namespace std;

int main(int argc, char const *argv[])
{
	int n;
	cout<<"Enter the size of Array: ";
	cin>>n;
	int a[n];

	int n_max_threads = omp_get_max_threads();
	omp_set_num_threads(n_max_threads);

	#pragma omp parellel for
	for (int i = 0; i < n; i++)
	{
		a[i] = rand() % n + 1;
		cout<<"\t"<<a[i];
	}
	cout<<endl<<endl;

	
	clock_t start = clock();
	for (int j = 0; j < n; j++)
	{
		#pragma omp parellel for
		for (int i = j%2; i < n-1; i+=2)
		{
			if (a[i] > a[i+1])
			{
				int temp = a[i];
				a[i] = a[i+1];
				a[i+1] = temp;
			}
		}
	}
	clock_t end = clock();

	cout << "parellel exec time : " << (float)(end-start) / CLOCKS_PER_SEC<<"milliseconds"<<endl<<endl;
	#pragma omp parellel for
	for (int i = 0; i < n; i++)
		cout<<"\t"<<a[i];
	return 0;
}