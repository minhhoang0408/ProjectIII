#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>
#include <unistd.h>
#include <fstream>

#include "drawTurtle.cpp"

using namespace std;

void runAllPossibleTrajectories()
{
  
  float firstAngle = 0;
  float lastAngle = 0;

  vector<pair<double, double>> pts;
  cout << "============================== SET UP =========================" << endl;
  fstream f;
  f.open("route.txt", ios::in);
  if (f.is_open())
  {
    string line;
    getline(f, line);
    firstAngle = stod(line);
    getline(f, line);
    lastAngle = stod(line);

    int i = 0;
    double prev = 0;
    getline(f, line);
    while (!f.eof())
    {
      float num = stod(line);
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

  vector<Point3> PATH;
  PATH.push_back(Point3(pts[0].first, pts[0].second, firstAngle));

  for (int i = 1; i < pts.size() - 1; i++)
  {
    double dx = pts[i + 1].first - pts[i].first;
    double dy = pts[i + 1].second - pts[i].second;
    double theta = atan2(dy, dx);
    PATH.push_back(Point3(pts[i].first, pts[i].second, rad2deg(theta)));
  }
  PATH.push_back(Point3(pts[pts.size() - 1].first, pts[pts.size() - 1].second, lastAngle));

  // cout << "============================== COMPUTE PATH:  " << PATH.size() << "=========================" << endl;

  fstream trajectoriesFile;
  trajectoriesFile.open("trajectory.txt", ios::out);
  if (trajectoriesFile.is_open())
  {
    cout << "+++ Open create trajectory.txt" << endl;
    vector<TRAJECTORY> localTrajectory;
    string s = "#Paths " + to_string(PATH.size() - 1) + "\n";
    trajectoriesFile << s;
    for (int i = 0; i < PATH.size() - 1; i++)
    { 
      cout << "PATH thứ : " << i << endl;
      vector<vector<PathElement>> paths = get_all_paths(PATH[i], PATH[i + 1]);
      s = "_PathID " + to_string(i) + " from " + to_string(PATH[i].x) + " " + to_string(PATH[i].y) + " to " + to_string(PATH[i + 1].x) + " " + to_string(PATH[i + 1].y) + "\n";
      trajectoriesFile << s;
      s = "__#Possible_Path_Segment " + to_string(paths.size()) + "\n";
      trajectoriesFile << s;
      for (int j = 0; j < paths.size(); j++)
      {
        s = "___SegmentID_" + to_string(j) + "_PathID_" + to_string(i);

        localTrajectory = draw_path(paths[j], PATH[i]);
        s = s + " " + to_string(localTrajectory.size()) + "\n";
        trajectoriesFile << s;

        for (int k = 0; k < localTrajectory.size(); k++)
        {
          s = "_____Section " + to_string(localTrajectory[k].x) + " ";
          s = s + to_string(localTrajectory[k].y) + " ";
          double param = round(localTrajectory[k].param * 100) / 100;
          s = s + to_string(param) + " ";
          string typeOfTraj = localTrajectory[k].typeOfTraj;
          s = s + typeOfTraj + " ";
          string steering = localTrajectory[k].typeOfSteering;
          s = s + steering + "\n";
          trajectoriesFile << s;
        }
      }
    }
    trajectoriesFile.close();
  }
  // cout << "============================== WRITE TRAJECTORIES PATH DONE =========================" << endl;
  fstream traceFile;
  traceFile.open("Trace.txt", ios::out);
  if (traceFile.is_open())
  {
    cout << "+++ Open create trajectory.txt" << endl;
    vector<PathElement> full_path;
    double total_length = 0;
    vector<TRAJECTORY> trajectory;
    string s = to_string(round(PathElement::RATIO * pts[0].first * 1000) / 1000) + " " + to_string(round(PathElement::RATIO * pts[0].second * 1000) / 1000) + " B\n";
    ;
    traceFile << s;
    for (int i = 0; i < PATH.size() - 1; i++)
    {
      vector<PathElement> path = get_optimal_path(PATH[i], PATH[i + 1]);

      for (int j = 0; j < path.size(); j++)
      {
        full_path.push_back(path[j]);
      }

      trajectory = draw_path(path, PATH[i]);
      for (int i = 0; i < trajectory.size(); i++)
      {
        s = to_string(PathElement::RATIO * trajectory[i].x) + " " + to_string(PathElement::RATIO * trajectory[i].y) + " " + to_string(trajectory[i].param) + " " + trajectory[i].typeOfTraj + " " + trajectory[i].typeOfSteering + "\n";
        traceFile << s;
      }

      total_length += path_length(path);
    }

    cout << "Shortest path length: " << total_length << endl;

    // for (int i = 0; i < full_path.size(); i++)
    // {
    //   cout << full_path[i].repr() << endl;
    // }
    traceFile.close();
  }
  cout << "============================== WRITE TRACE , END RUN =========================" << endl;
}

// int main(void)
// {
//   runAllPossibleTrajectories();
// }
