#include <iostream> //cin cout
#include <vector> //vector
#include <cstdio> //scanf
#include <sstream> //ostringstream
#include <cmath> //pow sqrt
#include <queue> //priority_queue
#include <stdlib.h> //atoi
#include <tuple> //get<n> make_tuple
#include <chrono>
#include <queue>
#include <bits/stdc++.h>
#include <algorithm>
#include <bits/stdc++.h>

//#include "draw.cpp"

using namespace std;

bool checkMovement(int** arrayOfAVs, int ROWs, int COLUMNs, int row, int col){
    if(row >= ROWs || col >= COLUMNs){
        return false;
    }
    return arrayOfAVs[row][col] != 0;

}

