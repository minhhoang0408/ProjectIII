#include "naive.cpp"

int numberOfSuccess = 0;
int numberOfFails = 0;
void testing(lineSegment l1, lineSegment l2, int shouldCross){
	int didCross = crosses(l1,l2);
	if(didCross==shouldCross){
		numberOfSuccess++;
		cout << "success" << endl;
	}
	else{
		numberOfFails++;
		cout << "Fail!" << didCross << " " << shouldCross << endl;
	}
	 
}

int main(){
	testing(lineSegment(point(0,0),point(1,1)),lineSegment(point(1,1),point(1,0)),5);
	testing(lineSegment(point(0,1),point(1,1)),lineSegment(point(0,0),point(1,1)),5);
	testing(lineSegment(point(0,0),point(0,1)),lineSegment(point(0,0),point(1,1)),5);
	testing(lineSegment(point(0,0),point(1,0)),lineSegment(point(0,1),point(1,0)),5);

	testing(lineSegment(point(0,0),point(2,1)),lineSegment(point(0,2),point(4,0)),4);
	testing(lineSegment(point(0,0),point(2,1)),lineSegment(point(-1,0),point(0,4)),0);

	testing(lineSegment(point(0,0),point(1,1)),lineSegment(point(0,1),point(1,0)),4);
	testing(lineSegment(point(1,1),point(2,0)),lineSegment(point(1,0),point(8,7)),4);

	cout << "number of fails: " << numberOfFails << ", number of success: " << numberOfSuccess << endl;
	return 0;
}
