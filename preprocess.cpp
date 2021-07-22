#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>
#include <stdio.h>
//#include "BuildPolygonState.cpp"
#include "RefinePolygons.cpp"

#ifndef _PREPROCESS_
#define _PREPROCESS_

using namespace std;


void setConfig(int argc, const char* argv[]){
	for(int i = 1 ; i < argc ; i++ ){
		string s(argv[i]);	
		if(s.compare("-k")==0){
			config.k = atoi(argv[i+1]);
		}
		if(s.compare("-p") == 0){
			config.printGraph = true;
			if(argc-1>i){
				string temp(argv[i+1]);

				if(temp.find_first_not_of( "0123456789" ) == string::npos){
					config.printLevel = atoi(argv[i+1]);
				}
			}
		}
	}
}

int getTime(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end){
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

vector<point> getTrajectory(int argc, const char* argv[]){
	//max_x=max_y=min_y=min_x=0;

	//setConfig(argc,argv);

	//Create variables for holding start and endpoint plus the test title
	point start, end;
	string testTitle;

	//Create vector for containing the linesegments of the polygons
	vector< vector < lineSegment> > polygons;

	//Create vector for containing the points of the polygons
	vector<point> points;

	//Call function that parses the file input
	readInput(start, end, testTitle, polygons, points);
	//std::cout<<"File: "<<testTitle<<std::endl;

	vector< vector < int > > graph;
	vector< vector < double > > graphDistance;

	//Vector so we can backtrack the route
	vector<int> route;

	vector< vector < int > > crossesNumber;


	//Get how many points we have
	size_t numberOfPoints = points.size();

	//Create a two dimenstional vector for the graph
	
	size_t dimension = numberOfPoints*(config.k+1);
	graph.resize(dimension,vector<int>());
	graphDistance.resize(dimension,vector<double>());

   auto time1 = std::chrono::steady_clock::now();

	//Call function that calculate the distance
	calculateNumberOfCrossings(crossesNumber, polygons, points);

   auto time2 = std::chrono::steady_clock::now();

	makeVisabilityGraph(graph, graphDistance, crossesNumber, points);

   auto time3 = std::chrono::steady_clock::now();


	//The graph is constructed call dijksta to calculate the distance
	double distance = dijkstra(graphDistance,graph,route, points);

   auto time4 = std::chrono::steady_clock::now();
	//Output the distance
	
	if(config.printGraph){
		//draw("test/test.svg", testTitle,start,end, polygons,distance,points,route,graph);
		return vector<point>();	
	}
	else{
		std::cout<<points.size()<<std::endl;
		for(int i = 0; i < points.size(); i++)
		{
			std::cout<<"("<<points.at(i).x<<", "<<points.at(i).y<<") ";
		}
		cout<<"\n";
		//cout << distance << endl;
		cout << getTime(time1,time2) << " " << getTime(time2,time3) << " " << getTime(time3,time4) << " " << distance << endl;
		//cout << "Dijkstra took : " << getTime(visibility_end,dijkstra_end) << endl;
		return points;
	}	
}


void buildStack(K_Stack *stacks, int numStacks, int numOfRows, int numOfCol){
	for(int i = 0; i < numStacks; i++){
		int **slotsOfAVs = NULL;
		slotsOfAVs = (int **)malloc(sizeof * slotsOfAVs * numOfRows);
		for(int j = 0; j < numOfRows; j++){
			slotsOfAVs[j] = (int *)malloc(sizeof * slotsOfAVs[i] * numOfCol);
			for(int l = 0; l < numOfCol; l++){
				slotsOfAVs[j][l] = 1;
			}
		}
		stacks[i].slotsOfAV = slotsOfAVs;
	}
}

int countAVs(int** arrayOfAVs, int rowsInStack, int columnsInStack){
	int count = 0;
	for(int i = 0; i < rowsInStack; i++){
		for(int j = 0; j < columnsInStack; j++){
			if(arrayOfAVs[i][j] == 1 || arrayOfAVs[i][j] == -1 ){
				count++;
			}
		}
	}
	return count;
}

int countAllStacks(K_Stack *stacks, 
			int numOfStacks, int rowsInStack, int columnsInStack){
	int sum = 0;
	int AVsInAStack = 0;
	for(int i = 0; i < numOfStacks; i++){

		AVsInAStack = countAVs(stacks[i].slotsOfAV, rowsInStack, columnsInStack);
		sum += AVsInAStack;
	}
	return sum;
}

void readKStacks(vector< vector< lineSegment> > &polygons, K_Stack *stacks, 
			int numOfStacks, int rowsInStack, int columnsInStack){
	
	for(int i = 0; i < numOfStacks; i++){

		vector<lineSegment> polygon;
		

		int count = countAVs(stacks[i].slotsOfAV, rowsInStack, columnsInStack);
		int checkedAVs = 0;
		int r = 0, c = 0;
		while(checkedAVs <= count){
			//InitState* state = new InitState(0, 0);
			//state->makeDecision(stacks[i].slotsOfAV, rowsInStack, columnsInStack);
			//state = Factory::getNextState(state);

			//if(!polygon.empty()){
			//		polygons.push_back(polygon);
			//}
		}
	}
}

void printStacks(K_Stack *stacks, int numOfStacks, int rowsInStack, int columnsInStack){
	cout<<"\n"<<endl;
	for(int i = 0; i < numOfStacks; i++){
		for(int j = 0; j < rowsInStack; j++){
			cout<<"\t";
			for(int l = 0; l < columnsInStack; l++){
				//cout<<stacks[i].slotsOfAV[j][l]<<" ";
				printf("%2d ", stacks[i].slotsOfAV[j][l]);
			}
			cout<<endl;
		}
		cout<<"\n"<<endl;
	}
}

#endif