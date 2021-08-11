#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>
#include <unistd.h>
#include <fstream>

#include "draw.cpp"

using namespace std;

int main(void)
{

  float firstAngle = 0;
  float lastAngle = 0;

  vector<pair<double, double>> pts;

  fstream f;
  f.open("../route.txt", ios::in);
  if (f.is_open())
  {
    string line;
    getline(f, line);
    firstAngle = stof(line);
    getline(f, line);
    lastAngle = stof(line);

    int i = 0;
    int prev = 0;
    getline(f, line);
    while (!f.eof())
    {
      float num = stof(line);
      if (i % 2 == 1)
      {
        pts.push_back(make_pair(prev / PathElement::RATIO, num / PathElement::RATIO));
      }
      getline(f, line);
      i += 1;
      prev = num;
    }
    f.close();
  }

  vector<Point3> PATH; // (x, y, theta) is tuple in Python
  PATH.push_back(Point3(pts[0].first, pts[0].second, firstAngle));

  for (int i = 1; i < pts.size() - 1; i++)
  {
    double dx = pts[i + 1].first - pts[i].first;
    double dy = pts[i + 1].second - pts[i].second;
    double theta = atan2(dy, dx);
    PATH.push_back(Point3(pts[i].first, pts[i].second, rad2deg(theta)));
  }
  PATH.push_back(Point3(pts[-1].first, pts[-1].second, lastAngle));

  /* DRAW GRAPHICS LIKE TURTLE ...
    .. Thinking...
    # init turtle
    tesla = turtle.Turtle()
    tesla.speed(0) # 0: fast; 1: slow, 8.4: cool
    tesla.shape('arrow')
    tesla.resizemode('user')
    tesla.shapesize(1, 1)

    screen = turtle.Screen()
    screen.screensize(2500, 4000)


    # draw vectors representing points in PATH
    for pt in PATH:
        draw.goto(tesla, pt)
        draw.vec(tesla)

    # draw all routes found
    tesla.speed(0)
    #edges = rs.get_all_polygons()
  */

  f.open("../trajectory.txt", ios::out);
  if (f.is_open())
  {
    string s = "#Paths " + to_string(PATH.size() - 1) + "\n";
    f << s;
    for (int i = 0; i < PATH.size() - 1; i++)
    {
      vector<vector<PathElement>> paths = get_all_paths(PATH[i], PATH[i + 1]);
      s = "_PathID " + to_string(i) + " from " + to_string(PATH[i].x) + " " + to_string(PATH[i].y) + " to " + to_string(PATH[i + 1].x) + " " + to_string(PATH[i + 1].y) + "\n";
      f << s;
      s = "__#Possible_Path_Segment " + to_string(paths.size()) + "\n";
      f << s;
      int j = 0;
      for (vector<PathElement> &path : paths)
      {
        s = "___SegmentID_" + to_string(j) + "_PathID_" + to_string(i);

        /*
        draw.set_random_pencolor(tesla)
        draw.goto(tesla, PATH[i])
        #first = PATH[i]
        #s = "____ " + str(PATH[i][0]) + " " + str(PATH[i][1]) + " " + str(PATH[i][2]) + "\n"
        */


      }
    }
  }
  for (int i = 0; i < pts.size(); i++)
  {
    // "first" and "second" are used to access
    // 1st and 2nd element of pair respectively
    cout << pts[i].first << " "
         << pts[i].second << endl;
  }
}