#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>
#include "Constant.cpp"

#include <cassert>  
#include <stdio.h>

//So we don't need to write std:: everywhere
using namespace std;

void twoSeperatePolygons(int** slots, char scenario){
    int NUM_STACKS = 1;
    int ROWS = 6;
    int COLS = 21;

    printf("Two independent polygons in both sides of stack[0]:\n");

    int arr[6][21] 
    = {
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0,
    };

    switch (scenario)
    {
        case '1':
            arr[0][16] = 0;
            arr[0][17] = 1;
            break;
        case '2':
            arr[0][14] = 0;
            arr[0][15] = 1;
            arr[0][16] = 0;
            arr[0][17] = 1;
            break;
        
        default:
            break;
    }

    printf("\n");
    for(int i = 0; i < ROWS; i++){
        printf("\t");
        for(int j = 0; j < COLS; j++){
            printf("%2d ", arr[i][j]);
        }
        printf("\n");
    }

    bool stopInput = false;
	int input;
    int indexOfStack = 0;
	while(!stopInput){
        stopInput = true;
        printf("Which stack you want to edit?: ");
        input = scanf("%d", &indexOfStack);
        if(input != EOF && input != 0 && indexOfStack >= 0 && indexOfStack < NUM_STACKS){
            printf("Which slot you want to change (from 0 to 1 or 1 to 0)? Type (row, colum): ");
            int rowIndex, colIndex;
            #ifdef __linux__ 
            //linux code goes here
            __fpurge(stdin);
            #elif _WIN32
            // windows code goes here
            fflush(stdin);
            #else
            #endif
            input = scanf("%d, %d", &rowIndex, &colIndex);
            if(input != EOF && input != 0 && rowIndex >= 0 && rowIndex < ROWS && colIndex >= 0 && colIndex < COLS){
                arr[rowIndex][colIndex] = arr[rowIndex][colIndex] == 0 ? 1 : 0;
                stopInput = false;
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            slots[i][j] = arr[i][j];
        }
    }
}

int*** loadWall(int ROWS, int COLS, int WIDTH_OF_GATE, int WIDTH_SLOT, int LENGTH_SLOT, int numOfStacks = 1, int WIDTH_LINE = 10){
    int numOfWalls = 8;
    int numOfVertices = 6;
    int*** walls = (int ***)malloc(sizeof * walls * numOfWalls);
    for(int i = 0; i < numOfWalls; i++){
        walls[i] = (int **)malloc(sizeof * walls[i] * numOfVertices);
        for(int j = 0; j < numOfVertices; j++){
            walls[i][j] = (int *)malloc(sizeof * walls[i][j] * 3);
            walls[i][j][0] = 0;
            walls[i][j][1] = 0;
            walls[i][j][2] = HENCE;
        }
    }

    #pragma region First hence 
    walls[0][0][0] = -WIDTH_LINE;   walls[0][0][1] = -WIDTH_LINE;
    walls[0][1][0] = walls[0][0][0] + (COLS*WIDTH_SLOT - WIDTH_OF_GATE*WIDTH_SLOT + 2*WIDTH_LINE)/2;    walls[0][1][1] = -WIDTH_LINE;
    walls[0][2][0] = walls[0][1][0];    walls[0][2][1] = walls[0][1][1] - 1;
    walls[0][3][0] = walls[0][0][0] - 1;   walls[0][3][1] = walls[0][0][1] - 1;
    walls[0][4][0] = walls[0][3][0];   walls[0][4][1] = walls[0][0][1] + (numOfStacks*ROWS*LENGTH_SLOT - WIDTH_OF_GATE*WIDTH_SLOT + (numOfStacks + 1)*WIDTH_LINE)/2;
    walls[0][5][0] = walls[0][4][0] + 1;   walls[0][5][1] = walls[0][4][1];
    /*Clockwise
 (0, 4) __   (0, 5)
       | |
       | |
       | |(0, 0)
       | |___________ (0, 1)
       |_____________| 
    (0, 3)            (0, 2)
    */
    #pragma endregion
    
    #pragma region Second hence
    walls[1][0][0] = -WIDTH_LINE - 1;    walls[1][0][1] = numOfStacks*ROWS*LENGTH_SLOT + (numOfStacks)*WIDTH_LINE + 1;
    walls[1][1][0] = -WIDTH_LINE + (COLS*WIDTH_SLOT - WIDTH_OF_GATE*WIDTH_SLOT + 2*WIDTH_LINE)/2;    walls[1][1][1] = walls[1][0][1];
    walls[1][2][0] = walls[1][1][0];    walls[1][2][1] = walls[1][1][1] - 1;
    walls[1][3][0] = -WIDTH_LINE;   walls[1][3][1] = walls[1][2][1];
    walls[1][4][0] = -WIDTH_LINE;   walls[1][4][1] = walls[0][5][1] + WIDTH_OF_GATE*WIDTH_SLOT;
    walls[1][5][0] = walls[1][4][0] - 1;   walls[1][5][1] = walls[1][4][1] ;
    /*
        Clockwise
       (1, 0)
        _____________  (1, 1)
        |  __________|
        | |(1, 3)     (1, 2)
        | |
        | |
        |_|
  (1, 5)   (1, 4)
    */
    #pragma endregion

    #pragma region Third hence
    walls[2][0][0] = (COLS*WIDTH_SLOT + WIDTH_OF_GATE*WIDTH_SLOT)/2;    walls[2][0][1] = walls[1][0][1];
    walls[2][1][0] = WIDTH_LINE + (COLS*WIDTH_SLOT) + 1;    walls[2][1][1] = walls[2][0][1];
    walls[2][2][0] = walls[2][1][0];    walls[2][2][1] = walls[1][4][1];
    walls[2][3][0] = walls[2][2][0] - 1;   walls[2][3][1] = walls[2][2][1];
    walls[2][4][0] = walls[2][3][0];   walls[2][4][1] = walls[1][3][1];
    walls[2][5][0] = walls[2][0][0];   walls[2][5][1] = walls[2][4][1] ;
    
    /*
        Clockwise
                  
  (2, 0) _____________  (2, 1)
        |__________  |
  (2, 5)     (2, 4)| |
                   | |
                   | |
                   |_|
             (2, 3)   (2, 2)
    */
    #pragma endregion


    #pragma region Forth hence
    walls[3][0][0] = walls[2][3][0];    walls[3][0][1] = walls[2][3][1] - WIDTH_OF_GATE*WIDTH_SLOT;
    walls[3][1][0] = walls[3][0][0] + 1;    walls[3][1][1] = walls[3][0][1];
    walls[3][2][0] = walls[3][1][0];    walls[3][2][1] = -WIDTH_LINE - 1;
    walls[3][3][0] = walls[2][5][0];   walls[3][3][1] = walls[3][2][1];
    walls[3][4][0] = walls[3][3][0];   walls[3][4][1] = walls[3][3][1] + 1;
    walls[3][5][0] = walls[3][0][0];   walls[3][5][1] = walls[3][4][1] ;
    
    /*
        Clockwise

            (3, 0)  _  (3, 1)
                   | |
                   | |
             (3, 5)| |
   (3, 4)__________| |
        |____________|
  (3, 3)              (3, 2)  
  
    */
    #pragma endregion

    #pragma region virtual gate
    walls[4][0][0] = walls[0][1][0] - 1;    walls[4][0][1] = walls[0][1][1];  walls[4][0][2] = GATE;
    walls[4][1][0] = walls[4][0][0] + WIDTH_OF_GATE*WIDTH_SLOT + 2;    walls[4][1][1] = walls[4][0][1]; walls[4][1][2] = GATE;
    walls[4][2][0] = walls[4][1][0];    walls[4][2][1] = walls[4][1][1] - 1;        walls[4][2][2] = GATE;
    walls[4][3][0] = walls[4][0][0];    walls[4][3][1] = walls[4][2][1];            walls[4][3][2] = GATE;
    walls[4][4][2] = NONE;      
    walls[4][5][2] = NONE;
    /*Clockwise
  (4, 0) _______________________ (4, 1)
        |_______________________|
    (4, 3)                      (4, 2)
    */
    #pragma endregion

    #pragma region virtual gate
    walls[5][0][0] = walls[1][1][0] - 1;    walls[5][0][1] = walls[1][1][1];  walls[5][0][2] = GATE;
    walls[5][1][0] = walls[5][0][0] + WIDTH_OF_GATE*WIDTH_SLOT + 2;    walls[5][1][1] = walls[5][0][1]; walls[5][1][2] = GATE;
    walls[5][2][0] = walls[5][1][0];    walls[5][2][1] = walls[5][1][1] - 1;        walls[5][2][2] = GATE;
    walls[5][3][0] = walls[5][0][0];    walls[5][3][1] = walls[5][2][1];            walls[5][3][2] = GATE;
    walls[5][4][2] = NONE;      
    walls[5][5][2] = NONE;
    /*Clockwise
  (5, 0) _______________________ (5, 1)
        |_______________________|
    (5, 3)                      (5, 2)
    */
    #pragma endregion

    #pragma region virtual gate
    walls[6][0][0] = walls[1][5][0];    walls[6][0][1] = walls[1][5][1] + 1;        walls[6][0][2] = GATE;
    walls[6][1][0] = walls[6][0][0] + 1;    walls[6][1][1] = walls[6][0][1];    walls[6][1][2] = GATE;
    walls[6][2][0] = walls[6][1][0] ;    walls[6][2][1] = walls[6][1][1] - WIDTH_OF_GATE*WIDTH_SLOT - 2;    walls[6][2][2] = GATE;
    walls[6][3][0] = walls[6][2][0] - 1;   walls[6][3][1] = walls[6][2][1];     walls[6][3][2] = GATE;
    walls[6][4][2] = NONE;  
    walls[6][5][2] = NONE;  
    
    /*
        Clockwise

             (6, 0) _  (6, 1)
                   | |
                   | |
                   | |
                   | |
                   |_|
             (6, 3)   (6, 2)  
  
    */
    #pragma endregion

    #pragma region virtual gate
    walls[7][0][0] = walls[2][3][0];    walls[7][0][1] = walls[2][3][1] + 1;        walls[7][0][2] = GATE;
    walls[7][1][0] = walls[7][0][0] + 1;    walls[7][1][1] = walls[7][0][1];    walls[7][1][2] = GATE;
    walls[7][2][0] = walls[7][1][0] ;    walls[7][2][1] = walls[7][1][1] - WIDTH_OF_GATE*WIDTH_SLOT - 2;    walls[7][2][2] = GATE;
    walls[7][3][0] = walls[7][2][0] - 1;   walls[7][3][1] = walls[7][2][1];     walls[7][3][2] = GATE;
    walls[7][4][2] = NONE;  
    walls[7][5][2] = NONE;  
    
    /*
        Clockwise

             (7, 0) _  (7, 1)
                   | |
                   | |
                   | |
                   | |
                   |_|
             (7, 3)   (7, 2)  
  
    */
    #pragma endregion

    return walls;
}

