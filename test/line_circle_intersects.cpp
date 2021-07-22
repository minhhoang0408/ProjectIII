#include <iostream>
#include <cmath>
#include <vector>

#include "structures.cpp"

using namespace std;

const double eps = 1e-14;

double sq (double x) {
  return x * x;
}

bool within(double x1, double y1, double x2, double y2, double x, double y) {
  // check (x, y) between ((x1, y1), (x2, y2))
  double d1 = sqrt(sq(x2 - x1) + sq(y2 - y1));    // distance between end-points
  double d2 = sqrt(sq(x - x1) + sq(y - y1));      // distance from point to one end
  double d3 = sqrt(sq(x2 - x) + sq(y2 - y));      // distance from point to other end
  double delta = d1 - d2 - d3;
  return fabs(delta) < eps;   // true if delta is less than a small tolerance
}

double fx(double A, double B, double C, double x) {
    return -(A * x + C) / B;
}
 
double fy(double A, double B, double C, double y) {
    return -(B * y + C) / A;
}

vector<Point2> intersects(Point2 p1, Point2 p2, Point2 cp, double r) {
    double x0 = cp.x, y0 = cp.y;
    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;
    double A = y2 - y1;
    double B = x1 - x2;
    double C = x2 * y1 - x1 * y2;
    double a = sq(A) + sq(B);
    double b, c, d;
    bool bnz = true;
    int cnt = 0;
    vector<Point2> result;
 
    if (fabs(B) >= eps) {
        // if B isn't zero or close to it
        b = 2 * (A * C + A * B * y0 - sq(B) * x0);
        c = sq(C) + 2 * B * C * y0 - sq(B) * (sq(r) - sq(x0) - sq(y0));
    } else {
        b = 2 * (B * C + A * B * x0 - sq(A) * y0);
        c = sq(C) + 2 * A * C * x0 - sq(A) * (sq(r) - sq(x0) - sq(y0));
        bnz = false;
    }
    d = sq(b) - 4 * a * c; // discriminant
    if (d < 0) {
        // line & circle don't intersect
        return result;
    }
 
    if (d == 0) {
        // line is tangent to circle, so just one intersect at most
        double x, y;
        if (bnz) {
            x = -b / (2 * a);
            y = fx(A, B, C, x);
            
        } else {
            y = -b / (2 * a);
            x = fy(A, B, C, y);
        }
        if (within(x1, y1, x2, y2, x, y)) {
          result.push_back(Point2(x, y));
        }
    } else {
        // two intersects at most
        d = sqrt(d);
        Point2 p1, p2;
        if (bnz) {
            double x = (-b + d) / (2 * a);
            double y = fx(A, B, C, x);
            p1 = Point2(x, y);
 
            x = (-b - d) / (2 * a);
            y = fx(A, B, C, x);
            p2 = Point2(x, y);
        } else {
            double y = (-b + d) / (2 * a);
            double x = fy(A, B, C, y);
            p1 = Point2(x, y);
 
            y = (-b - d) / (2 * a);
            x = fy(A, B, C, y);
            p2 = Point2(x, y);
        }
        if (within(x1, y1, x2, y2, p1.x, p1.y)) {
          result.push_back(p1);
        }
        if (within(x1, y1, x2, y2, p2.x, p2.y)) {
          result.push_back(p2);
        }
    }
 
    return result;
}

int main(void) {
  // test 
  Point2 cp = Point2(0, 0);
  double r = 1.0;
  vector<Point2> result = intersects(Point2(0, 0), Point2(2, 0), cp, r);
  for (int i = 0; i < result.size(); i++) {
    cout << " Point : ( " << result[i].x << " ," << result[i].y << " )" << endl;
  }
}