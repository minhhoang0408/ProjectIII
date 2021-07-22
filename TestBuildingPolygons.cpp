#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>
//#include "BuildPolygonState.cpp"
#include "draw.cpp"
#include "naive.cpp"
//#include "preprocess.cpp"
#include "MovementController.cpp"
#include "PathPlanning.cpp"
#include "BuiltInMap.cpp"
#include <cassert>  
#include <stdio.h>
#ifdef __linux__ 
    //linux code goes here
#include <stdio_ext.h> // __fpurge(stdin);
#endif


//So we don't need to write std:: everywhere
using namespace std;

void getPathPlanning(K_Stack *stacks, int numOfStacks, int rowsInStack, int columnsInStack, BuildingPolygons* generator);

void testFullAVsInStacks(){
    int numOfStacks = 1; 
	int rowsInStack = 6;
	int columnsInStack = 21;
	int widthOfLine = 5;
	int widthOfAV = 2;
	int lengthOfAV = 6;

    K_Stack *stacks = NULL;
	stacks = (K_Stack *)malloc(numOfStacks*sizeof(K_Stack));
	stacks->k = numOfStacks;
	buildStack(stacks, numOfStacks, rowsInStack, columnsInStack);

	bool loadBuiltInMap = false;

	#ifdef __linux__ 
    //linux code goes here
	__fpurge(stdin);
	#elif _WIN32
	// windows code goes here
	fflush(stdin);
	#else
	#endif

	printf("Do you want to load built-in map? (Y/y/1 or N/n/0) ");
	char load;
	char scenario;	
	int input = scanf("%c", &load);
	if(input == EOF || load == '.')
		return;

	if(load == 'Y' || load == 'y' || load == '1'){
		printf("Which scenario you want? 0-9:  ");
		#ifdef __linux__ 
		//linux code goes here
		__fpurge(stdin);
		#elif _WIN32
		// windows code goes here
		fflush(stdin);
		#else
		#endif
		input = scanf("%c", &scenario);
		switch (scenario)
		{
			case '0':
			case '1':
			case '2':
				twoSeperatePolygons(stacks[0].slotsOfAV, scenario);
				break;
			case '.':
				return;
			default:
				break;
		}

		
	}
	else{
		/*stacks[0].slotsOfAV[0][0] = 0;*/	stacks[0].slotsOfAV[0][1] = 0;  stacks[0].slotsOfAV[0][3] = 0;  stacks[0].slotsOfAV[0][4] = 0;  
																				stacks[0].slotsOfAV[0][6] = 0;  stacks[0].slotsOfAV[0][7] = 0;

		/*stacks[0].slotsOfAV[1][3] = 0;*/	stacks[0].slotsOfAV[1][6] = 0;  stacks[0].slotsOfAV[1][7] = 0;	
		stacks[0].slotsOfAV[2][7] = 0;
		stacks[0].slotsOfAV[3][7] = 0;
		stacks[0].slotsOfAV[4][0] = 0;	stacks[0].slotsOfAV[4][2] = 0;  stacks[0].slotsOfAV[4][6] = 0;  stacks[0].slotsOfAV[4][7] = 0;
		stacks[0].slotsOfAV[5][0] = 0;	stacks[0].slotsOfAV[5][2] = 0;  stacks[0].slotsOfAV[5][3] = 0;  stacks[0].slotsOfAV[5][6] = 0;  
																					stacks[0].slotsOfAV[5][7] = 0;
		

						//stacks[0].slotsOfAV[5][1] = 0;  stacks[0].slotsOfAV[5][4] = 0;  stacks[0].slotsOfAV[5][5] = 0;

		//stacks[0].slotsOfAV[1][0] = 0;  stacks[0].slotsOfAV[2][0] = 0;  stacks[0].slotsOfAV[3][0] = 0;


		for(int i = 0; i < 6; i++){
			for(int j = 7; j < columnsInStack; j++){
				stacks[0].slotsOfAV[i][j] = 0;
			}
		}
		stacks[0].slotsOfAV[0][9] = 1;
	}

	//readKStacks(polygons, stacks, numOfStacks, rowsInStack, columnsInStack);
	BuildingPolygons* generator = new RefinePolygons(rowsInStack, columnsInStack, 1, 33, widthOfAV, lengthOfAV);
	generator->NUMBER_STACKS = numOfStacks;
	generator->getRawPolygons(0, stacks[0].slotsOfAV);

	//printf("Which wall-map do you want? N/n/0 or Y/y/1:   ");
	//fflush(stdin);
	//input = scanf("%c", &scenario);
	int*** walls;
	//switch (scenario)
	{
		//case 'Y':
		//case 'y':
		//case '1':
			walls = loadWall(rowsInStack, columnsInStack, 5, widthOfAV, lengthOfAV);
			generator->loadWall(walls, 8);
			//break;
	
		//default:
		//	break;
	}

	setMinMax(generator->polygons);
	
	printStacks(stacks, numOfStacks, rowsInStack, columnsInStack);

	int ones = 0;
	for(int k = 0; k < numOfStacks; k++){
		for(int i = 0; i < rowsInStack; i++){
			for(int j = 0; j < columnsInStack; j++){
				if(stacks[k].slotsOfAV[i][j] == 1)
					ones++;
			}
		}
	}

	getPathPlanning(stacks, numOfStacks, rowsInStack, columnsInStack, generator);
	//assert(ones == 0);

	//assert(generator->polygons.size() == 2);
	//cout<<"\nstart: "<<start.x<<", "<<start.y<<endl;
	
	//point end(8, 0);
	//cout<<"End: "<<end.x<<", "<<end.y<<endl;
	
}

void getPathPlanning(K_Stack *stacks, int numOfStacks, int rowsInStack, int columnsInStack, BuildingPolygons* generator){

	int i = 0, j = 0, indexOfStack = 0;
	int gateNumber = 0;
	bool stopInput = false;
	int input;
	int directionAtTheEnd = 0;
	PlanningController* plan = new PlanningController();
	while(!stopInput && countAllStacks(stacks, numOfStacks, rowsInStack, columnsInStack) > 0){
		stopInput = true;
		printf("Type the index of stack: ");
		#ifdef __linux__ 
		//linux code goes here
		__fpurge(stdin);
		#elif _WIN32
		// windows code goes here
		fflush(stdin);
		#else
		#endif
		input = scanf("%d", &indexOfStack);
		if(input != EOF && input != 0 && indexOfStack >= 0 && indexOfStack < numOfStacks){
			#ifdef __linux__ 
			//linux code goes here
			__fpurge(stdin);
			#elif _WIN32
			// windows code goes here
			fflush(stdin);
			#else
			#endif
			printf("Type the row index of AV: ");
			input = scanf("%d", &i);
			if(input != EOF && input != 0 && i >= 0 && i < rowsInStack){
				fflush(stdin);
				printf("Type the column index of AV: ");
				#ifdef __linux__ 
				//linux code goes here
				__fpurge(stdin);
				#elif _WIN32
				// windows code goes here
				fflush(stdin);
				#else
				#endif
				input = scanf("%d", &j);
				if(input != EOF && input != 0 && j >= 0 && j < columnsInStack){
					if(stacks[indexOfStack].slotsOfAV[i][j] == 1 ||
						stacks[indexOfStack].slotsOfAV[i][j] == -1
						){
						point slot = generator->getSlot(indexOfStack, i, j);
						//fflush(stdin);
						//printf("Type the gate number (zero - 0 - for pick up an arbitrary point): ");
						#ifdef __linux__ 
						//linux code goes here
						__fpurge(stdin);
						#elif _WIN32
						// windows code goes here
						fflush(stdin);
						#else
						#endif
						//input = scanf("%d", &gateNumber);
						if(//input != EOF && 
								gateNumber >= 0 && gateNumber <= 4)
						{
							if(gateNumber == 0){
								fflush(stdin);
								printf("The direction from rear to center (4/8/6/2)?: ");
								#ifdef __linux__ 
								//linux code goes here
								__fpurge(stdin);
								#elif _WIN32
								// windows code goes here
								fflush(stdin);
								#else
								#endif
								input = scanf("%d", &directionAtTheEnd);
								if(input == EOF || input == 0 ||
										(directionAtTheEnd != 4 && directionAtTheEnd != 8 && directionAtTheEnd != 6 && directionAtTheEnd != 2))
								{
									cout<<"Wrong direction (4-Left/8-Up/6-Right/2-Down)"<<endl;
									break;
								}
								else{
									generator->setDirectionAtTheEnd(directionAtTheEnd);
								}
							}
							fflush(stdin);
							printf("Do you want to get in (I/i/1/l) or get out (O/o/0)?");
							char Mode;
							#ifdef __linux__ 
							//linux code goes here
							__fpurge(stdin);
							#elif _WIN32
							// windows code goes here
							fflush(stdin);
							#else
							#endif
							input = scanf("%c", &Mode);
							if(input != EOF && Mode != '.' && (Mode == 'I' || Mode == 'O' || Mode == '1' || Mode == 'l' 
									|| Mode == 'i' || Mode == 'o' || Mode == '0')){
								
								stopInput = false;
								if(Mode == 'I' || Mode == 'i' || Mode == '1' || Mode == 'l'){
									point gate = generator->getPositionInGate(gateNumber, numOfStacks, true);
									plan->getTrajectory(generator, gate, slot);
								}
								else{
									point gate = generator->getPositionInGate(gateNumber, numOfStacks, false);
									generator->removeEdgesAndVertices(indexOfStack, i, j);
									plan->getTrajectory(generator, slot, gate);
								}
							}
							else{
								cout<<"Wrong mode! (Only I or O)"<<endl;
								break;
							}
						}
						else{
							cout<<"Wrong number of gate (only 1-4)!"<<endl;
							break;
						}
						
					}
					else{
						cout<<"No AV over there"<<endl;
					}
				}
				else{
					cout<<"Wrong value of column!"<<endl;
					break;
				}
			}
			else{
				cout<<"Wrong value of row!"<<endl;
				break;
			}
		}
		else{
			cout<<"Wrong index of stack!"<<endl;
			break;
		}
	}

	cout<<"Finish choosing path of AVs"<<endl;
}

void testInsidePolygon(){
	//point p(5.936, 12.02);
	point p(-10.22115414, 12.9808);
	//point p0(0, 36);
	point p0(-11, -11);
	//point p1(16, 36);
	point p1(-11, 13);
	//point p2(16, 18);
	point p2(-10, 13);
	//point p3(12, 18);
	point p3(-10, -10);
	//point p4(12, 12);
	point p4(16, -10);
	//point p5(6, 12);
	point p5(16, -11);
	//point p6(6, 18);
	//point p6()
	//point p7(0, 18);
	point arr[] = {p0, p1, p2, p3, p4, p5
							//, p6
							//, p7
						};
	int N = 6; //8;
	vector<lineSegment> polygon;
	for(int i = 0; i < N; i++){
		lineSegment line;
		line.p = arr[i];
		line.q = arr[(i + 1) % N];
		polygon.push_back(line);
	}

	int check = wn_PnPoly(p, polygon);
	cout<<"Check = "<<check<<endl;
}

void testTouching(){
	point p1(40, 0); 
	point q1(-48, 0);
	point p2(38, 0);
	point q2(38, 12);
	lineSegment line1(p1, q1);
	lineSegment line2(p2, q2);

	int c = doIntersect(line1, line2);
	cout<<"c = "<<c<<endl;
}


int main(int argc, const char* argv[]){
	//testInsidePolygon();
	//testTouching();
	testFullAVsInStacks();
	return 0;
}