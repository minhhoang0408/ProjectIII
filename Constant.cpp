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
#include <fstream>


#ifndef _CONSTANT_
#define _CONSTANT_

using namespace std;

const char LEFT = 'L';

const char RIGHT = 'R';

const char CIRCLE = 'C';

const double MIN_VELOCITY = 2; //(m/s)

const double MAX_VELOCITY = 2.5; //(m/s)

const double SMALL_ANGLE = 0.01; //small angle which allows sin(alpha) = alpha

const double RATIO = 6.8; //it is also the maximum of minumun rotated radius for all cars in the market recent years:https://laithutoyota.vn/ban-kinh-quay-vong-xe/

const double EPSILON = 0.001;

enum SIDE{LeftSide = 0, RightSide = 1, NotAnswer = 3};
//enum NORMAL_VECTOR {IN, OUT};
enum TypeOfLineSegment{ AV_EDGE = 0, HENCE = 1, GATE = 2, NONE = 3};

enum Clockwise{ YES = 1, NO = 0, Unknown = 2, TurnToClockwise = 3, TurnToCounterClockwise = 4};

#endif