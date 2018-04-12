openmp:
	g++ -fopenmp game_of_life_omp.cpp -o game_of_life_omp

clean:
	rm game_of_life_omp 