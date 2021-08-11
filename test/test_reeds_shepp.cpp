#include <iostream>
#include <vector>
#include <string>

#include "reeds_shepp.cpp"

using namespace std;

// ROUTE = [(-2,4,180), (2,4,0), (2,-3,90), (-5,-6,240), (-6, -7, 160), (-7,-1,80)]

string getNameSteering(int code)
{
  switch (code)
  {
  case Steering::LEFT:
  {
    return "LEFT";
  }
  case Steering::RIGHT:
  {
    return "RIGHT";
  }
  case Steering::STRAIGHT:
  {
    return "STRAIGHT";
  }
  default:
    return "";
  }
}

string getNameGear(int code)
{
  switch (
      code)
  {
  case Gear::FORWARD:
    return "FORWARD";
  case Gear::BACKWARD:
    return "BACKWARD";
  default:
    return "";
  }
}

int main()
{
  vector<Point3> ROUTE = {Point3(-2, 4, 180), Point3(2, 4, 0), Point3(2, -3, 90), Point3(-5, -6, 240), Point3(-6, -7, 160), Point3(-7, -1, 80)};
  vector<PathElement> full_path;
  double total_length = 0;

  for (int i = 0; i < 1; i++)
  {
    Point3 x = change_of_basis(ROUTE[i], ROUTE[i+1]);
    cout << "Change of basic: " << x.x << " - " << x.y << " - " << x.theta << endl; 
    vector<PathElement> path = get_optimal_path(ROUTE[i], ROUTE[i + 1]);

    for (int j = 0; j < path.size(); j++)
    {
      full_path.push_back(path[j]);
    }

    total_length += path_length(path);
  }

  full_path = timeflip(full_path);

  cout << "Shortest path length: " << total_length << endl;

  for (int i = 0; i < full_path.size(); i++)
  {
    cout << "{ Steering: " << getNameSteering(full_path[i].steering) << "\t Gear: " << getNameGear(full_path[i].gear) << "\t distance: " << full_path[i].param << endl;
  }
}
