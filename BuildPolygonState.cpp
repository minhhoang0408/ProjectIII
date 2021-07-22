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

#include "ReadRSData.cpp"

#ifndef _BUILD_POLYGON_
#define _BUILD_POLYGON_

using namespace std;

class BuildingPolygons{
    private:
        int WIDTH;
        int LENGTH; 
        int directionAtTheEnd;
        
        vector<point> checkedPoints;
        //Create vector for containing the points of the polygons
	    
        int ROWS;
        int COLUMNS;
        int cordX0;
        int cordY0;
        double highest;
        bool fourDirections[4] = {false};//0: RIGHT, 1: DOWN, 2: LEFT, 3: UP
        point d0, d1, d2, d3;
        point delta[4];
        
        
        queue<Slot> ongoingCheckedSlots;
       
        

        void getNeighborByIndex(int** arrayOfAVs, int index, int r, int c){
            int x = r; int y = c;
            //
            switch(index){
                case 0: x--; y--; break;
                case 1: x--; break;
                case 2: x--; y++; break;
                case 3: y++; break;
                case 4: x++; y++; break;
                case 5: x++; break;//previous version gets r++ here which is totally wrong 
                case 6: x++; y--; break;
                case 7: y--; break;
            }
            
            if(arrayOfAVs[x][y] == 1){
                Slot s(x, y);
                ongoingCheckedSlots.push(s);
                arrayOfAVs[x][y] = -1;
            }
            
            //return p;
        }

        void insertNeighborSlots(int** arrayOfAVs, Slot slot){
            int row = slot.row;
            int col = slot.column;
            int neighbors[8] = {(row - 1) | (col - 1), //0
                                (row - 1) | col, //1
                                (row - 1) | (col + 1) | (COLUMNS - col - 2), //2
                                (row) | (col + 1) | (COLUMNS - col - 2),      //3               
                                (row + 1) | (col + 1) | (COLUMNS - col - 2) | (ROWS - row - 2),  //4
                                (row + 1) | (col) | (ROWS - row - 2), //5
                                (row + 1) | (col - 1) | (ROWS - row - 2), //6
                                (row + 1) | (col + 1) | (COLUMNS - col - 2) | (ROWS - row - 2) //7
                                };
            for(int i = 0; i < 8; i++){
                if(neighbors[i] >= 0){
                    getNeighborByIndex(arrayOfAVs, i, row, col);
                }  
            }
        }


    public:
        //vector<lineSegment> edges;
        vector< vector<lineSegment> > polygons;
        vector<point> points;
        int NUMBER_STACKS;
        static const int  DISTANCE = 1;
        //int** checkedPoints;
        
        //vector<point> fourVertecies;

        point pA, pB, pC, pD;
        void getFourVertecies(int indexOfStack, int indexRow, int indexCol){
            int deltaY = indexOfStack*ROWS*LENGTH;
            int xCenter = indexCol*WIDTH + this->cordX0;
            int yCenter = -indexRow*LENGTH + this->cordY0 - deltaY;
            
            pA.x = xCenter - (WIDTH/2);
            pA.y = yCenter + (LENGTH/2);

            pB.x = xCenter + (WIDTH/2);
            pB.y = yCenter + (LENGTH/2);

            pC.x = xCenter + (WIDTH/2);
            pC.y = yCenter - (LENGTH/2);

            pD.x = xCenter - (WIDTH/2);
            pD.y = yCenter - (LENGTH/2);
        }

        void setDirectionAtTheEnd(int directionAtTheEnd){
            this->directionAtTheEnd = directionAtTheEnd;
        }

        int getDirectionAtTheEnd(){
            return this->directionAtTheEnd;
        }
        
        void getRawPolygons(int indexOfStack, int** arrayOfAVs){
            
            int r0 = 0, c0 = 0;
            for(int i = r0; i < ROWS; i++){
                for(int j = c0; j < COLUMNS; j++){
                    if(arrayOfAVs[i][j] == 1){//Tim thay mot diem chua thuoc polygon nao ca
                        Slot s;
                        
                        s.row = i; s.column = j;
                        arrayOfAVs[i][j] = -1;
                        ongoingCheckedSlots.push(s);
                        highest = INT64_MIN;
                        while(!ongoingCheckedSlots.empty()){
                            Slot temp = ongoingCheckedSlots.front();
                            getFourVertecies(indexOfStack, temp.row, temp.column);
                            insertNonExistedPoints(arrayOfAVs, temp);
                            insertNeighborSlots(arrayOfAVs, temp);
                            ongoingCheckedSlots.pop();
                            //pushLineSegment(temp, prev);
                        }
                        //cout<<"size of checked points: "<<checkedPoints.size()<<endl;
                        drawLineSegmentsOfPolygon(arrayOfAVs);
                        //cout<<"\n\n"<<endl;
                    }
                }
            }
            
        }

        void insertNonExistedPoints(int** arrayOfAVs, Slot center){
            //int i = 0;
            bool found = false;

            double xCenterGroup[4] = {0};  double yCenterGroup[4] = {0};
            

            point group[4] = {pA, pB, pC, pD};
            for(int i = 0; i < 4; i++){
                found = false;
                
                for(int j = 0; j < checkedPoints.size(); j++){
                    if(checkedPoints.at(j) == group[i]) 
                    {
                        found = true;
                        break;
                    }
                }

                if(!found){
                    xCenterGroup[0] = group[i].x - (WIDTH/2);  yCenterGroup[0] = group[i].y + (LENGTH/2);
                    xCenterGroup[1] = group[i].x + (WIDTH/2);  yCenterGroup[1] = group[i].y + (LENGTH/2);
                    xCenterGroup[2] = group[i].x + (WIDTH/2);  yCenterGroup[2] = group[i].y - (LENGTH/2);
                    xCenterGroup[3] = group[i].x - (WIDTH/2);  yCenterGroup[3] = group[i].y - (LENGTH/2);

                    int aroundAVs = 0;
                    for(int j = 0; j < 4; j++){
                        int row = ((int)(this->cordY0 - yCenterGroup[j])) / LENGTH;
                        int col = ((int)(xCenterGroup[j] - this->cordX0)) / WIDTH;
                        if((row | col | (COLUMNS - col - 1) | (ROWS - row - 1)) >= 0){
                            if(arrayOfAVs[row][col] != 0)
                                aroundAVs++;
                        }
                    }

                    if(aroundAVs < 4){
                        point p(group[i].x, group[i].y);
                        if(p.y > highest){
                            highest = p.y;
                        }
                        checkedPoints.push_back(p);
                    }
                }
            }
        }

        void loadWall(int*** walls, int numOfWalls = 8, int numOfVertices = 6){
            //double prevX, prevY;
            //enum TypeOfLineSegment type = TypeOfLineSegment.VIRTUAL;
            for(int i = 0; i < numOfWalls; i++){
                vector<lineSegment> edges;
                int countVertices = 0;
                for(int j = 0; j < numOfVertices; j++){
                    if(walls[i][j][2] != NONE){
                        countVertices++;
                    }
                }

                for(int j = 0; j < numOfVertices; j++){
                    if(walls[i][j][2] != NONE){
                        point p(walls[i][j][0], walls[i][j][1]);
                        checkedPoints.push_back(p);
                        point next(walls[i][(j + 1) % countVertices][0], walls[i][(j + 1) % countVertices][1]);
                        TypeOfLineSegment type = NONE;
                        switch(walls[i][j][2]){
                            case 0: type = AV_EDGE; break;
                            case 1: type = HENCE; break;
                            case 2: type = GATE; break;
                            //case 3: type = NONE; break;//never reach
                        }
                        lineSegment line(p, next, type);
                        edges.push_back(line);
                    }
                }
                polygons.push_back(edges);

            }
        }

        void drawLineSegmentsOfPolygon(int** arrayOfAVs){
            //get highest point
            int i = 0;
            
            for(; i < checkedPoints.size(); i++){
                if(highest == checkedPoints.at(i).y){
                    break;
                }
            }
            point highestPoint;
            highestPoint.x = checkedPoints.at(i).x;
            highestPoint.y = checkedPoints.at(i).y;


            checkedPoints.erase(checkedPoints.begin() + i);

            int index = getNeighborOfHighest(highestPoint);
            if(index != -1){
                vector<lineSegment> edges;
                point temp(0, 0);
                temp.x = checkedPoints.at(index).x;
                temp.y = checkedPoints.at(index).y;
                checkedPoints.erase(checkedPoints.begin() + index);
                point prev(0, 0);
                prev.x = highestPoint.x;
                prev.y = highestPoint.y;
                pushLineSegment(temp, prev, edges);
                //cout<<"\n\n\n\n("<<temp.x<<", "<<temp.y<<") ";
                do{
                    index = getNearestNeighborClockwise(arrayOfAVs, temp, prev);
                    if(index != -1){
                        prev.x = temp.x;
                        prev.y = temp.y;
                        temp.x = checkedPoints.at(index).x;
                        temp.y = checkedPoints.at(index).y;
                        checkedPoints.erase(checkedPoints.begin() + index);
                        pushLineSegment(temp, prev, edges);

                    }
                }while(checkedPoints.size() > 0 && index != -1);

                pushLineSegment(highestPoint, temp, edges);

                polygons.push_back(edges);

                for (int i = 0; i < edges.size(); i++){
                    lineSegment line = edges.at(i);
                    //cout<<"("<<line.p.x<<", "<<line.p.y<<") => ("<<line.q.x<<", "<<line.q.y<<") | ";
                    point vertex(line.p.x, line.p.y);
                    points.push_back(vertex);
                    //edges.erase(edges.begin());
                }

            }
        }

        int getNeighborOfHighest(point highestPoint){
            int i = 0;
            bool foundInRight = false;
            bool foundInDown = false;

            for(; i < checkedPoints.size(); i++){
                if((highestPoint.y == checkedPoints.at(i).y)
                    && (highestPoint.x == (checkedPoints.at(i).x - WIDTH))
                    ){
                    foundInRight = true; 
                    break;
                }
            }

            
            if(!foundInRight){
                i = 0;
                for(; i < checkedPoints.size(); i++){
                    if((highestPoint.x == checkedPoints.at(i).x)
                        && (highestPoint.y == (checkedPoints.at(i).y + LENGTH))
                        ){
                        foundInDown = true; 
                        break;
                    }
                }
            }
            if(foundInRight || foundInDown)
                return i;
            return -1;
        }

        int getNearestNeighborClockwise(int** arrayOfAVs, point current, point prev){
            int index = 0;
            fourDirections[0] = false;    fourDirections[1] = false;
            fourDirections[2] = false;    fourDirections[3] = false;
            for(int i = 0; i < 4; i++){
                index = 0;
                //cout<<"Sz: "<<checkedPoints.size()<<" ";
                for(index = 0; index < checkedPoints.size(); index++){
                    if(!(checkedPoints.at(index) == prev) && 
                        checkedPoints.at(index).x == current.x + delta[i].x
                        && checkedPoints.at(index).y == current.y + delta[i].y
                    ){
                        if(IsLineExisted(checkedPoints.at(index), current, arrayOfAVs)){
                            fourDirections[i] = true;
                            break;
                        }
                    }
                }
                if(fourDirections[i] == true){
                    return index;
                }
            }
            return -1;
        }

        bool IsLineExisted(point pA, point pB, int** arrayOfAVs){
            int row1, row2, col1, col2;
            if(pA.x == pB.x){
                col1 = ((int)(pA.x - (WIDTH/2)- this->cordX0))/WIDTH;
                col2 = col1 + 1;
                row1 = ((int)(this->cordY0 - (pA.y + pB.y)/2))/LENGTH;
                row2 = row1;
                if(col1 < 0){
                    return (arrayOfAVs[row2][col2] != 0);
                }
                if(col1 == this->COLUMNS - 1)
                    return (arrayOfAVs[row1][col1] != 0);
                return ((std::abs(arrayOfAVs[row1][col1]) ^ std::abs(arrayOfAVs[row2][col2])) == 1);
            }

            if(pA.y == pB.y){
                col1 = ((int)((pA.x + pB.x)/2 - this->cordX0))/WIDTH;
                col2 = col1;
                row1 = ((int)(this->cordY0 - (pA.y + (LENGTH/2))))/LENGTH;
                row2 = row1 + 1;
                if(row1 < 0){
                    return (arrayOfAVs[row2][col2] != 0);
                }
                if(row1 == this->ROWS - 1)
                    return (arrayOfAVs[row1][col1] != 0);
                
                return ((std::abs(arrayOfAVs[row1][col1]) ^ std::abs(arrayOfAVs[row2][col2])) == 1);
            }
            return false;
        }


        void pushLineSegment(point current, point previous, vector<lineSegment> &edges){
            if(edges.empty()){
                lineSegment l;
                l.p = previous;
                l.q = current;
                edges.push_back(l);
            }
            else{
                lineSegment l = edges.at(edges.size() - 1);
                double vX = l.q.x - l.p.x;
                double vY = l.q.y - l.p.y;

                double uX = current.x - previous.x;
                double uY = current.y - previous.y;

                double uv = uX*vX + uY*vY;
                if(uv == 0){
                    lineSegment newL;
                    newL.p = previous;
                    newL.q = current;
                    edges.push_back(newL);
                }
                else{
                    l.q = current;
                    edges.pop_back();
                    edges.push_back(l);
                }
            }
        }

        BuildingPolygons(int Row, int Column, int cordX, int cordY, int width, int length){
            this->ROWS = Row;
            this->COLUMNS = Column;
            this->cordX0 = cordX;
            this->cordY0 = cordY;
            WIDTH = width;
            LENGTH = length;
            
            d0.x = WIDTH;   d0.y = 0; //0: Right: 
            d1.x = 0;       d1.y = -LENGTH; //1: Down
            d2.x = -WIDTH;  d2.y = 0;       //2: Left
            d3.x = 0;       d3.y = LENGTH;  //3: Up
            delta[0] = d0;
            delta[1] = d1;
            delta[2] = d2;
            delta[3] = d3;

            highest = INT64_MIN;
        }

        point getSlot(int indexOfStack, int indexRow, int indexCol){
            int deltaY = indexOfStack*ROWS*LENGTH;
            int xCenter = indexCol*WIDTH + this->cordX0;
            int yCenter = -indexRow*LENGTH + this->cordY0 - deltaY;
            point p(xCenter, yCenter);
            return p;
        }

        

        point getPositionInGate(int gateNumber, int kStacks, bool enter){
            double x = 0, y = 0;
            double LAND_WIDTH = 2*this->WIDTH + 1;
            double deltaX = this->WIDTH;
            double deltaY = this->LENGTH/2;
            if(gateNumber == 1 || gateNumber == 3){
                x = 2*LAND_WIDTH + (this->COLUMNS/2)*this->WIDTH + (this->WIDTH/2);
                if(gateNumber == 1){
                    y = 4*LAND_WIDTH + (kStacks - 1)*2*LAND_WIDTH + kStacks*this->ROWS*this->LENGTH;
                    x += enter ? -deltaX : deltaX;
                }
                else{
                    y = 0;
                    x += enter ? deltaX : -deltaX;
                }
                
            }
            else if(gateNumber == 2 || gateNumber == 4){
                y = (4*LAND_WIDTH + (kStacks - 1)*2*LAND_WIDTH + kStacks*this->ROWS*this->LENGTH)/2;
                if(gateNumber == 2){
                    x = 0;
                    y += enter ? deltaY : -deltaY;
                }
                else{
                    x = 4*LAND_WIDTH + this->COLUMNS*this->WIDTH;
                    y += enter ? -deltaY : deltaY;
                }
            }
            else{
                cout<<"X coord of the dest point: ";
                cin>>x;
                cout<<"Y coord of the dest point: ";
                cin>>y;
            }
            point p(x, y);
            return p;
        }

        int getWIDTH(){
            return this->WIDTH;
        }

        int getLENGTH(){
            return this->LENGTH;
        }

        //draw the polygon again as an AV is going to get out its slot.
        virtual void removeEdgesAndVertices(int indexOfStack, int row, int column) {}

        
};

#endif