#include <iostream>
#include <cmath>

#include "utils.cpp"

using namespace std;

const float MIN_RADIAN = 0.001;

float getSin(float degree)
{
  float rad = deg2rad(degree);
  if (rad < MIN_RADIAN)
  {
    return rad;
  }
  else
  {
    return sin(rad);
  }
}

float getCos(float degree)
{
  float rad = deg2rad(degree);
  if (rad < MIN_RADIAN)
  {
    return round5Digits(sqrt(1 - rad * rad));
  }
  else
  {
    return cos(rad);
  }
}

float checkAlpha(float alpha)
{
  if (alpha >= 360)
  {
    return convertModPython(alpha, 360.0);
  }
  else if (alpha < 0)
  {
    if (alpha <= -360)
    {
      return convertModPython(alpha, 360.0);
    }
    return alpha + 360;
  }
  return alpha;
}

class Turtle
{
private:
  float x, y, alpha;

public:
  Turtle()
  {
    this->x = 0;
    this->y = 0;
    this->alpha = 0;
  }
  void setPos(float, float);
  void setHeading(float);
  Point2 getPos()
  {
    return Point2(x, y);
  };
  float getHeading()
  {
    return this->alpha;
  }
  void forward(float distance);
  void backward(float distance);
  void _rotate(float degree);
  void left(float);
  void right(float);
  void circle(float, float);
  void _goto(float);
};

void Turtle::setPos(float x, float y)
{
  this->x = x;
  this->y = y;
}

void Turtle::setHeading(float degree)
{
  this->alpha = degree;
}

void Turtle::_goto(float distance)
{
  this->x = round5Digits(this->x + distance * getCos(alpha));
  this->y = round5Digits(this->y + distance * getSin(alpha));
}

void Turtle::forward(float distance)
{
  this->_goto(distance);
}

void Turtle::backward(float distance)
{
  this->_goto(-distance);
}
void Turtle::_rotate(float degree)
{
  float newAlpha = this->alpha + degree;
  this->alpha = checkAlpha(newAlpha);
}

void Turtle::left(float degree)
{
  this->_rotate(degree);
}

void Turtle::right(float degree)
{
  this->_rotate(-degree);
}

void Turtle::circle(float radius, float degree)
{
  // Compute new Coordinate
  if (degree > 360)
  {
    // exception
    return;
  }

  float frac = degree / 360.0;
  cout << "Frac: " << frac << endl;
  int step = 1 + int(min(11 + abs(radius) / 6.0, 59.0));
  float w = 1.0 * degree / step; // góc ở tâm
  float w2 = w / 2;              // góc tiếp tuyến
  float l = 2.0 * radius * getSin(w2);

  if (radius < 0)
  {
    w = -w;
    w2 = -w2;
    l = -l;
  }
  this->_rotate(w2);
  for (int i = 0; i < step; i++)
  {
    this->_goto(l);
    this->_rotate(w);
  }
  this->_rotate(-w2);
}