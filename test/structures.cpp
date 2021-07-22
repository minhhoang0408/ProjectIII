struct Point2
{
  double x;
	double y;
	Point2(double x,double y){
		this->x = x;
		this->y = y;
	}

	Point2(){

	}

	bool operator==(const Point2 &other) const {
		return (this->x==other.x && this->y==other.y);
	}
};

struct PolarCodinate
{
  double r;
	double theta;
	PolarCodinate(double r,double theta){
		this->r = r;
		this->theta = theta;
	}

	PolarCodinate(){

	}

	bool operator==(const PolarCodinate &other) const {
		return (this->r==other.r && this->theta==other.theta);
	}
};

struct Point3
{
  double x;
	double y;
  double theta;
	Point3(double x,double y, double theta){
		this->x = x;
		this->y = y;
    this->theta = theta;
	}

	Point3(){

	}

	bool operator==(const Point3 &other) const {
		return (this->x==other.x && this->y==other.y && this->theta==other.theta);
	}
};