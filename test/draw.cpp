#include <iostream>
#include <vector>
#include <string>

#include "reeds_shepp.cpp"

using namespace std;

const int SCALE = 200;

struct TRAJECTORY {
  double x;
  double y;
  double gear;
  string typeOfTraj;
  string typeOfSteering;

  TRAJECTORY(double x, double y, double gear, string typeOfTraj, string typeOfSteering) {
    this->x = x;
    this->y = y;
    this->gear = gear;
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

/*
def vec(bob):
  """
  Draw an arrow.
  """
  bob.down()
  bob.pensize(3)
  bob.forward(scale(1.2))
  bob.right(25)
  bob.backward(scale(.4))
  bob.forward(scale(.4))
  bob.left(50)
  bob.backward(scale(.4))
  bob.forward(scale(.4))
  bob.right(25)
  bob.pensize(1)
  bob.up()

def goto(bob, pos, scale_pos=True):
  """
  Go to a position without drawing.
  """
  bob.up()
  if scale_pos:
      bob.setpos(scale(pos[:2]))
  else:
      bob.setpos(pos[:2])
  bob.setheading(pos[2])
  bob.down() 
*/

