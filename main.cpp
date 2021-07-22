#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>


//So we don't need to write std:: everywhere
using namespace std;



#include "draw.cpp"
#include "naive.cpp"
#include "preprocess.cpp"





int main(int argc, const char* argv[]){
	//local variables
	int numOfStacks = 2; 
	int rowsInStack = 3;
	int columnsInStack = 35;
	int widthOfLine = 5;
	int widthOfAV = 2;
	int lengthOfAV = 6;

	getData(&numOfStacks, &rowsInStack, &columnsInStack, 
				&widthOfLine, &widthOfAV, &lengthOfAV);
	
	K_Stack *stacks = NULL;
	stacks = (K_Stack *)malloc(numOfStacks*sizeof(K_Stack));
	stacks->k = numOfStacks;
	buildStack(stacks, numOfStacks, rowsInStack, columnsInStack);

	//printStacks(stacks, numOfStacks, rowsInStack, columnsInStack);
	
	//vector<point> trajectory = getTrajectory(argc, argv);
	return 0;
}