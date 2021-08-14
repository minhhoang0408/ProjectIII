#include <iostream>
#include <vector>
#include <string>

#include "reeds_shepp.cpp"

using namespace std;

const int SCALE = 200;

struct TRAJECTORY
{
  double x;
  double y;
  double param;
  string typeOfTraj;
  string typeOfSteering;

  TRAJECTORY(double x, double y, double param, string typeOfTraj, string typeOfSteering)
  {
    this->x = x;
    this->y = y;
    this->param = param;
    this->typeOfSteering = typeOfSteering;
    this->typeOfTraj = typeOfTraj;
  }
};

double scale_primitive(double x)
{
  return x * SCALE;
}

double unscale_primitive(double x)
{
  return x / SCALE;
}

Point3 scale_point(Point3 x)
{
  return Point3(x.x * SCALE, x.y * SCALE, x.theta * SCALE);
}

Point3 unscale_point(Point3 x)
{
  return Point3(x.x / SCALE, x.y / SCALE, x.theta / SCALE);
}

vector<TRAJECTORY> draw_path(vector<PathElement> path)
{
  // todo: Get Position

  vector<TRAJECTORY> trajectory;
  string typeOfTraj = "C";
  string typeOfSteering = "S";

  Point3 currentCoordinate;


  for (int i = 0; i < path.size(); i++)
  { 
    int gear = path[i].gear == Gear::FORWARD ? 1 : -1;
    if (path[i].steering == Steering::LEFT)
    {
      // set Pos
      typeOfSteering = "L";
    }
    else if (path[i].steering == Steering::RIGHT)
    {
      // set Pos
      typeOfSteering = "R";
    }
    else if (path[i].steering == Steering::STRAIGHT)
    {
      // set Pos
      typeOfTraj = "F";
      typeOfSteering = "S";
    }
    
    trajectory.push_back(TRAJECTORY(0, 0, gear * path[i].param, typeOfTraj, typeOfSteering));
    typeOfTraj = "C";
  }

  // print
  return trajectory;
}
