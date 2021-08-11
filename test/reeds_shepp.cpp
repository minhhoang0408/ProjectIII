#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>
#include <unistd.h>
#include <fstream>

#include "utils.cpp"

using namespace std;

enum Steering
{
  LEFT = 1,
  RIGHT = 2,
  STRAIGHT = 3
};

enum Gear
{
  FORWARD = 1,
  BACKWARD = 2
};

class PathElement
{

private:
public:
  static float RATIO;
  int steering;
  int gear;
  float param;
  PathElement(float _param, int _steering, int _gear)
  {
    this->steering = _steering;
    this->param = _param;
    this->gear = _gear;
  }

  string repr()
  {
    string steering_str, gear_str, param_str;

    switch (this->steering)
    {
    case Steering::LEFT:
      steering_str = "left";
      break;
    case Steering::RIGHT:
      steering_str = "right";
      break;
    default:
      steering_str = "straight";
      break;
    }
    if (this->gear == Gear::FORWARD)
    {
      gear_str = "forward";
    }
    else
    {
      gear_str = "backward";
    }
    // problem: to_string -> can't round
    return "{ Steering: " + steering_str + "\tGear: " + gear_str + "\tdistance: " + to_string((roundf(this->param * 100) / 100)) + " }";
  }

  void reverse_steering()
  {
    switch (this->steering)
    {
    case Steering::LEFT:
      this->steering = Steering::RIGHT;
      break;
    case Steering::RIGHT:
      this->steering = Steering::LEFT;
      break;
    default:
      break;
    }
  }
  void reverse_gear()
  {
    if (this->gear == Gear::FORWARD)
    {
      this->gear = Gear::BACKWARD;
    }
    else
    {
      this->gear = Gear::FORWARD;
    }
  }
};

float PathElement::RATIO = 6.8f;

// long path_length(double path)

vector<PathElement> path1(double x, double y, double phi)
{

  /*
  """
  Formula 8.1: CSC (same turns)
  """
  */

  PolarCodinate polar1 = R(x, y);
  phi = deg2rad(phi);
  vector<PathElement> path;

  PolarCodinate ut = R(x - sin(phi), y - 1 + cos(phi));
  double v = M(phi - ut.theta);

  if (ut.theta >= 0 && ut.r >= 0 && v >= 0)
  {
    path.push_back(PathElement(ut.theta, Steering::LEFT, Gear::FORWARD));
    path.push_back(PathElement(ut.r, Steering::STRAIGHT, Gear::FORWARD));
    path.push_back(PathElement(v, Steering::LEFT, Gear::FORWARD));
  }

  return path;
}

vector<PathElement> path2(double x, double y, double phi)
{
  /*
  """
  Formula 8.2: CSC (opposite turns)
  """
  */

  PolarCodinate polar1 = R(x, y);
  phi = M(deg2rad(phi));
  vector<PathElement> path;

  PolarCodinate rt = R(x + sin(phi), y - 1 - cos(phi));

  if (rt.r * rt.r >= 4)
  {
    double u = sqrt(rt.r * rt.r - 4);
    double t = M(rt.theta + atan2(2, u));
    double v = M(t - phi);

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(u, Steering::STRAIGHT, Gear::FORWARD));
      path.push_back(PathElement(v, Steering::LEFT, Gear::FORWARD));
    }
  }

  return path;
}

vector<PathElement> path3(double x, double y, double phi)
{
  /*
  """
  Formula 8.3: C|C|C
  """
  */

  PolarCodinate polar1 = R(x, y);
  phi = M(deg2rad(phi));
  vector<PathElement> path;

  double xi = x - sin(phi);
  double eta = y - 1 + cos(phi);
  PolarCodinate rt = R(xi, eta);

  if (rt.r <= 4)
  {
    double A = acos(rt.r / 4);
    double t = M(rt.theta + M_PI / 2 + A);
    double u = M(M_PI - 2 * A);
    double v = M(phi - t - u);

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(u, Steering::RIGHT, Gear::BACKWARD));
      path.push_back(PathElement(v, Steering::LEFT, Gear::FORWARD));
    }
  }

  return path;
}

vector<PathElement> path4(double x, double y, double phi)
{
  /*
  """
  Formula 8.4 (1): C|CC
  """
  */

  PolarCodinate polar1 = R(x, y);
  phi = deg2rad(phi);
  vector<PathElement> path;

  double xi = x - sin(phi);
  double eta = y - 1 + cos(phi);
  PolarCodinate rt = R(xi, eta);

  if (rt.r <= 4)
  {
    double A = acos(rt.r / 4);
    double t = M(rt.theta + M_PI / 2 + A);
    double u = M(M_PI - 2 * A);
    double v = M(t + u - phi);

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(u, Steering::RIGHT, Gear::BACKWARD));
      path.push_back(PathElement(v, Steering::LEFT, Gear::FORWARD));
    }
  }

  return path;
}

vector<PathElement> path5(double x, double y, double phi)
{
  /*
  """
  Formula 8.4 (2): CC|C
  """
  */

  PolarCodinate polar1 = R(x, y);
  phi = deg2rad(phi);
  vector<PathElement> path;

  double xi = x - sin(phi);
  double eta = y - 1 + cos(phi);
  PolarCodinate rt = R(xi, eta);

  if (rt.r <= 4)
  {
    double u = acos(1 - (rt.r * rt.r) / 8);
    double A = asin(2 * sin(u) / rt.r);
    double t = M(rt.theta + M_PI / 2 - A);
    double v = M(t - u - phi);

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(u, Steering::RIGHT, Gear::FORWARD));
      path.push_back(PathElement(v, Steering::LEFT, Gear::BACKWARD));
    }
  }

  return path;
}

vector<PathElement> path6(double x, double y, double phi)
{
  /*
  """
  Formula 8.7: CCu|CuC
  """
  */

  PolarCodinate polar1 = R(x, y);
  phi = deg2rad(phi);
  vector<PathElement> path;

  double xi = x + sin(phi);
  double eta = y - 1 - cos(phi);
  PolarCodinate rt = R(xi, eta);

  if (rt.r <= 4)
  {
    double A, t, u, v;
    if (rt.r <= 2)
    {
      A = acos((rt.r + 2) / 4);
      t = M(rt.theta + M_PI / 2 + A);
      u = M(A);
      v = M(phi - t + 2 * u);
    }
    else
    {
      A = acos((rt.r - 2) / 4);
      t = M(rt.theta + M_PI / 2 - A);
      u = M(M_PI - A);
      v = M(phi - t + 2 * u);
    }

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(u, Steering::RIGHT, Gear::FORWARD));
      path.push_back(PathElement(u, Steering::LEFT, Gear::BACKWARD));
      path.push_back(PathElement(v, Steering::RIGHT, Gear::BACKWARD));
    }
  }
  return path;
}

vector<PathElement> path7(double x, double y, double phi)
{
  /*
  """
  Formula 8.8: C|CuCu|C
  """
  */
  PolarCodinate polar1 = R(x, y);
  phi = deg2rad(phi);
  vector<PathElement> path;

  double xi = x + sin(phi);
  double eta = y - 1 - cos(phi);
  PolarCodinate rt = R(xi, eta);
  double u1 = (20 - rt.r * rt.r) / 16;

  if (rt.r <= 6 && 0 <= u1 && u1 <= 1)
  {
    double u = acos(u1);
    double A = asin(2 * sin(u) / rt.r);
    double t = M(rt.theta + M_PI / 2 + A);
    double v = M(t - phi);

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(u, Steering::RIGHT, Gear::BACKWARD));
      path.push_back(PathElement(u, Steering::LEFT, Gear::BACKWARD));
      path.push_back(PathElement(v, Steering::RIGHT, Gear::FORWARD));
    }
  }

  return path;
}

vector<PathElement> path8(double x, double y, double phi)
{
  /*
  """
  Formula 8.9 (1): C|C[pi/2]SC
  """
  */

  PolarCodinate polar1 = R(x, y);
  phi = deg2rad(phi);
  vector<PathElement> path;

  double xi = x - sin(phi);
  double eta = y - 1 + cos(phi);
  PolarCodinate rt = R(xi, eta);

  if (rt.r >= 2)
  {
    double u = sqrt(rt.r * rt.r - 4) - 2;
    double A = atan2(2, u + 2);
    double t = M(rt.theta + M_PI / 2 + A);
    double v = M(t - phi + M_PI / 2);

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(M_PI / 2, Steering::RIGHT, Gear::BACKWARD));
      path.push_back(PathElement(u, Steering::STRAIGHT, Gear::BACKWARD));
      path.push_back(PathElement(v, Steering::LEFT, Gear::BACKWARD));
    }
  }

  return path;
}

vector<PathElement> path9(double x, double y, double phi)
{
  /*
  """
  Formula 8.9 (2): CSC[pi/2]|C
  """
  */

  PolarCodinate polar1 = R(x, y);
  phi = deg2rad(phi);
  vector<PathElement> path;

  double xi = x - sin(phi);
  double eta = y - 1 + cos(phi);
  PolarCodinate rt = R(xi, eta);

  if (rt.r >= 2)
  {
    double u = sqrt(rt.r * rt.r - 4) - 2;
    double A = atan2(u + 2, 2);
    double t = M(rt.theta + M_PI / 2 - A);
    double v = M(t - phi - M_PI / 2);

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(u, Steering::STRAIGHT, Gear::FORWARD));
      path.push_back(PathElement(M_PI / 2, Steering::RIGHT, Gear::FORWARD));
      path.push_back(PathElement(v, Steering::LEFT, Gear::BACKWARD));
    }
  }

  return path;
}

vector<PathElement> path10(double x, double y, double phi)
{
  /*
  """
  Formula 8.10 (1): C|C[pi/2]SC
  """
  */
  PolarCodinate polar1 = R(x, y);
  phi = deg2rad(phi);
  vector<PathElement> path;

  double xi = x + sin(phi);
  double eta = y - 1 - cos(phi);
  PolarCodinate rt = R(xi, eta);

  if (rt.r >= 2)
  {
    double t = M(rt.theta + M_PI / 2);
    double u = rt.r - 2;
    double v = M(M_PI - t - M_PI / 2);

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(M_PI / 2, Steering::RIGHT, Gear::BACKWARD));
      path.push_back(PathElement(u, Steering::STRAIGHT, Gear::BACKWARD));
      path.push_back(PathElement(v, Steering::RIGHT, Gear::BACKWARD));
    }
  }

  return path;
}

vector<PathElement> path11(double x, double y, double phi)
{
  /*
  """
  Formula 8.10 (2): CSC[pi/2]|C
  """
  */

  PolarCodinate polar1 = R(x, y);
  phi = deg2rad(phi);
  vector<PathElement> path;

  double xi = x + sin(phi);
  double eta = y - 1 - cos(phi);
  PolarCodinate rt = R(xi, eta);

  if (rt.r >= 2)
  {
    double t = M(rt.theta);
    double u = rt.r - 2;
    double v = M(phi - t - M_PI / 2);

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(u, Steering::STRAIGHT, Gear::FORWARD));
      path.push_back(PathElement(M_PI / 2, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(v, Steering::RIGHT, Gear::BACKWARD));
    }
  }

  return path;
}

vector<PathElement> path12(double x, double y, double phi)
{
  /*
  """
  Formula 8.11: C|C[pi/2]SC[pi/2]|C
  """
  */

  PolarCodinate polar1 = R(x, y);
  phi = deg2rad(phi);
  vector<PathElement> path;

  double xi = x + sin(phi);
  double eta = y - 1 - cos(phi);
  PolarCodinate rt = R(xi, eta);

  if (rt.r >= 4)
  {
    double u = sqrt(rt.r * rt.r - 4) - 4;
    double A = atan2(2, u + 4);
    double t = M(rt.theta + M_PI / 2 + A);
    double v = M(t - phi);

    if (t >= 0 && u >= 0 && v >= 0)
    {
      path.push_back(PathElement(t, Steering::LEFT, Gear::FORWARD));
      path.push_back(PathElement(M_PI / 2, Steering::RIGHT, Gear::BACKWARD));
      path.push_back(PathElement(u, Steering::STRAIGHT, Gear::BACKWARD));
      path.push_back(PathElement(M_PI / 2, Steering::LEFT, Gear::BACKWARD));
      path.push_back(PathElement(v, Steering::RIGHT, Gear::FORWARD));
    }
  }

  return path;
}

vector<PathElement> timeflip(vector<PathElement> path)
{
  /*
  """
  timeflip transform described around the end of the article
  """
  */
  for (PathElement &element : path)
  {
    element.reverse_gear();
  }
  return path;
}

vector<PathElement> reflect(vector<PathElement> path)
{
  /*
  """
  reflect transform described around the end of the article
  """
  */
  for (PathElement &element : path)
  {
    element.reverse_steering();
  }
  return path;
}

double path_length(vector<PathElement> path)
{
  /*
    """
    this one's obvious
    """
  */
  double sum = 0;
  for (PathElement element : path)
  {
    sum += element.param;
  }
  return sum;
}

vector<vector<PathElement>> get_all_paths(Point3 start, Point3 end, vector<vector<double>> edges = {})
{
  /*
  """
  Return a list of all the paths from start to end generated by the
  12 functions and their variants
  """
  */

  vector<function<vector<PathElement>(double, double, double)>> path_fns{path1, path2, path3, path4, 
                                                                         path5, path6, path7, path8, 
                                                                         path9, path10, path11, path12};
  vector<vector<PathElement>> paths;

  // get coordinates of end in the set of axis where start is (0,0,0)
  Point3 xyt = change_of_basis(start, end);
  double x = xyt.x;
  double y = xyt.y;
  double theta = xyt.theta;

  for (int i = 0; i <= path_fns.size() - 1; i++)
  {
    // get the four variants for each path type, cf article
    paths.push_back(path_fns[i](x, y, theta));
    paths.push_back(timeflip(path_fns[i](-x, y, -theta)));
    paths.push_back(reflect(path_fns[i](x, -y, -theta)));
    paths.push_back(reflect(timeflip(path_fns[i](-x, -y, theta))));
  }

  // remove path elements that have parameter 0
  for (vector<PathElement> &path : paths)
  {
    path.erase(
        remove_if(
            path.begin(),
            path.end(),
            [](const PathElement &item)
            { return item.param == 0; }),
        path.end());
  }

  // remove empty paths
  paths.erase(
      remove_if(
          paths.begin(),
          paths.end(),
          [](const vector<PathElement> &path)
          { return path.empty(); }),
      paths.end());

  // Chưa rõ polygon vector<float>
  // if edges.size() > 0 {
  //   // # remove curver cuts through polygon
  //   // #paths = list(filter(lambda e: cutThroughCurver(start, end, e, edges) == 0, paths))
  // }

  return paths;
}

vector<PathElement> get_optimal_path(Point3 start, Point3 end, vector<vector<double>> polygons = {})
{
  /*
    """
    Return the shortest path from start to end among those that exist
    """
  */

  double maxValue = INFINITY;
  vector<vector<PathElement>> paths = get_all_paths(start, end, polygons);
  int i_min = 0;
  double L_min = path_length(paths[0]);

  for (int i = 1; i < paths.size(); i++)
  {
    double L = path_length(paths[i]);
    if (L <= L_min)
    {
      L_min = L;
      i_min = i;
    }
  }

  return paths[i_min];
}

vector<vector<double>> get_all_polygons()
{
  cout << "Path at terminal when executing this file" << endl;
  char tmp[256];
  getcwd(tmp, 256);
  cout << tmp << endl;

  vector<vector<double>> edges;
  fstream f;
  f.open("polygons.txt", ios::in);
  if (f.is_open())
  {

    string line;
    getline(f, line);
    int numPolygons = stoi(line);
    for (int i = 0; i < numPolygons; i++)
    {
      if (f.eof())
      {
        break;
      }
      getline(f, line);
      int numEdges = stoi(line);
      for (int j = 0; j < numEdges; j++)
      {
        vector<string> coords = split(line, " ");
        double x1 = stod(coords[0]);
        double y1 = stod(coords[1]);
        double x2 = stod(coords[2]);
        double y2 = stod(coords[3]);
        edges.push_back({x1, y1, x2, y2});
      }
    }

    f.close();
  }
  return edges;
}

vector<double> readPlots()
{
  vector<double> pts;
  fstream f;
  f.open("trajectory.txt", ios::in);
  if (f.is_open())
  {
    string line;
    while (true)
    {
      if (f.eof())
      {
        break;
      }
      getline(f, line);
      int numEdges = stoi(line);
      for (int j = 0; j < numEdges; j++)
      {
        vector<string> coords = split(line, " ");
        double x1 = stod(coords[0]);
        double y1 = stod(coords[1]);
        double x2 = stod(coords[2]);
        double y2 = stod(coords[3]);
        // edges.push_back({x1, y1, x2, y2});
      }
    }

    f.close();
  }
  return pts;
}

int cutThroughCurver(Point3 start, Point3 end, double param, string steering, vector<vector<double>> edges)
{
  if (steering == "S")
  {
    return 0;
  }

  double deltaX = end.x - start.x;
  double deltaY = end.y - start.y;

  double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
  double midX = (start.x + end.x) * 0.5;
  double midY = (start.y + end.y) * 0.5;
  double xNormal = 0;
  double yNormal = 0;
  if (steering == "L")
  {
    xNormal = -deltaY / distance;
    yNormal = -deltaX / distance;
  }
  if (steering == "R")
  {
    xNormal = deltaY / distance;
    yNormal = deltaX / distance;
  }

  double alpha = param / 2;
  double Radius = 1; //#0.5*distance/math.sin(alpha)
  double deltaR = cos(alpha);
  double xR = midX + deltaR * xNormal;
  double yR = midY + deltaR * yNormal;

  // circle
  Point2 c = Point2(xR, yR);

  double maxX = start.x > end.x ? start.x : end.x;
  double minX = maxX == end.x ? start.x : end.x;

  double maxY = start.y > end.y ? start.y : end.y;
  double minY = maxY == end.y ? start.y : end.y;

  for (vector<double> &e : edges)
  {
    // vector<Point2> l {Point2(e[0], e[1]), Point2(e[2], e[3])};
    vector<Point2> i = intersects(Point2(e[0], e[1]), Point2(e[2], e[3]), c, Radius);
    if (i.size() >= 1)
    {
      double xIntersection = i[0].x;
      double yIntersection = i[1].y;

      if ((xIntersection > minX && xIntersection < maxX) && (yIntersection > minY && yIntersection < maxY))
      {
        return 1;
      }
      if (i.size() == 2)
      {
        xIntersection = i[1].x;
        yIntersection = i[1].y;
        if ((xIntersection > minX && xIntersection < maxX) && (yIntersection > minY && yIntersection < maxY))
        {
          return 1;
        }
      }
    }
  }

  return 0;
}
