#include <iostream> //cin cout
#include <cmath>

#include "line_circle_intersects.cpp"

#define M_PI 3.14159265358979323846


using namespace std;

double rad2deg(double rad)
{
  return 180 * rad / M_PI;
}

double deg2rad(double deg)
{
  return M_PI * deg / 180;
}

int sign(double x)
{
  return (x >= 0) ? 1 : -1;
}

double convertModPython(double dividend, double division) {
  return dividend - (division * floor(dividend/division));
}

double M(double theta)
{
  /*  
    """
     Return the angle phi = theta mod (2 pi) such that -pi <= theta < pi.
    """ 
  */
  // theta = fmod(theta, 2 * M_PI);
  theta = convertModPython(theta, 2*M_PI);
  if (theta < -M_PI)
    return theta + 2 * M_PI;
  if (theta >= M_PI)
    return theta - 2 * M_PI;
  return theta;
}

PolarCodinate R(double x, double y)
{
  /*
  """
  Return the polar coordinates (r, theta) of the point (x, y).
  """
  */
  PolarCodinate p;
  p.r = sqrt(x * x + y * y);
  p.theta = atan2(y, x);
  return p;
}

Point3 change_of_basis(Point3 p1, Point3 p2)
{
  /*
  """
    Given p1 = (x1, y1, theta1) and p2 = (x2, y2, theta2) represented in a
    coordinate system with origin (0, 0) and rotation 0 (in degrees), return
    the position and rotation of p2 in the coordinate system which origin
    (x1, y1) and rotation theta1.
  """
  */
  Point3 p;
  double theta1 = deg2rad(p1.theta);
  double dx = p2.x - p1.x;
  double dy = p2.y - p1.y;
  p.x = dx * cos(theta1) + dy * sin(theta1);
  p.y = -dx * sin(theta1) + dy * cos(theta1);
  p.theta = p2.theta - p1.theta;
  return p;
}

vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

double round5Digits(double value) {
  // Round up to 5 digits 
  return round(value * 100000) / 100000;
}

