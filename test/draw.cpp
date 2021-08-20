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

vector<TRAJECTORY> draw_path(vector<PathElement> path, Point3 startPoint)
{
  // todo: Get Position
  Turtle tesla;
  vector<TRAJECTORY> trajectory;
  string typeOfTraj = "C";
  string typeOfSteering = "S";

  Point3 currentCoordinate;
  
  tesla.setPos(startPoint.x, startPoint.y);
  tesla.setHeading(startPoint.theta);

  for (int i = 0; i < path.size(); i++)
  { 
    int gear = path[i].gear == Gear::_FORWARD ? 1 : -1;
    if (path[i].steering == Steering::_LEFT)
    {
      // set Pos
      tesla.circle(1, gear * rad2deg(path[i].param));
      typeOfSteering = "L";
    }
    else if (path[i].steering == Steering::_RIGHT)
    {
      // set Pos
      tesla.circle(-1, gear * rad2deg(path[i].param));
      typeOfSteering = "R";
    }
    else if (path[i].steering == Steering::_STRAIGHT)
    {
      // set Pos
      tesla.forward(gear * path[i].param);
      typeOfTraj = "F";
      typeOfSteering = "S";
    }
    
    trajectory.push_back(TRAJECTORY(tesla.getPos().x, tesla.getPos().y, gear * path[i].param, typeOfTraj, typeOfSteering));
    typeOfTraj = "C";
  }

  // print
  return trajectory;
}
