#include "lingeo3D.h"
#include <iostream>

using namespace lingeo3D;

int main(){

	int tri_n;
	std::cin >> tri_n;
	if(!std::cin.good()){
		std::cout << "Invalid input!\n";
		return 0;
	}

	polygon_t* triangles = new polygon_t[tri_n];

	for(int i = 0; i < tri_n; i++){
		for(int j = 0; j < 3; j++){
			float a, b, c;
			std::cin >> a >> b >> c;
			if(!std::cin.good()){
				std::cout << "Invalid input!\n";
				return 0;
			}
			triangles[i].add(point_t{a, b, c});
		}
		triangles[i].print();
	}


	for(int i = 0; i < tri_n; i++)
		for(int j = i; j < tri_n; j++)
			if(i != j && triangles[i].intersect(triangles[j]))
				std::cout << i << " " << j << std::endl;

	delete[] triangles;
	return 0;
}