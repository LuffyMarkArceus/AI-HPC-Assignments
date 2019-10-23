#include <iostream>
#include <mpi.h>
#include <string.h>
#include <fstream>
#include <math.h>

using namespace std;

float euclidean(float x1[1][4], float x2[1][4]) {
	float distance = 0;
	for (int i = 0; i < 4; i++) {
		distance = pow(x1[0][i] - x2[0][i], 2);
	}
	return sqrt(distance);
}

string classify(float dist[][2], int K, string iris_data_raw[][5]) {
	int c1 = 0;
	int c2 = 0;
	int c3 = 0;
	
	string class1 = "Iris-setosa";
	string class2 = "Iris-versicolor";
	string class3 = "Iris-virginica";
	
	string result = "";
	
	for (int i = 0; i < K; i++) {
		if (iris_data_raw[(int) dist[i][0]][4] == class1) {
			c1++;
		} else if (iris_data_raw[(int) dist[i][0]][4] == class2) {
			c2++;
		} else {
			c3++;
		}
	}
	
	if (c1 > c2) {
		if (c1 > c3) {
			result = class1;
		} else {
			result = class3;
		}
	} else {
		if (c2 > c3) {
			result = class2;
		} else {
			result = class3;
		}
	}
	
	return result;
}

void process(int my_id, int K, int examples[20], string iris_data_raw[][5], float iris_data[][4]) {
	int example_num = examples[my_id];
	float distances[150][2];		// column 1 stores index, column 2 stores euclidean distance
	
	for (int i = 0; i < 150; i++) {
		distances[i][0] = i;
		if (i == example_num) {
			// dist to self set to high number to avoid bias
			distances[i][1] = 1000;
		} else {
			distances[i][1] = euclidean(&iris_data[example_num], &iris_data[i]);
		}
	}
	
	// bubble sort distances
	for (int i = 0; i < 150; i++) {
		for (int j = 0; j < 150-i-1; j++) {
			if (distances[j][1] > distances[j+1][1]) {
				float t_dist = distances[j][1];
				float t_index = distances[j][0];
				distances[j][0] = distances[j+1][0];
				distances[j][1] = distances[j+1][1];
				distances[j+1][0] = t_index;
				distances[j+1][1] = t_dist;
			}
		}
	}
	
	cout << "Thread: " << my_id << " | The example " << examples[my_id] << " belongs to the class: " << classify(distances, K, iris_data_raw) << endl;
}

int main(int argc, char **argv) {
	string iris_data_raw[150][5];
	float iris_data[150][4];
	
	ifstream iris_file;
	int K = 13;		// K = sqrt(N), here N = 150
	int examples[20];		// 20 random samples to be classified
	
	for (int i = 0; i < 20; i++) {
		examples[i] = rand() % 150;
	}
	
	iris_file.open("iris.data");
	
	for (int i = 0; i < 150; i++) {
		for (int j = 0; j < 5; j++) {
			getline(iris_file, iris_data_raw[i][j], ',');
		}
	}
	
	const clock_t begin = clock();
	int my_id, num_procs;
	
	MPI_Status stat;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	
	// Master
	if (my_id == 0) {
		char parse_to_float[3];
		for (int i = 0; i < 150; i++) {
			for (int j = 0; j < 4; j++) {
				iris_data[i][j] = atof(iris_data_raw[i][j].c_str());
			}
		}
		
		for (int i = 1; i < num_procs; i++) {
			cout << "Sending data to " << i << endl;
			MPI_Send(iris_data, 600, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
		}
		
		process(my_id, K, examples, iris_data_raw, iris_data);
	} 
	else {
		float iris_data_recvd[150][4];
		MPI_Recv(iris_data_recvd, 600, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &stat);
		cout << "Received data by " << my_id << endl;
		
		process(my_id, K, examples, iris_data_raw, iris_data_recvd);
	}
	
	MPI_Finalize();
	
	cout << "Time taken by " << my_id << ": " << ((float) clock() - begin) / CLOCKS_PER_SEC << endl;

	return 0;
}
