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

//#include "preprocess.cpp"
#include "PairGenerator.cpp"

#ifndef _PLANNING_CONTROLLER_
#define _PLANNING_CONTROLLER_



using namespace std;

class PlanningController{
    public:
        //double dotProduct;
        bool clockWise = false;
        
        
        PlanningController(){

        }

        
        
        vector<point> getTrajectory(BuildingPolygons* generator, point start, point end){
            vector<point> points = generator->points;
            vector< vector< lineSegment> > polygons = generator->polygons;
            
            vector< vector < int > > graph;
            vector< vector < double > > graphDistance;

            points.insert(points.begin(), start);
            points.push_back(end);

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
            //numberOfVirtualCrossing(crossesNumber, polygons, points);

            auto time2 = std::chrono::steady_clock::now();

            makeVisabilityGraph(graph, graphDistance, crossesNumber, points);

            auto time3 = std::chrono::steady_clock::now();


            //The graph is constructed call dijksta to calculate the distance
            double distance = dijkstra(graphDistance,graph, route, points);

            auto time4 = std::chrono::steady_clock::now();

            //std:string title = "Automated valet parking v = 24km/h";
            //Output the distance
            RawRoute* rawRoute = new RawRoute();
            //if(config.printGraph)
            {
                getRawRoute(rawRoute, route, points);
                cout<<"Raw route"<<endl;
                for(int i = 0; i < route.size(); i++){
                    cout<<route.at(i)<<" ";
                }
                vector<point> shortestPath = this->echo(rawRoute, polygons, route, points, generator);
                //this->movementOfTheSide(generator->getWIDTH(), generator->getLENGTH(), rawRoute->points, polygons);
                for(int i = 0; i < shortestPath.size(); i++){
                    cout<<"\t"<<i<<") Point("<<shortestPath.at(i).x<<", "<<shortestPath.at(i).y<<") "<<endl;
                }
                Homotopy* homotopy = new Homotopy(generator->getWIDTH(), generator->getLENGTH(), generator->getDirectionAtTheEnd());
                //vector<point> sideSteps = homotopy->sideStepRouting(shortestPath, polygons, points);
                //cout<<"After sideStepRouting=========="<<endl;
                //for(int i = 0; i < sideSteps.size(); i++){
                //    cout<<"\t"<<i<<") Point("<<sideSteps.at(i).x<<", "<<sideSteps.at(i).y<<") "<<endl;
                //}
                vector<point> rightDirection = homotopy->calculateClockwise(shortestPath, polygons, points
                        );
                cout<<"After calculateClockwise=========="<<endl;
                for(int i = 0; i < shortestPath.size(); i++){
                    cout<<"\t"<<i<<") Point("<<shortestPath.at(i).x<<", "<<shortestPath.at(i).y<<") "<<endl;
                }
                
                string fileName = "test/test";
                fileName += to_string(nmrMovement);
                fileName += ".svg";
                //drawShortestPath(fileName, start, end, polygons, distance, points, sideSteps, graph);
                if(distance != -1){
                    
                    runPythonCode("test\\AllPossibleTrajectories.py", rightDirection, generator->getDirectionAtTheEnd());
                    vector<Path*> paths = readRSFile("trajectory.txt", polygons, generator->getWIDTH(), rightDirection);

                    drawShortestPath(fileName, start, end, generator->getWIDTH(),
                                                 polygons, //sideSteps, 
                                                                    points, 
                                                                    //shortestPath, 
                                                                    rightDirection,
                                                                    //sideSteps,
                                                                    paths,
                                                                    graph);
                }
                else{
                    cout<<"Runtime error"<<endl;
                    exit(-1);
                }
                //drawShortestPath(fileName, start, end, polygons, distance, points, rawRoute->points, graph);
                nmrMovement++;
                return vector<point>();	
            }
            
        }

        point getMaxOfXY(point p1, point p2){
            double maxX = max(p1.x, p2.x);
            double maxY = max(p1.y, p2.y);

            point p;
            p.x = maxX; p.y = maxY;
            return p;
        }

        point getMinOfXY(point p1, point p2){
            double minX = min(p1.x, p2.x);
            double minY = min(p1.y, p2.y);

            point p;
            p.x = minX; p.y = minY;
            return p;
        }

        void getRawRoute(RawRoute* rawRoute, vector<int> & route,vector<point> & points){
            int current = route.size()-1;
            while(current != -1){
                point p = points[current%points.size()];
                current = route[current];
                rawRoute->insert(p);
            }
        }

        vector<point> echo(RawRoute* r, vector< vector< lineSegment> > polygons, vector<int> route, vector<point> & points, BuildingPolygons* generator){

            this->clockWise = isClockwise(r->points.at(0), r->points.at(1));
            

            vector<lineSegment> group;

            int last = r->points.size() - 1;
            int preLast = last - 1;

            double deltaX, deltaY;

            /*double uX = r->points.at(last).x - r->points.at(preLast).x;
            double uY = r->points.at(last).y - r->points.at(preLast).y;
            double u = sqrt(uX*uX + uY*uY);
            //double U = sqrt((WIDTH*WIDTH/4) + (LENGTH*LENGTH/4));
            uX = (uX/u)*(generator->getWIDTH()/2);
            uY = (uY/u)*(generator->getWIDTH()/2);

            

            if(uY == 0){
                vX = 0;
                vY = -uX*generator->getWIDTH()/2;
            }
            else{
                double tag = uX/uY;
                double tag_1 = 1/sqrt(1 + tag*tag);
                double sin = tag*tag_1;
                double cos = tag_1*0.5;
                vY = (generator->getWIDTH()/2)*sin;
                vX = (generator->getWIDTH()/2)*cos;
                double newDotProduct = uX*vY - uY*vX;
                if(newDotProduct > 0){
                    vX = -vX;
                }
            }*/
            switch(generator->getDirectionAtTheEnd()){
                case 8: deltaX = generator->getWIDTH()/2;
                        deltaY = 0;
                        break;
                case 2: deltaX = -generator->getWIDTH()/2;
                        deltaY = 0;
                        break;
                case 6: deltaX = 0;
                        deltaY = -generator->getWIDTH()/2;
                        break;
                case 4: deltaX = 0;
                        deltaY = generator->getWIDTH()/2;
                        break;
            }
            //point preLastPoint()
            r->points.at(last).x += deltaX;
            r->points.at(last).y += deltaY;

            //cout<<"Prelast x = "<<r->points.at(preLast).x<<" y = "<<r->points.at(preLast).y<<endl;
            cout<<"Last x = "<<r->points.at(last).x<<" y = "<<r->points.at(last).y<<endl;

            for(int i = 0; i < r->points.size(); i++){
                //cout<<"("<<r->points.at(i).x<<", "<<r->points.at(i).y<<") ";
                point p1 = r->points.at(i);

                for(int j = i + 2; j < r->points.size(); j++){
                    point p2 = r->points.at(j);
                    
                    lineSegment l;
                    l.p = p1;
                    l.q = p2;
                    
                    //int crossing = numberOfCuttingThrough(polygons,l);
                    bool crossing = cutThroughPolygons(l, polygons);
                    if(!crossing){
                        //if(!insidePolygon(l, polygons))
                        {
                            group.push_back(l);
                        }
                    }
                    else{
                        //cout<<"("<<l.p.x<<", "<<l.p.y<<") => ("
                        //	<<l.q.x<<", "<<l.q.y<<") crosses "<<crossing<<endl;
                    }
                }
            }
            cout<<"\nsize of group "<<group.size()<<endl;
            for(int  i = 0; i < group.size();  i++){
                cout<<"Line ("<<group.at(i).p.x<<", "<<group.at(i).p.y<<")=>("<<group.at(i).q.x<<", "<<group.at(i).q.y<<")\n";
            }
            
            double  **PATHS     = (double**)  NULL;  //edgelerin uzunluklari
            int **ROUTE2     = (int**) NULL;   //ROUTE mizi belirleriz
            double  *SHORTEST_PATH       = (double*)   NULL;  //o vertex in baslangica en kisa yolu
            double  *VISITED = (double*)   NULL;   

            CollectingPoints* collectionPoints = new CollectingPoints();
            
            vector<point> result = collectionPoints->assignValueToMatrix(PATHS, ROUTE2, SHORTEST_PATH, VISITED, route, points, group);

            //result.at(0).x += generator->getWIDTH()/2;
            //result.at(0).y -= generator->getLENGTH()/2;

            /*for(int i = 0; i < r->points.size(); i++){
                cout<<"("<<r->points.at(i).x<<", "<<r->points.at(i).y<<") ";
            }*/
            movementOfTheSide(generator->getWIDTH(), generator->getLENGTH(),    
                        result
                        //r->points
                        //, polygons
                        , r->points.at(last).x
                        , r->points.at(last).y
                        );
            
            return result;
            //return r->points;
        }

    private:
        int nmrMovement = 0;
        //double dotProductLeft;
        double dotProductRight;
        

        void movementOfTheSide(int WIDTH, int LENGTH, vector<point> &trajectory, double lastX, double lastY){ 

            trajectory.at(0).x += WIDTH/2;
            trajectory.at(0).y -= LENGTH/2;
            int last = trajectory.size() - 1;
            if(trajectory.at(last).x != lastX){
                trajectory.at(last).x = lastX;
                trajectory.at(last).y = lastY;
            }
            return;

        }

        bool isClockwise(point p1, point p2){
            double uX = 0;
            double uY = 1;
            double vX = p2.x - p1.x;
            //double vY = p2.y - p1.y;
            double uv = /*uX*vY*/ - vX;
            return (uv < 0);
        }
        //bool couldReachLine()
        
};




#endif