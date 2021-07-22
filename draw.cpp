//#define _USE_MATH_DEFINES

#include <iostream>
#include <sstream> //ostringstream
#include <fstream> 
#include <math.h>
#include <vector> //vector
#include <float.h>
#include <string>
#include <stdlib.h>
#include "Constant.cpp"

#ifndef _DRAW_
#define _DRAW_

using namespace std;

struct {
	bool printGraph = false;
	bool drawRoute = true;
	int printLevel = 0;
	size_t k = 0;
} config;

typedef struct Stacks{
	int** slotsOfAV;
	int k;
}K_Stack;

//point type for holding a coordinate 
void setMinMax(double x, double y);
struct point {
	double x;
	double y;
	point(double x,double y){
		this->x = x;
		this->y = y;
	}

	point(){

	}

	bool operator==(const point &other) const {
		return (this->x==other.x && this->y==other.y);
	}
};

struct Trajectory{
	point position;
	double t;

	Trajectory(double x, double y, double t){
		this->position.x = x;
		this->position.y = x;
		this->t = t;
	}

};

class RawRoute{
    public:
        vector<point> points;
        
        RawRoute(){

        }

        void insert(point p){
            points.insert(points.begin(), p);
        }
};

struct Slot {
	int row;
	int column;
	Slot(int x, int y){
		this->row = x;
		this->column = y;
	}

	Slot(){

	}

	bool operator==(const Slot &other) const {
		return (this->row == other.row && this->column == other.column);
	}
};

//Type used in the priority queue in the dijkstra function
typedef std::pair<double,std::pair<int,int> > pq_pair;



//linesegment type holding two points
struct lineSegment {
	point p;
	point q;
	enum TypeOfLineSegment type;
	lineSegment(point p, point q){
		this->p = p;
		this->q = q;
		this->type = AV_EDGE;
	};

	lineSegment(point p, point q, TypeOfLineSegment type){
		this->p = p;
		this->q = q;
		this->type = type;
		if(type == NONE)
			cout<<"NONE"<<endl;
	};

	lineSegment(){

	}
	bool operator==(const lineSegment &other) const {
		return (this->p==other.p && this->q==other.q) ||
		(this->p==other.q && this->q==other.p);
  	}

	bool contains(point c){
		return (c == p) || (c == q); 
	}
};

//Each instance of movement point is the point which AV should reach before a time
class MovementPoint{
    public:
        double beganX;
        double beganY;
        double endedX;
        double endedY;
		int idOfAV;//A point belongs to one AV
		enum SIDE side;//The AV which in the first movement turns right or left?
};

//A section is a small trajectory which routes AV to move (both forward and backward) from
//a point to other one. 
class Section : public MovementPoint{
    public:
        char typeOfTrajectory ;
        char steering;
        double param;
        vector<point> possiblePoints;
		double centerX = FLT_MAX;
		double centerY = FLT_MAX;
};

//A path segment is a group of continuous section to move from one point to other one.
class PathSegment : public MovementPoint{
    public:
        vector<Section*> sections;
        float L;
		float penalty;
		//lineSegment firstVelocity;
		//lineSegment lastVelocity;
        //int index;

        PathSegment(){
            //this->index = -1;
            this->L = 0;
			this->penalty = 0;
        }

		void setSide(enum SIDE side){
			for(int i = 0; i < sections.size(); i++){
				sections.at(i)->side = side;
			}
		}


};

//A path is a group of possible path segment and its best choice to move from
//one point to other one
class Path : public MovementPoint{
    public:
        vector<PathSegment*> segments;
        float Lmin;
		float penaltyMin;
        //int index;

        Path(){
            this->Lmin = FLT_MAX;
			this->penaltyMin = FLT_MAX;
            //this->index = -1;
        }
};

class Range{
    public:
        double xMax;
        double xMin;
        double yMax;
        double yMin;

        Range(){
            this->xMax = -FLT_MAX;
            this->yMax = -FLT_MAX;
            this->xMin = FLT_MAX;
            this->yMin = FLT_MAX;
        }
};

double max_y,max_x,min_y,min_x;

bool isVirtualGate(vector<lineSegment> polygon);

void setMinMax(vector<vector<lineSegment>> &polygons
				//, double *max_y, double *max_x, double *min_y, double *min_x
							){
	max_x = -FLT_MAX;							
	max_y = -FLT_MAX;
	min_x = FLT_MAX;
	min_y = FLT_MAX;
	
	for(int i = 0; i < polygons.size(); i++){
		for(int j = 0; j < polygons.at(i).size(); j++){
			if(max_y < polygons.at(i).at(j).p.y){
				max_y = polygons.at(i).at(j).p.y;
			}
			if(min_y > polygons.at(i).at(j).p.y){
				min_y = polygons.at(i).at(j).p.y;
			}

			if(max_x < polygons.at(i).at(j).p.x){
				max_x = polygons.at(i).at(j).p.x;
			}
			if(min_x > polygons.at(i).at(j).p.x){
				min_x = polygons.at(i).at(j).p.x;
			}
		}
	}
	
}

string drawX(point &p){
	double r = p.x*10;
	return to_string(r);
}

string drawY(point &p){
	double r = p.y*-10;
	return to_string(r);
}

double getMABCOfLine(lineSegment line, double *A, double *B, double *C){
	double x1 = line.p.x;
	double x2 = line.q.x;
	double y1 = line.p.y;
	double y2 = line.q.y;
	*A = y2 - y1;
	*B = x1 - x2;
	*C = y1*x2 - x1*y2;
	double M = sqrt((*A)*(*A) + (*B)*(*B));
	return M;
}

//we define the "normal in" vector v of vector u is a vector such that
//(u^v) has direction to go in the plan of (u, v)
//deltaX, deltaY is coords of vector u
void getNormalInAndOut(double deltaX, double deltaY, double *xIn, double *yIn, double *xOut = NULL, double *yOut = NULL)
{
	if(deltaX == 0){
		if(xIn && yIn){
			*xIn = (deltaY > 0 ? 1 : -1); *yIn = 0;
		}
		
		if(xOut && yOut){
			*xOut = (deltaY > 0 ? -1 : 1); //fix bug in case *xIn = NULL
			*yOut = 0;
		}
	}
	else if(deltaY == 0){
		if(xIn && yIn){
			*xIn = 0; *yIn = (deltaX > 0 ? -1 : 1);
		}

		if(xOut && yOut){
			*xOut = 0; *yOut = (deltaX > 0 ? 1 : -1);//fix bug in case *yIn = NULL
		}
	}
	else{
		
		double length = sqrt(deltaX*deltaX + deltaY*deltaY);
		
		if(xIn && yIn){
			*xIn = deltaY/length; *yIn = -deltaX/length;
		}

		if(xOut && yOut){
			*xOut = -deltaY/length; *yOut = deltaX/length;
		}
	}
}

string drawRoute(vector<int> & route, vector<point> & points){
	string str = "<polyline stroke='red' stroke-width='2' fill='none' points='";
	int current = route.size()-1;
	while(current != -1){
		point p = points[current%points.size()];
		str.append(drawX(p));
		str.append(",");
		str.append(drawY(p));
		str.append(" ");
		//cout<<"\nroute["<<current<<"] = "<<route[current]<<" => ("<<p.x<<", "<<p.y<<")";
		current = route[current];
	}
	str.append("'/>\n");

	
	return str;
}

string drawShortestRoute(vector<point> & route//, vector<point> & sideSteps
							){
	string str = "<polyline stroke='black' stroke-width='2' fill='none' points='";
	int current = route.size()-1;
	while(current != -1){
		point p = //points[current%points.size()];
					route.at(current);
		str.append(drawX(p));
		str.append(",");
		str.append(drawY(p));
		str.append(" ");
		//cout<<"\nroute["<<current<<"] = ("<<p.x<<", "<<p.y<<")";
		//current = route[current];
		current--;
	}
	str.append("'/>\n");

	
	return str;
}



string drawPoint(point &p,string color){
	string str = "<circle cx='";
	str.append(drawX(p)); str.append("' cy='");
	str.append(drawY(p)); str.append("' r='0.8' fill='");
	str.append(color);
	str.append("' />\n");
	return str;
}

string drawPolygon(vector<lineSegment> & polygon){
	if(isVirtualGate(polygon)){
		string nothing = "";
		return nothing;
	}
	string str = "<polygon stroke='black' stroke-width='0.1' fill='#D3D3D3'  points='";
	str.append(drawX(polygon[0].p));
	str.append(","); str.append(drawY(polygon[0].p));
	str.append(" ");
	str.append(drawX(polygon[0].q));
	str.append(",");
	str.append(drawY(polygon[0].q));
	str.append(" ");
	for(int i = 1; i < polygon.size(); i++){
		str.append(drawX(polygon[i].q));
		str.append(",");
		str.append(drawY(polygon[i].q));
		str.append(" ");
	}
	str.append("'/>\n");
	return str;
}

string drawPolygons(vector <vector <lineSegment> > & polygons){
	string str = "";
	for(int i = 0; i < polygons.size(); i++){
		str.append(drawPolygon(polygons[i]));
	}
	return str;
}
string drawTitle(string testTitle, double distance){
	string str = "";
	str = str.append("<text x='0' y='");
	str = str + to_string(-10*max_y-5); 
	str = str + "' font-family='Verdana' font-size='5'>";
	str = str + testTitle;
	str = str + ", length: ";
	str = str + to_string(distance);
	str = str + "</text>\n";
	return str;
}

string drawGraph(vector< vector< int> > &graph, vector<point>& points){
	string str = "";
	size_t plane_start = ((points.size()*config.printLevel)/points.size())*points.size();
	size_t plane_end   = plane_start+points.size();
	for(size_t i = plane_start; i < plane_end; i++){
		for(size_t j = 0; j < graph[i].size(); j++){
			point from = points[i%points.size()], to=points[graph[i][j]%points.size()];
			str = str + "<line x1='";
			str = str + drawX(from);
			str = str + "' y1='";
			str = str + drawY(from);
			str = str + "' x2='";
			str = str + drawX(to);
			str = str + "' y2='";
			str = str + drawY(to);
			str = str + "' stroke-width='0.1' stroke='blue'/>\n";
		}
	}
	return str;
}

bool isVirtualGate(vector<lineSegment> polygon){
    for(int i = 0; i < polygon.size(); i++){
        if(polygon.at(i).type == GATE)
            return true;
    }
    return false;
}

int cutThrough(lineSegment l1, lineSegment l2){
	point AC; 
	AC.x = l2.p.x - l1.p.x; 
	AC.y = l2.p.y - l1.p.y;
	point AB;
	AB.x = l1.q.x - l1.p.x; 
	AB.y = l1.q.y - l1.p.y;
	point AD;
	AD.x = l2.q.x - l1.p.x; 
	AD.y = l2.q.y - l1.p.y;
	double AC_AB = AC.x*AB.y - AC.y*AB.x;
	double AD_AB = AD.x*AB.y - AD.y*AB.x;
	if(AC_AB*AD_AB < 0){
		//point A = l2.p; point B = l2.q;
		//point C = l1.p; point D = l1.q;
		AC.x = l1.p.x - l2.p.x; 
		AC.y = l1.p.y - l2.p.y;
		
		AB.x = l2.q.x - l2.p.x; 
		AB.y = l2.q.y - l2.p.y;
		
		AD.x = l1.q.x - l2.p.x; 
		AD.y = l1.q.y - l2.p.y;
		AC_AB = AC.x*AB.y - AC.y*AB.x;
		AD_AB = AD.x*AB.y - AD.y*AB.x;
		if(AC_AB*AD_AB < 0){
			return 1; //cut through
		}
	}
	return 0;

}

bool onSegment(point p, point q, point r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
       return true;
  
    return false;
}
  
// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(point p, point q, point r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);
  
    if (val == 0) return 0;  // colinear
  
    return (val > 0)? 1: 2; // clock or counterclock wise
}
  
// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
int doIntersect(lineSegment line1, lineSegment line2)
{
	point p1 = line1.p;
	point q1 = line1.q;
	point p2 = line2.p;
	point q2 = line2.q;
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
  
    // General case
    if (o1 != o2 && o3 != o4)
        return 1;
  
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return 1;
  
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return 1;
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return 1;
  
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return 1;
  
    return 0; // Doesn't fall in any of the above cases
}

bool isEqual(double pX, double pY, double qX, double qY){
	if(std::abs(pX - qX) <= EPSILON && std::abs(pY - qY) <= EPSILON){
		return true;
	}
	return false;
}


// isLeft(): tests if a point is Left|On|Right of an infinite line.
//    Input:  three points P0, P1, and P2
//    Return: >0 for P2 left of the line through P0 and P1
//            =0 for P2  on the line
//            <0 for P2  right of the line
inline double isLeft( point P0, point P1, point P2, double scale = 1)
{
    return ( (P1.x*scale - P0.x*scale) * (P2.y*scale - P0.y*scale)
            - (P2.x*scale -  P0.x*scale) * (P1.y*scale - P0.y*scale) );
}
//===================================================================

//===================================================================

// wn_PnPoly(): winding number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  wn = the winding number (=0 only when P is outside)
int wn_PnPoly(point P, vector<lineSegment> polygon, double scale = 1)
{
    int    wn = 0;    // the  winding number counter
	int n = polygon.size();
	double P_y_Scale = P.y*scale;
    // loop through all edges of the polygon
    for (int i = 0; i < n; i++) {   // edge from V[i] to  V[i+1]
        if (polygon[i].p.y*scale <= P_y_Scale) {          // start y <= P.y
            if (polygon[i].q.y*scale  > P_y_Scale)      // an upward crossing
                 if (isLeft( polygon[i].p, polygon[i].q, P, scale) > 0)  // P left of  edge
                     ++wn;            // have  a valid up intersect
        }
        else {                        // start y > P.y (no test needed)
            if (polygon[i].q.y*scale  <= P_y_Scale)     // a downward crossing
                 if (isLeft( polygon[i].p, polygon[i].q, P, scale) < 0)  // P right of  edge
                     --wn;            // have  a valid down intersect
        }
    }
    return wn;
}
//===================================================================

//If the return value is an odd, the point definitely is inside polygon
/*int pnpoly(vector<lineSegment> polygon, double testx, double testy, bool OyDirection)
{
	int i, j, c = 0;
	int nEdges = polygon.size();
	

	point pA(testx, testy);
	int result = 0;

	for(size_t j = 0; j < polygon.size();j++){
		
		//result += touch(line,polygon[j]);
	}

	return result;
}*/


vector<point> middlePoints(point pA, point pB){
	vector<point> allMiddlePoints;
	bool hasManyMiddlePoints = true;
	int size = (int)round(std::abs(pA.x - pB.x));
	if(size <= 2){
		size = (int)round(std::abs(pA.y - pB.y));
		if(size <= 2)
		{
			point p((pA.x + pB.x)/2, (pA.y + pB.y)/2);
			allMiddlePoints.push_back(p);
			hasManyMiddlePoints = false;
		}
	}
	
	if(hasManyMiddlePoints){
		
		double unitX = (pB.x - pA.x)/size;
		double unitY = (pB.y - pA.y)/size;
		size--;
		double x = pA.x;
		double y = pA.y;
		//double x = min(pA.x + 1, pB.x + 1); 
		//double y = ((x - pA.x)*(pB.y - pA.y)/(pB.x - pA.x)) + pA.y;
		for(int i = 0; i < size; i++){
			x += unitX;
			y += unitY;
			point p(x, y);
			allMiddlePoints.push_back(p);
		}
	}
	return allMiddlePoints;
}


bool isEdge(lineSegment l, vector<lineSegment> polygon){
	bool found_P = false, found_Q = false, found_both = false;
	for(int i = 0; i < polygon.size(); i++){
		//found_P = false; found_Q = false; found_both = false;
		//for(int j = 0; j < polygons.at(i).size(); j++)
		{
			if((l.p == polygon.at(i).p && l.q == polygon.at(i).q)
				|| (l.p == polygon.at(i).q && l.q == polygon.at(i).p)
				)
			{
				return true;
			}	
		}
	}

	return false;
	
}

bool cutThroughPolygons(lineSegment line, vector<vector<lineSegment> > &polygons){
	double x;
	double y;
	
	vector<point> allMiddlePoints = middlePoints(line.p, line.q);

	for(int i = 0; i < polygons.size(); i++){

		for(int j = 0; j < allMiddlePoints.size(); j++){
			x = allMiddlePoints.at(j).x; 
			y = allMiddlePoints.at(j).y;
			//if(x != FLT_MAX && y != FLT_MAX)
			if(!isEdge(line, polygons[i]))
			{
				//int c = pnpoly(polygons[i], x, y, OyDirection);
				int c = wn_PnPoly(allMiddlePoints.at(j), polygons[i]);
				if(c != 0)
					return true;
			}
		}
	}
	return false;
}

//Take a line segment and returns number of edges which the line segment touches
int numberOfTouchingPoints(vector<vector<lineSegment> > &polygons, lineSegment l){
	int result = 0;
	for(size_t i = 0; i < polygons.size();i++){
		//int numberOfvaolation=0;
		for(size_t j=0;j<polygons[i].size();j++){
			result += doIntersect(l,polygons[i][j]);
		}
	}
	return result;
}

//Take a line segment and returns number of edges which the line segment cuts through
int numberOfCuttingThrough(vector<vector<lineSegment> > &polygons, lineSegment l){
	int result = 0;
	int c = 0;
	for(size_t i = 0; i < polygons.size();i++){
		//int numberOfvaolation=0;
		for(size_t j=0;j<polygons[i].size();j++){
			c = cutThrough(l,polygons[i][j]);
			result += c;
			//if(c != 0){
			//	cout<<"Crossing at polygons: "<<i<<" "<<j<<endl;
			//}
		}
	}
	return result;
}

//Take a line segment and returns number of edges which the line segment cuts through
int cutThroughRegardlessVirtualGate(lineSegment l, vector<vector<lineSegment> > &polygons){
	int result = 0;
	int c = 0;
	for(size_t i = 0; i < polygons.size();i++){
		//int numberOfvaolation=0;
		if(!isVirtualGate(polygons.at(i))){
			for(size_t j=0;j<polygons[i].size();j++){
				c = cutThrough(l,polygons[i][j]);
				result += c;
			}
		}
	}
	return result;
}

/*double minDistance(vector<vector<lineSegment> > &polygons, lineSegment l){
	double result = FLT_MAX;

}*/

vector<double> getXYOfOtherSide(double X, double Y, int signedValue, double centerX, double centerY, int WIDTH){
	double xOtherSide, yOtherSide;

	//double tag = (Y - centerY)/(X - centerX);
	double sin = (Y - centerY); ///RATIO;
	double cos = (X - centerX); ///RATIO;
	xOtherSide = centerX + cos*(1 + signedValue*WIDTH/RATIO);
	yOtherSide = centerY + sin*(1 + signedValue*WIDTH/RATIO);

	vector<double> result;
	result.push_back(xOtherSide);
	result.push_back(yOtherSide);
	return result;
}

int getSignedValue(Section *section){
	int signedValue = 
	//					((section->steering == 'R' && section->side == RightSide)
	//					|| (section->side == LeftSide && section->steering == 'L')) ? -1 : 1;
	(section->steering == 'R') ? 1 : -1;
	return signedValue;
}

/*
* this function return a string which is combination of two substrings
the first one is drew path trajectory of one AV's side 
the second one is the other path of the remaining AV's side 
*/
string drawCircleArc(Section *section, int WIDTH){
	int signedValue = getSignedValue(section);
	
	string str = "";  string strSub = ""; 	string strCenter = "";
	str.append("<path d=\"M ");			strCenter.append("<path d=\"M ");			strSub.append("<path d=\"M ");

	vector<double> otherPositions = getXYOfOtherSide(section->beganX, 
										section->beganY, signedValue, section->centerX, section->centerY, WIDTH);

	double R1 = (signedValue*WIDTH*0.5 + RATIO)*10;
	double R2 = (signedValue*WIDTH + RATIO)*10;

	double x0 = RATIO*(section->beganX)*10;				double u0 = otherPositions.at(0);
	double y0 = RATIO*(section->beganY*(-10));			double v0 = otherPositions.at(1);
	str.append(to_string(x0));			strCenter.append(to_string(x0*0.5 + u0*RATIO*5));			strSub.append(to_string(u0*RATIO*10));
	str.append(" ");					strCenter.append(" ");										strSub.append(" ");
	str.append(to_string(y0));			strCenter.append(to_string(y0*0.5 - v0*RATIO*5));			strSub.append(to_string(v0*RATIO*(-10)));
	str.append(" ");					strCenter.append(" ");										strSub.append(" ");

	str.append("A ");					strCenter.append("A ");					strSub.append("A ");
	str.append(to_string(RATIO*10));	strCenter.append(to_string(R1));		strSub.append(to_string(R2));
	str.append(" ");					strCenter.append(" ");					strSub.append(" ");
	str.append(to_string(RATIO*10));	strCenter.append(to_string(R1));		strSub.append(to_string(R2));
	str.append(" 0 ");					strCenter.append(" 0 ");				strSub.append(" 0 ");
	int fA = section->param > 3.14 ? 1 : 0;
	int fB = ((section->param < 0 && section->steering == 'R')
					|| (section->param > 0 && section->steering == 'L')
					) ? 0 : 1;
	str.append(to_string(fA));			strCenter.append(to_string(fA));		strSub.append(to_string(fA));
	str.append(" ");					strCenter.append(" ");					strSub.append(" ");
	str.append(to_string(fB));			strCenter.append(to_string(fB));		strSub.append(to_string(fB));
	str.append(" ");					strCenter.append(" ");					strSub.append(" ");

	otherPositions = getXYOfOtherSide(section->endedX, 
										section->endedY, signedValue, section->centerX, section->centerY, WIDTH);


	str.append(to_string(section->endedX*RATIO*(10)));

	strCenter.append(to_string((section->endedX + otherPositions.at(0))*RATIO*(5)));

	strSub.append(to_string(otherPositions.at(0)*RATIO*(10)));

	str.append(" ");					strCenter.append(" ");					strSub.append(" ");
	str.append(to_string(section->endedY*RATIO*(-10)));

	strCenter.append(to_string((section->endedY + otherPositions.at(1))*RATIO*(-5)));

	strSub.append(to_string(otherPositions.at(1)*RATIO*(-10)));
	
	str.append("\"");					strCenter.append("\"");					strSub.append("\"");
	str.append(" style=\"stroke:red; fill:transparent\" />\n");

	strCenter.append(" style=\"stroke:violet; fill:transparent\" />\n");

	strSub.append(" style=\"stroke:blue; fill:transparent\" />\n");

	str.append("\n");
	str.append(strCenter);
	str.append("\n");
	str.append(strSub);
	str.append("\n");

	return str;
}


string drawLines(Section *section, int WIDTH){
	double xNormal, yNormal;
	//double xIn, yIn, xOut, yOut;
	string str = "";
	//if(section->param > 0 && section->side == RightSide
	//	|| (section->param < 0 && section->side == LeftSide)
	//	 )
		 {
		getNormalInAndOut(section->endedX - section->beganX, 
								section->endedY - section->beganY, NULL, NULL, 
								&xNormal, &yNormal);
		//xNormal = xOut;
		//yNormal = yOut;
	}
	//else{
	//	getNormalInAndOut(section->endedX - section->beganX, 
	//							section->endedY - section->beganY, 
	//							&xNormal, &yNormal);
	//}

	xNormal *= WIDTH*10;//To be honest, it should be xNormal *= WIDTH/RATIO
	yNormal *= WIDTH*(-10);

	#pragma region draw the side
	str.append("\n<line x1='");
	str.append(to_string(section->beganX*RATIO*(10)));
	str.append("' y1='");
	str.append(to_string(section->beganY*RATIO*(-10)));
	str.append("' x2='");
	str.append(to_string(section->endedX*RATIO*(10)));
	str.append("' y2='");
	str.append(to_string(section->endedY*RATIO*(-10)));
	str.append("' style=\"stroke:red; fill:transparent\" />\n");
	#pragma endregion

	int signedValue = section->param >= 0 ? 1 : -1;
	#pragma region draw the center axes
	str.append("\n<line x1='");
	str.append(to_string((section->beganX*RATIO*(10) + signedValue*xNormal/2)));
	str.append("' y1='");
	str.append(to_string((section->beganY*RATIO*(-10)  + signedValue*yNormal/2)));
	str.append("' x2='");
	str.append(to_string((section->endedX*RATIO*(10) + signedValue*xNormal/2)));
	str.append("' y2='");
	str.append(to_string((section->endedY*RATIO*(-10) + signedValue*yNormal/2)));
	str.append("' style=\"stroke:violet; fill:transparent\" />\n");
	#pragma endregion

	#pragma region draw the remaining side
	str.append("\n<line x1='");
	str.append(to_string((section->beganX*RATIO*(10) + signedValue*xNormal)));
	//To be honest, it should be (section->beganX + xNormal)*RATIO*(10). 
	// But I use the shortcut to reduce number of FP arithmetic calculation

	str.append("' y1='");
	str.append(to_string((section->beganY*RATIO*(-10) + signedValue*yNormal)));
	str.append("' x2='");
	str.append(to_string((section->endedX*RATIO*(10) + signedValue*xNormal)));
	str.append("' y2='");
	str.append(to_string((section->endedY*RATIO*(-10) + signedValue*yNormal)));
	str.append("' style=\"stroke:blue; fill:transparent\" />\n");
	#pragma endregion

	return str;

}


string drawCurverMovement(vector<Path*> trajectory, int WIDTH){
	string str = "\n";

	vector<char> typeOfTrajectory;
	lineSegment prev;
	int numPaths = trajectory.size();
	char prevTypeOfTrajectory = ' ';
    for(int i = 0; i < numPaths; i++){
        Path* path = trajectory.at(i);
        //cout<<"Path "<<i<<endl;
        vector<PathSegment*> segments = path->segments;
		if(segments.size() == 1){
			vector<Section*> sections = segments.at(0)->sections;
			int numSections = sections.size();
			for(int k = 0; k < numSections; k++)
			{
				
				if(sections.at(k)->typeOfTrajectory == 'C'){
					str.append(drawCircleArc(sections.at(k), WIDTH));
				}
				else if(sections.at(k)->typeOfTrajectory == 'F'){
					str.append(drawLines(sections.at(k), WIDTH));
				}
			}
		}
    }
	return str;
}

void drawShortestPath(string file_name, point & start, point & end, 
						int WIDTH,
						vector <vector < lineSegment > > & polygons,
						//vector<point> &sideSteps, 
						vector<point> &points, 
						vector<point> &route,
						vector<Path*> trajectory,
						vector< vector<int> >graph){
	string str1 = "<?xml version='1.0' encoding='UTF-8' ?>\n";
	string str2 = "<svg viewBox='"; //+ 
	str2.append(std::to_string(10*min_x-5));
	str2.append(" ");
	str2.append(std::to_string(/*-10*max_y-45*/-655));
	str2.append(" ");
	str2.append(std::to_string((std::abs(min_x)+std::abs(max_x))*10+10));
	str2.append(" ");
	str2.append(std::to_string((955/*abs(min_y)+abs(max_y))*10 + 80)*/)));
	str2.append("' xmlns='http://www.w3.org/2000/svg' version='1.1'>\n");

	str1 = str1 + str2;

	str1 = str1 + drawPolygons(polygons);

	str1 = str1 + drawPoint(start,"#FFA500");
	str1 = str1 + drawPoint(end,"green");

	//str1 = str1 + drawTitle(testTitle,distance);
	str1 = str1 + drawGraph(graph,points);
	//if(distance!=-1 && config.drawRoute)
	{ 
		str1 = str1 + drawShortestRoute(route);
		//str1 = str1 + drawShortestRoute(sideSteps);
		str1 = str1 + drawCurverMovement(trajectory, WIDTH);
	}
	str1 = str1 + "</svg>\n";
	std::ofstream ofs(file_name.c_str());
	if (!ofs.good())
		cout<<"Could not write data to "<<file_name;

	ofs << str1;
	ofs.close();
}
#endif