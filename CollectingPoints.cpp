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
#include <stdio.h>
#include <stdlib.h>       
#include <string.h>      
#include <time.h> 

#include "preprocess.cpp"

#ifndef _COLLECT_POINTS_
#define _COLLECT_POINTS_

#define MALLOC(t,n)    (t *) malloc( n*sizeof(t) ) 
#define MALLOC2D(t,n)  (t **) malloc(n*sizeof(t) )
//#define CHECKPTR(p)    if (!p) Error("Out of memory")

#define MAX_INT 999999999

using namespace std;


class CollectingPoints{

    public:
        CollectingPoints(){

        }

        vector<point> assignValueToMatrix(double  **PATHS, int **ROUTE2, double  *SHORTEST_PATH, double  *VISITED, vector<int> route, vector<point> & points, 
                                    vector<lineSegment> group){

            vector<point> trajectory;
            int current = route.size()-1;
            while(current != -1){
                point p = points[current % points.size()];
                
                //cout<<"\nroute["<<current<<"] = "<<route[current]<<" => ("<<p.x<<", "<<p.y<<")";
                trajectory.insert(trajectory.begin(), p);
                current = route[current];
            }

            int vertexNum = trajectory.size();
            
            SHORTEST_PATH = MALLOC( double , vertexNum );
            

            VISITED = MALLOC( double , vertexNum );
            

            PATHS = MALLOC2D( double , vertexNum );
            

            ROUTE2 = MALLOC2D( int , vertexNum );

            this->buildAdjMatrix(trajectory.size(), PATHS, ROUTE2, SHORTEST_PATH, VISITED);

            for(int  i = 0; i < trajectory.size() - 1;  i++){
                double x1 = trajectory.at(i).x;
                double y1 = trajectory.at(i).y;
                int  j = i+1; 
                double x2 = trajectory.at(j).x;
                double y2 = trajectory.at(j).y;
                PATHS[i][j] = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
            }

            for(int i = 0; i < group.size(); i++){
                int indexI = getIndex(group.at(i).p.x, group.at(i).p.y, trajectory);
                int indexJ = getIndex(group.at(i).q.x, group.at(i).q.y, trajectory);
                double x1 = group.at(i).p.x;
                double y1 = group.at(i).p.y;
                
                double x2 = group.at(i).q.x;
                double y2 = group.at(i).q.y;
                PATHS[indexI][indexJ] = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
            }

            for(int i = 0; i < vertexNum; i++)
            {
                for(int j = 0; j < vertexNum; j++)
                {
                    ROUTE2[i][j] = -1;
                }
            }

            vector<point> shortestPath;

            this->findTheRotutines(vertexNum, PATHS, ROUTE2, SHORTEST_PATH, VISITED);
            int i = vertexNum - 1;
            {
                for(int j=0; j<vertexNum; j++)
                {
                    if(ROUTE2[i][j] != -1){
                        shortestPath.push_back(trajectory.at(ROUTE2[i][j]));
                        double x = trajectory.at(ROUTE2[i][j]).x;
                        double y = trajectory.at(ROUTE2[i][j]).y;
                    }
                }
            }
            return shortestPath;
        }

        int getIndex(double x, double y, vector<point> trajectory){
            int index = 0;
            for(int i = 0; i < trajectory.size(); i++){
                if(trajectory.at(i).x == x && trajectory.at(i).y == y){
                    index = i;
                    return index;
                }
            }
            return index;
        }
    
        //////////////////////////////////////
        //
        //  find the shortest paths for the beginnig vertex
        //  and store the routines that on which vertex can we go..
        //
        //////////////////////////////////////
        void findTheRotutines(int vertexNum, double  **PATHS, int **ROUTE2, double  *SHORTEST_PATH, double  *VISITED)
        {
            int i, j, k;
            int source = 0; 
            int my_vertex;
            int ek;

            SHORTEST_PATH[source ] = 0;
            my_vertex              = source; //o anda uzerinde islem yaptigimiz dugum..


            i=0;
            while( i++ < vertexNum )
            {
                //we will decide to which vertex is closer that is not in Y (not visited)
                for(j=0; j<vertexNum; j++)
                {
                    //if we find a vertex that is not Y, lets work on it
                    if( VISITED[j] == 0 )
                    {
                        //if there is a connection with the vertexes
                        if( PATHS[my_vertex][j] != -1 )
                        {
                            //decide the shortest paths
                            if( SHORTEST_PATH[j] > PATHS[my_vertex][j] + SHORTEST_PATH[my_vertex] )
                            {
                                //we found more shortest way.. change the old one
                                SHORTEST_PATH[j] = PATHS[my_vertex][j] + SHORTEST_PATH[my_vertex];

                                for(k=0; k<vertexNum; k++ )
                                {
                                    ROUTE2[j][k] = ROUTE2[my_vertex][k];
                                }

                                k=0;
                                while(ROUTE2[j][k] != -1 )
                                {
                                    k++;
                                }
            
                                ROUTE2[j][k] = my_vertex;	
                            }
                            
                        }
                    }
                    
                }
                

                ek = MAX_INT;

                for( j=1; j<vertexNum; j++)
                {
                    if( VISITED[j] == 0 )
                    {
                        if( SHORTEST_PATH[j] < ek )
                        {
                            ek = SHORTEST_PATH[j];
                            my_vertex = j;
                        }
                    }
                }

                VISITED[ my_vertex ] = 1;
            }


            
            //for( i=0; i<vertexNum; i++)
            i = vertexNum - 1;
            {
                
                if( ROUTE2[i][0] != -1 || source == i)
                {
                    k=0;
                    while(ROUTE2[i][k] != -1 )
                    {
                        k++;
                    }

                    ROUTE2[i][k] = i;
                }
            }

            cout<<"\n\n";
            //for(i=0; i<vertexNum; i++)
            
            

        }
        
    
    private:
        void buildAdjMatrix(int N, double  **PATHS, int **ROUTE2, double  *SHORTEST_PATH, double  *VISITED){
            int vertexNum = N;
            //mallocations dynamicly and check for errors 

            
            
            for(int i=0; i < vertexNum; i++)
            {
                PATHS[i] =  MALLOC( double , vertexNum );
                for(int j = 0; j < vertexNum; j++)
                {
                    PATHS[i][j] = -1;
                }

                ROUTE2[i] = MALLOC( int , vertexNum );

                //say that Y is empty
                VISITED[i] = 0; 

                //set the paths as very long 
                SHORTEST_PATH[i] = MAX_INT;

            }

        }

};



#endif