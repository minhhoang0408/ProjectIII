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
#include <float.h>


#include "generatePy.cpp"

#ifndef _READ_REEDS_SHEEP_
#define _READ_REEDS_SHEEP_

using namespace std;



vector<point> getSegmentOfCircle(double p1X, double p1Y, double p2X, double p2Y, double rotatedAngle, char steering);

int checkCollisionRegardlessVirtualGate(lineSegment &line, vector<vector<lineSegment>> &polygons//, int directionAtTheEnd = -1
)
{
    int collision = 0;
    int c = 0;
    for(int i = 0; i < polygons.size(); i++){
        if(!isVirtualGate(polygons.at(i))){
            for(size_t j=0;j<polygons[i].size();j++){
                //c = cutThrough(line,polygons[i][j]);
                c = doIntersect(line,polygons[i][j]);
                collision += c;
            }
        }
    }
    return collision;
}


bool collisionOfTrajectoryAndPolygon(point pT, vector<lineSegment> polygon, double X_MIN = FLT_MIN, 
                                        double X_MAX = FLT_MAX, 
                                        double Y_MIN = FLT_MIN, 
                                        double Y_MAX = FLT_MAX){
    double xT_RATIO = pT.x*RATIO;
    double yT_RATIO = pT.y*RATIO;
    if(xT_RATIO >= X_MIN && xT_RATIO <= X_MAX &&
                yT_RATIO >= Y_MIN && yT_RATIO <= Y_MAX
        && polygon.at(0).type != GATE
                )
    {
        int check = wn_PnPoly(pT, polygon, RATIO);//, xT, yT, true);
        if(check != 0){
            return true;//collide with one of the polygons
        }
    }
    return false;
}

//p1, p2: diem dau diem cuoi cua chuyen dong tron
//rotatedAngle: goc quay cua quy dao hinh tron (co the am hoac duong)
//steering: quay theo duong tron ben trai hay duong tron ben phai cua xe
int getPointsOfCircle(Section *section, vector<vector<lineSegment>> &polygons, vector<Range*> ranges, int WIDTH, point &first, point &last, float &penalty)
{
    double p1X = section->beganX;
    double p1Y = section->beganY;

    //double deltaP1X = p1X*cos(yawAngle);
    //double deltaP1Y = p1Y*sin(yawAngle);
    double p2X = section->endedX;
    double p2Y = section->endedY;   

    int n = std::abs((int)(section->param/SMALL_ANGLE));
    //the rotatedAngle (section->param) sometime is negative and then lets n < 0 unless you call abs func
    if(n <= 1){
        point p(p2X, p2Y);
        section->typeOfTrajectory = 'F';
        section->possiblePoints.push_back(p);
        return 1;
    }
        

    double midPointX = (p1X + p2X)/2;
    double midPointY = (p1Y + p2Y)/2;
    double x = p2X - p1X;
    double y = p2Y - p1Y;

    double R = 1; double R1 = 1;

    double xIn = 0, yIn = 0, xOut = 0, yOut = 0;
    if(section->param < 0){
        x = -x;
        y = -y;
    }
    getNormalInAndOut(x, y, &xIn, &yIn, &xOut, &yOut);
    //getNormalInAndOut(deltaP1X, deltaP1Y, &xIn, &yIn, &xOut, &yOut);
    double xNormal, yNormal;
    if(LEFT == section->steering)
    {
        xNormal = xOut;
        yNormal = yOut;
    }
    else{
        xNormal = xIn;
        yNormal = yIn;
    }

    penalty = 0;
    double directionX = last.x - first.x;
    double directionY = last.y - first.y;
    

    double h = (cos(section->param/2));
    double centerX = midPointX + h*xNormal;
    double centerY = midPointY + h*yNormal;

    section->centerX = centerX;
    section->centerY = centerY;

    #pragma region Comments for explaination of the below code    
    //sin(omega_T0) = (p1.y - centerY)/R;
    //cos(omega_T0) = (p1.x - centerX)/R;
    //sin(deltaOmega) = ...;
    //cos(deltaOmega) = ...;
    //for i = 1..n
    //  sin(omega_T) = sin(omega_T0)*cos(deltaOmega) + cos(omega_0)*sin(deltaOmega)
    //  cos(omega_T) = cos(omega_T0)*cos(deltaOmega) - sin(omega_0)*sin(deltaOmega)
    //  x(t) = centerX + R*cos(omega)
    //  y(t) = centerY + R*sin(omega)
    #pragma endregion
    double sin_omega_T0 = (p1Y - centerY)/R;
    
    double cos_omega_T0 = (p1X - centerX)/R;
    if(sin_omega_T0 > 1){ sin_omega_T0 = 1; cos_omega_T0 = 0; }
    if(sin_omega_T0 < -1){ sin_omega_T0 = -1; cos_omega_T0 = 0; }
    if(cos_omega_T0 > 1){ cos_omega_T0 = 1; sin_omega_T0 = 0; }
    if(cos_omega_T0 < -1){ cos_omega_T0 = -1; sin_omega_T0 = 0; }

    double sin_omega, cos_omega, x_t, y_t;
    double sin_omega_T = sin_omega_T0, cos_omega_T = cos_omega_T0;

    //rotatedAngle > 0 <=> clockwise <=> omega reduces
    double sin_deltaOmega = (section->param > 0 ? -SMALL_ANGLE : SMALL_ANGLE);
    int signedValue = section->steering == 'R' ? 1 : -1;
    if(LEFT == section->steering){
        sin_deltaOmega = -sin_deltaOmega;
        //signedValue = -signedValue;
    }
    double cos_deltaOmega = 1 - (SMALL_ANGLE*SMALL_ANGLE/2);
    double xT, yT;
    //point pT;
    double rangesX_MIN[polygons.size()], rangesX_MAX[polygons.size()];
    double rangesY_MIN[polygons.size()], rangesY_MAX[polygons.size()];
    for(int j = 0; j < polygons.size(); j++){
        rangesX_MIN[j] = ranges.at(j)->xMin*RATIO;
        rangesX_MAX[j] = ranges.at(j)->xMax*RATIO;
        rangesY_MIN[j] = ranges.at(j)->yMin*RATIO;
        rangesY_MAX[j] = ranges.at(j)->yMax*RATIO;
    }
    double xT_RATIO, yT_RATIO;
    
    R1 = signedValue*WIDTH/RATIO + R;
    //cout<<"R1 = "<<R1<<endl;
    //point pA, pB, pC, pD;
    point p0, p1, p2, p3;
    point arr[] = {//pA, pB, 
                    //pC, pD
                    p0, p1, p2, p3
                    };
    double LENGTH = 6;
    double product = 0;
    double M, A, B, C, D;
    lineSegment line(first, last);
    
    for(int i = 1; i < n; i++){
        sin_omega_T = sin_omega_T*cos_deltaOmega + cos_omega_T*sin_deltaOmega;
        cos_omega_T = cos_omega_T*cos_deltaOmega - sin_omega_T*sin_deltaOmega;
        xT = centerX + R*cos_omega_T;
        yT = centerY + R*sin_omega_T;
        arr[0].x = xT; arr[0].y = yT;
        //v = getXYOfOtherSide(xT, yT, signedValue, centerX, centerY, WIDTH);
        arr[1].x = //v.at(0);
                    centerX + R1*cos_omega_T;
        arr[1].y = //v.at(1);
                    centerY + R1*sin_omega_T;
        getNormalInAndOut(signedValue*WIDTH*cos_omega_T/RATIO, signedValue*WIDTH*sin_omega_T/RATIO, &xIn, &yIn);
        
        arr[2].x = arr[0].x + LENGTH*xIn/RATIO;
        arr[2].y = arr[0].y + LENGTH*yIn/RATIO;
        arr[3].x = arr[1].x + LENGTH*xIn/RATIO;
        arr[3].y = arr[1].y + LENGTH*yIn/RATIO;

        xT_RATIO = xT*RATIO;
        yT_RATIO = yT*RATIO;
        
        
        for(int j = 0; j < polygons.size(); j++){
            if(!isVirtualGate(polygons.at(j))){
                for(int k = 0; k < 4; k++){
                    if(collisionOfTrajectoryAndPolygon(arr[k]
                                                        //pC
                                                        , polygons.at(j)
                                                        , rangesX_MIN[j], 
                                rangesX_MAX[j], rangesY_MIN[j], rangesY_MAX[j])){
                        return -1;//collide with one of the polygons
                    }
                }
               
            }
            product = (xT_RATIO - first.x)*directionY - (yT_RATIO - first.y)*directionX;
            if(product < 0){
                M = getMABCOfLine(line, &A, &B, &C);
                D = std::abs(A*xT_RATIO + B*yT_RATIO + C)/M;
                penalty += D;
            }
            
        }
        point p(xT, yT);
        section->possiblePoints.push_back(p);
    }
    
    return 1;
}

PathSegment* readSegment(double x, double y, double nextX, double nextY, ifstream& infile, 
                            vector<vector<lineSegment>> &scaledPolygons, vector<Range*> ranges, int *error, int WIDTH
                            , point &p, point &q
                            ){
    PathSegment *segment = new PathSegment();
    segment->beganX = x;       segment->beganY = y;
    segment->endedX = nextX;   segment->endedY = nextY;
    string line;               string strTemp1;
    getline(infile, line);
    *error = 0;
    istringstream allSections(line);
    int numberSections = 0;
    float penalty = 0;//tính toán độ lệch so với đường đi bên tay phải (right roadside)
    
    if(allSections >> strTemp1 >> numberSections){
        double startX = x; 
        double startY = y;
        double p3X = 0, p3Y = 0;
        
        while(numberSections > 0){
            Section *section = new Section();
            getline(infile, line);
            istringstream sectionInfo(line);
            if(sectionInfo >> strTemp1 >> section->endedX 
                    >> section->endedY >> section->param
                    >> section->typeOfTrajectory >> section->steering){
                if(*error == 0){//Neu *error con bang 0
                //tuc chua tim thay section nao va cham voi polygon
                //thi con nap cac diem cua section do vao quy dao
                //nguoc lai: chi duyet file chu khong tinh toan cac diem cua section
                    int check = 1;
                    section->beganX = startX; section->beganY = startY; 
                    if(section->steering != 'S'){
                        check = getPointsOfCircle(section, scaledPolygons, ranges, WIDTH, p, q, penalty);
                    }
                    if(check == 1){
                        startX = section->endedX; startY = section->endedY; 
                        
                        segment->sections.push_back(section);
                        segment->L += std::abs(section->param);
                        segment->penalty += penalty;
                    }
                    else{
                        *error = 1;
                    }
                }
            }
            numberSections--;
        }
    }
    return segment;
}

void scaleAndGenerateRange(vector<Range*> &ranges, vector<vector<lineSegment>> &scaledPolygons, vector<vector<lineSegment>> &polygons){
    
    for(int i = 0; i < polygons.size(); i++){
        vector<lineSegment> lines;
        Range *rangeOfAPolygon = new Range();
        int j = 0;
        for(j = 0; j < polygons.at(i).size(); j++){
            
            point p(polygons.at(i).at(j).p.x/RATIO, polygons.at(i).at(j).p.y/RATIO);
            point q(polygons.at(i).at(j).q.x/RATIO, polygons.at(i).at(j).q.y/RATIO);

            if(rangeOfAPolygon->xMax < p.x)
                rangeOfAPolygon->xMax = p.x;
            if(rangeOfAPolygon->xMin > p.x)
                rangeOfAPolygon->xMin = p.x;

            if(rangeOfAPolygon->yMax < p.y)
                rangeOfAPolygon->yMax = p.y;
            if(rangeOfAPolygon->yMin > p.y)
                rangeOfAPolygon->yMin = p.y;

            lineSegment aLine(p, q, polygons.at(i).at(j).type);
            lines.push_back(aLine);
        }
        /*cout<<"Polygon "<<i<<" Max X = "
            <<RATIO*rangeOfAPolygon->xMax<<" "
            <<" Min X = "<<RATIO*rangeOfAPolygon->xMin
            <<" Max Y = "<<rangeOfAPolygon->yMax
            <<" Min Y = "<<RATIO*rangeOfAPolygon->yMin<<endl;*/
        ranges.push_back(rangeOfAPolygon);
        scaledPolygons.push_back(lines);
    }
}

vector<Path*> readPath(ifstream& infile, int numPaths, vector<vector<lineSegment>> &polygons, int WIDTH, vector<point> &rightDirection){
    string line;
    string strTemp, strTemp1, strTemp2, strTemp3;

    vector<Range*> ranges;      vector<vector<lineSegment>> scaledPolygons;
    scaleAndGenerateRange(ranges, scaledPolygons, polygons);
    
    double x, y, nextX, nextY, distance;
    int i;
    char typeOfTraj;
    vector<Path*> result;
    bool firstSetSide = true; enum SIDE side = LeftSide;
    int sizeOfPaths = numPaths;
    point firstPath(0, 0);
    point lastPath(0, 0);
    while(numPaths > 0){
        Path *path = new Path();
        
        getline(infile, line);      istringstream segment(line);
        if(segment >> strTemp1 >> i >> strTemp2 >> x >> y >> strTemp3 >> nextX >> nextY)
        {
            path->beganX = x;                  path->beganY = y;
            path->endedX = nextX;              path->endedY = nextY;
            int numOfSegmentsInPath = 0;
            getline(infile, line);
            istringstream possiblePathSegment(line);
            if(possiblePathSegment >> strTemp1 >> numOfSegmentsInPath){
                while(numOfSegmentsInPath > 0){
                    int error = 0;
                    
                    firstPath = rightDirection.at(sizeOfPaths - numPaths);
                    lastPath = rightDirection.at(sizeOfPaths - numPaths + 1);
                    PathSegment* segment = readSegment(x, y, nextX, nextY, infile, scaledPolygons, ranges, &error, WIDTH, firstPath, lastPath);

                    if(error == 0)
                    {
                        if(firstSetSide){
                            if(segment->sections.at(0)->steering == 'R')
                                side = RightSide;
                            firstSetSide = false;
                        }
                        segment->setSide(side);//It seems no need to use side

                        if(path->segments.size() == 0){
                            path->segments.push_back(segment);
                            path->Lmin = segment->L;
                            path->penaltyMin = segment->penalty;
                        }
                        else{
                            if(path->penaltyMin >= segment->penalty){
                                if(path->Lmin > segment->L || path->penaltyMin > segment->penalty){
                                    path->segments.erase(path->segments.begin());
                                    path->segments.push_back(segment);
                                    path->Lmin = segment->L;
                                    path->penaltyMin = segment->penalty;
                                }
                            }
                        }
                    }/*else{     cout<<"Va cham tai path "<<numPaths<<" segment "<<numOfSegmentsInPath<<endl;      }*/
                    numOfSegmentsInPath--;
                }
            }
        }
        numPaths--;
        //cout<<"Num paths: "<<numPaths<<endl;
        result.push_back(path);
    }

    return result;
}

void printReedSheppTrajectories(vector<Path*> trajectories){
    int numPaths = trajectories.size();
    for(int i = 0; i < numPaths; i++){
        Path* path = trajectories.at(i);
        cout<<"Path "<<i<<endl;
        vector<PathSegment*> segments = path->segments;
        int numPossibleSegments = segments.size();
        for(int j = 0; j < numPossibleSegments; j++){
            cout<<"\tSegment "<<j<<endl;
            vector<Section*> sections = segments.at(j)->sections;
            int numSections = sections.size();
            for(int k = 0; k < numSections; k++){
                int signedValue = getSignedValue(sections.at(k));
                cout<<"\t\t("<<RATIO*(sections.at(k)->beganX)<<", "<<RATIO*(sections.at(k)->beganY)
                    <<") to ("<<RATIO*(sections.at(k)->endedX)<<", "<<RATIO*(sections.at(k)->endedY)<<") param = "
                    <<sections.at(k)->param<<" traj = "<<sections.at(k)->typeOfTrajectory
                    <<" steering "<<sections.at(k)->steering
                    <<" along with "<<sections.at(k)->possiblePoints.size()
                    <<" pts. Signed value = "<<signedValue<<endl;
            }
        }
    }
}

vector<Path*> readRSFile(string fileName, vector<vector<lineSegment>> &polygons, int WIDTH, vector<point> &rightDirection){

    //vector<point> discreteTrajectory;
    vector<Path*> result; 
    ifstream infile(fileName);
    string line;
    char steering;
    string strTemp, strTemp1, strTemp2, strTemp3;
    int numPaths;
    double x, y, nextX, nextY, distance;
    int i;
    char typeOfTraj;
    getline(infile, line);
    istringstream firstIss(line);
    if (firstIss >> strTemp >> numPaths) 
    { 
        cout<<"number of paths: "<<numPaths<<endl;
        
        result = readPath(infile, numPaths, polygons, WIDTH, rightDirection);
        printReedSheppTrajectories(result);
    }
    cout<<"Close file"<<endl;
    infile.close();

    //return discreteTrajectory;
    return result;
}


//p1, p2: diem dau diem cuoi cua chuyen dong tron
//rotatedAngle: goc quay cua quy dao hinh tron (co the am hoac duong)
//steering: quay theo duong tron ben trai hay duong tron ben phai cua xe
vector<point> getSegmentOfCircle(double p1X, double p1Y, double p2X, double p2Y, double rotatedAngle, char steering)
{
    vector<point> segment;
    if(rotatedAngle == 0)
        return segment;

    double midPointX = (p1X + p2X)/2;
    double midPointY = (p1Y + p2Y)/2;
    double x = p2X - p1X;
    double y = p2Y - p1Y;
    double distance = sqrt(x*x + y*y)/2;

    double R = distance/sin(std::abs(rotatedAngle)/2);
    cout<<"R = "<<R<<"as angle = "<<rotatedAngle<<" dis "<<distance<<endl;

    //angle velocity
    double omegaVelocity = MAX_VELOCITY/R;
    double t = rotatedAngle/omegaVelocity;
    int n = (int)std::abs(t);

    if(n <= 1){
        return segment;//shortcut to prevent wasted floating-point calculation
    }

    double xIn = 0, yIn = 0, xOut = 0, yOut = 0;
    getNormalInAndOut(p2X - p1X, p2Y - p1Y, &xIn, &yIn, &xOut, &yOut);
    double xNormal, yNormal;
    if(LEFT == steering)
    {
        xNormal = xOut;
        yNormal = yOut;
    }
    else{
        xNormal = xIn;
        yNormal = yIn;
    }

    double h = distance/tan(std::abs(rotatedAngle)/2);
    double centerX = midPointX + h*xNormal;
    double centerY = midPointY + h*yNormal;
    
    //sin(omega_T0) = (p1.y - centerY)/R;
    //cos(omega_T0) = (p1.x - centerX)/R;
    //sin(deltaOmega) = ...;
    //cos(deltaOmega) = ...;
    //for i = 1..n
    //  sin(omega_T) = sin(omega_T0)*cos(deltaOmega) + cos(omega_0)*sin(deltaOmega)
    //  cos(omega_T) = cos(omega_T0)*cos(deltaOmega) - sin(omega_0)*sin(deltaOmega)
    //  x(t) = centerX + R*cos(omega)
    //  y(t) = centerY + R*sin(omega)
    double sin_omega_T0 = (p1Y - centerY)/R;
    double cos_omega_T0 = (p1X - centerX)/R;
    double sin_omega, cos_omega, x_t, y_t;
    double sin_omega_T = sin_omega_T0, cos_omega_T = cos_omega_T0;
    double sin_deltaOmega = sin(rotatedAngle/n);
    double cos_deltaOmega = cos(rotatedAngle/n);
    double xT, yT;
    for(int i = 1; i < n; i++){
        sin_omega_T = sin_omega_T*cos_deltaOmega + cos_omega_T*sin_deltaOmega;
        cos_omega_T = cos_omega_T*cos_deltaOmega - sin_omega_T*sin_deltaOmega;
        xT = centerX + R*cos_omega_T;
        yT = centerY + R*sin_omega_T;
        point p(xT, yT);
        segment.push_back(p);
    }
    return segment;
}
#endif