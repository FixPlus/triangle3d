#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "lingeo3D.h"

using namespace lingeo3D;


/*
	              TRIANGLE GENERATOR
	  generates a list of *n_tri* triangles that lay in bounds (0.0,0.0,0.0) - (bounds, bounds, bounds)
	  and are not bigger than cube (abs_size, abs_size, abs_size)

*/

template<typename T>
T random_num_in_bounds(T lower_bound, T upper_bound){
	return lower_bound + (T)std::rand()/(const T)RAND_MAX * (upper_bound - lower_bound);
}

template<typename T>
polygon_t<T> generate_triangle(T bounds, T size){
	polygon_t<T> ret;
	T actual_size = bounds * size;
	point_t<T> upper_bounds{random_num_in_bounds(actual_size, bounds), random_num_in_bounds(actual_size, bounds), random_num_in_bounds(actual_size, bounds)};
	point_t<T> lower_bounds{upper_bounds.x_ - actual_size, upper_bounds.y_ - actual_size, upper_bounds.z_ - actual_size};
	for (int j = 0; j < 3; j++)
	{
		point_t<T> vert{random_num_in_bounds(lower_bounds.x_, upper_bounds.x_),
					 random_num_in_bounds(lower_bounds.y_, upper_bounds.y_),
					 random_num_in_bounds(lower_bounds.z_, upper_bounds.z_),};
		ret.add(vert);
	}
	return ret;
}

int main(int argc, char** argv){
	if(argc != 4){
		std::cout << "Usage: count bounds size\n";
		return 0;
	}
	int randmax = RAND_MAX;
	int n_tri = std::atoi(argv[1]);
	int bounds = std::atoi(argv[2]);
	int abs_size = std::atoi(argv[3]);
	std::srand(std::time(nullptr));
	std::cout << n_tri << std::endl;
	for(int i = 0; i < n_tri; i++){
		polygon_t<float> tri = generate_triangle((float)bounds, (float)abs_size/(float)bounds);
		tri.print();
	}

}