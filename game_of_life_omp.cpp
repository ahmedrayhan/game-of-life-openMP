#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <ctime>
#include <fstream>
using namespace std;

bool* initiate_first_generation(int size);
int analyze_cell(bool* current_generation,int x, int y, int size);
void copy_matrix(bool* current_generation, bool* next_generation,int size);
void print_martix( bool* current_generation, int size);

int main(int argc, char *argv[])
{
	srand(time(0));
	int iterations = atoi(argv[3]);
	ifstream file(argv[1]);
	ofstream output("output.txt");
	int size;
	file>>size>>size;
	output<<size<<" "<<size<<endl;
	int total_threads = atoi(argv[2]);
	double start_time = 0, end_time = 0;
	
	bool* next_generation = new bool[size*size];
	bool* current_generation = new bool[size*size];//initiate_first_generation(size);
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			file >> current_generation[i*size+j];
		}
	}
	start_time = omp_get_wtime();
	for(int i = 0; i<iterations; i++)
	{
		#pragma omp parallel num_threads(total_threads)
		{
			int current_thread = omp_get_thread_num();
			int iterations_per_thread = size /total_threads;
			int my_start = current_thread * iterations_per_thread;
			int my_end = 0;
			current_thread == total_threads-1 ? my_end = size : my_end = my_start + iterations_per_thread;
			for(int i = my_start; i<my_end; i++)
			{
				for(int j = 0; j<size; j++)
				{
					next_generation[i*size+j] = analyze_cell(current_generation,i, j, size);
				}
			}
		}
		copy_matrix(current_generation, next_generation,size);
		output<<"Generation: "<< i+1<<endl;
		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				output<<current_generation[i*size+j]<<" ";
			}output<<endl;
		}output<<endl;
	}

	end_time = omp_get_wtime();
	cout << "Total execution time = " << end_time-start_time << endl;
}

bool* initiate_first_generation(int size) //initializing first generation
{
	bool* generation = new bool [size*size];
	for(int i = 0; i < size*size; i++) {
		generation[i] = rand()%2;
	}
	return generation;

}
int analyze_cell(bool* current_generation,int x, int y, int size)
{
// 	//calculating neighbors
	int alive_neighbours = 0;
	int left = current_generation[x*size+((y-1+size)%size)]; //left cell
	int right = current_generation[x*size+((y+1+size)%size)]; //right cell
	int top = current_generation[(((x-1+size)%size)*size)+y]; //top cell information received from previous process
	int bottom = current_generation[((x+1+size)%size)*size+y]; //bottom cell information received from next process
	int topleft =current_generation[(((x-1+size)%size)*size)+((y-1+size)%size)];
	int topright = current_generation[(((x-1+size)%size)*size)+((y+1+size)%size)];
	int bottomleft = current_generation[((x+1+size)%size)*size+((y-1+size)%size)];
	int bottomright = current_generation[((x+1+size)%size)*size+((y+1+size)%size)];	
	alive_neighbours = left +right+top+bottom+topleft+topright+bottomleft+bottomright;

	if(alive_neighbours < 2 || alive_neighbours > 3) 
		return 0;
 	else if(current_generation[x*size+y] == 1 && (alive_neighbours == 2 || alive_neighbours == 3))
		return 1;
	else if(current_generation[x*size+y] == 0 && alive_neighbours == 3)
		return 1;
	else return 0;
}

void copy_matrix(bool* current_generation, bool* next_generation,int size) //copy temporary matrix to original
{

	for(int i = 0; i<size*size; i++)
	{
		current_generation[i] = next_generation[i];
	}
}