#include "neuralnet.hpp"
#include <cstdio>
#include <time.h>
int main() {
  srand(time(NULL));
  neural_network bug_brain = neural_network(20, 12);
  bug_brain.make_random_brain();
  bug_brain.to_file("runtime/brain.txt");

  return 0;
}