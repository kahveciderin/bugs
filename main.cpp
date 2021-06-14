#include "neuralnet.hpp"
#include <cstdio>
int main(){
	srand (time(NULL));
	for(int i = 0; i < 10; i++){
		printf("%d\t%f\n", i, gaussian_dist(i, 0.5, 4));
	}
	return 0;
}