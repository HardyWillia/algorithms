// You need to complete this program for your second project.

/*
    Willia Hardy
    Algorithms Project 2
    2905326
*/

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 

//Libraries specific to this project
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <stack>


struct Point{
    int x, y;
};

//Global variables
int count = 0;
Point p0;

//Specifics for Quick Hull
#define iPair std::pair<int, int>
std::set<iPair> quick_hull;
std::vector<Point> quick_hull_vector;

//Used for Jarvis March and QuickHull
std::vector<Point> jarvis_quick;

/*
    Funtions below would be more cohesive in a header file, but it is included in the main file for now.
*/

//Swap two points, from Graham Scan adaptation
int swap(Point &p1, Point &p2){
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

//Returns the point under the top point in the stack, from Graham Scan adaptation
Point nextToTop(std::stack<Point> &S){
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}

//Returns the square of distance between two points, from Graham Scan adaptation
int dist(Point p1, Point p2){
    return (p1.x - p2.x) * (p1.x-p2.x) + (p1.y - p2.y) * (p1.y- p2.y);
}

//Returns the orientation of the points, from Graham Scan adaptation
// 0 --> p, q and r are colinear
// 1 --> Clockwise, right turn
// 2 --> Counterclockwise, left turn
int orientation(Point p, Point q, Point r){
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0; //colinear
    return (val > 0) ? 1 : 2; //right or left turn
}

// Helps sort an array of points with respect to first point,from Graham Scan adaptation
int compare(const void *vp1, const void *vp2)
{
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;

    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (dist(p0, *p2) >= dist(p0, *p1)) ? -1 : 1;
    return (o == 2) ? -1 : 1;
}

// Returns side of a point that joins p1 and p2, from Quick Hull adaptation
int findSide(iPair p1, iPair p2, iPair p)
{
    int val = (p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first);
    if (val > 0)
        return 1;
    if (val < 0)
        return -1;
    return 0;
}

// Return value proportional to distance between p and the line
// connecting p1 and p2, from Quick Hull adaptation
int lineDist(iPair p1, iPair p2, iPair p)
{
    return abs((p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first));
}

/*
    Implementation of Convex Hull Algorithms
*/

//Graham Scan adapted from: https://www.geeksforgeeks.org/convex-hull-set-2-graham-scan/
void grahamScan(std::vector<Point> points, int n, std::string outputFile)
{
    int ymin = points[0].y, min = 0;

    // Go through all points and find one with least y value
    for (int i = 1; i < n; i++)
    {
        int y = points[i].y;

        if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
            ymin = points[i].y, min = i;
    }

    // Place the bottom-most point at first position
    swap(points[0], points[min]);

    // Sort n-1 points with respect to the first point.
    // A point p1 comes before p2 in sorted ouput if p2
    // has larger polar angle (in counterclockwise
    // direction) than p1
    p0 = points[0];
    qsort(&points[1], n - 1, sizeof(Point), compare);

    // If two or more points make same angle with p0,
    // Remove all but the one that is farthest from p0
    // Remember that, in above sorting, our criteria was
    // to keep the farthest point at the end when more than
    // one points have same angle.
    int m = 1; //Initialize size of modified array
    for (int i = 1; i < n; i++)
    {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
            i++;
        points[m] = points[i];
        m++; //Update size of modified array
    }

    // If modified array of points has less than 3 points,
    // convex hull is not possible
    if (m < 3)
        return;

    // Create an empty stack and push first three points
    // to it.
    std::stack<Point> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);

    // Loop through the rest of the candidates, leaving hull points on the stack
    for (int i = 3; i < m; i++)
    {
        while (orientation(nextToTop(S), S.top(), points[i]) != 2)
            S.pop();
        S.push(points[i]);
    }

    // Export Graham Scan Convex Hull to a file
    std::ofstream file;
    file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
    while (!S.empty())
    {
        Point p = S.top();
        file << p.x << "\t" << p.y << "\n";
        S.pop();
    }
    file.close();
}

//Jarvis March adapted from: https://www.geeksforgeeks.org/convex-hull-set-1-jarviss-algorithm-or-wrapping/
void jarvisMarch(std::vector<Point> points, int n, std::string outputFile)
{
    //There must be at least 3 points
    if (n < 3) return;

    // Find the leftmost point
    int left = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[left].x)
            left = i;

    // Start from leftmost point, keep moving counterclockwise
    // until reach the start point again.  This loop runs O(h)
    // times where h is number of points in result or output.
    int p = left, q;
    do
    {
        //Add current point to result
        jarvis_quick.push_back(points[p]);

        // Search for a point 'q' such that orientation(p, x,
        // q) is counterclockwise for all points 'x'. The idea
        // is to keep track of last visited most counterclock-
        // wise point in q. If any point 'i' is more counterclock-
        // wise than q, then update q.
        q = (p + 1) % n;
        for (int i = 0; i < n; i++){
            // If i is more counterclockwise than current q, then
            // update q
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }
        // Now q is the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to
        // result 'hull'
        p = q;

    } while (p != left);

    // Export hull to file
    std::ofstream file;
    file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
    for (int i = 0; i < jarvis_quick.size(); i++)
    {
        file << jarvis_quick[i].x << "\t" << jarvis_quick[i].y << "\n";
    }
    file.close();
}

//Quick Hull adapted from: https://www.geeksforgeeks.org/quickhull-algorithm-convex-hull/
// Recursive helper for Quickhull function
void hullHelper(std::vector<iPair> points, int n, iPair p1, iPair p2, int side)
{
    int ind = -1;
    int max_dist = 0;

    // finding the point with maximum distance
    // from L and also on the specified side of L.
    for (int i = 0; i < n; i++)
    {
        int temp = lineDist(p1, p2, points[i]);
        if (findSide(p1, p2, points[i]) == side && temp > max_dist)
        {
            ind = i;
            max_dist = temp;
        }
    }

    // If no point is found, add the end points
    // of L to the convex hull.
    if (ind == -1)
    {
        quick_hull.insert(p1);
        quick_hull.insert(p2);
        return;
    }

    // Recurse twice with smaller sets of points
    // Recurse for the two parts divided by a[ind]
    hullHelper(points, n, points[ind], p1, -findSide(points[ind], p1, p2));
    hullHelper(points, n, points[ind], p2, -findSide(points[ind], p2, p1));
}

// Find a convex hull using Quickhull
void quickHull(std::vector<iPair> points, int n, std::string outputFile)
{
    // a[i].second -> y-coordinate of the ith point
    if (n < 3)
    {
        std::cout << "Quick Hull is not possible" << std::endl;
        return;
    }

    // Find the minimum and maximum x-coordinate values
    int min_x = 0, max_x = 0;
    for (int i = 1; i < n; i++)
    {
        if (points[i].first < points[min_x].first)
            min_x = i;
        if (points[i].first > points[max_x].first)
            max_x = i;
    }

    // Recursively find convex hull points on
    // one side and other side of line joining a[min_x] and
    // a[max_x]
    hullHelper(points, n, points[min_x], points[max_x], 1);
    hullHelper(points, n, points[min_x], points[max_x], -1);


    // Export hull to file
    std::ofstream file;
    file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
    for (int i = 0; i < quick_hull_vector.size(); i++)
    {
        file << quick_hull_vector[i].x << "\t" << quick_hull_vector[i].y << "\n";
    }
    file.close();
}

//Main program
int main(int argc, char *argv[])
{
   //Second project starts here
   if (argc < 3) 
      std::cout << "wrong format! should be \"a.exe algType dataFile\"";
   else {
      std::string algType = argv[1];
      std::string dataFilename = argv[2];
      //std::string outputFile = "";
      //read your data points from dataFile (see class example for the format)

      std::ifstream file;
      file.open(dataFilename.c_str());
      std::vector<Point> points;
      std::vector<iPair> quick_points;

      // Read datapoints from dataFile
      if (file.is_open())
      {
          int x, y;
          while (file >> x >> y)
          {
              Point myPoint;
              myPoint.x = x;
              myPoint.y = y;
              points.push_back(myPoint);
              quick_points.push_back(*new iPair(x, y));
          }
      }

      if (algType[0]=='G') {
         //call your Graham Scan algorithm to solve the problem
         std::clock_t start = std::clock();
         grahamScan(points, points.size(), "hull_G.txt");
         double duration = (std::clock() - start);
         std::cout << "Graham scan on " << dataFilename << " took " << duration << " ms" << std::endl;
      } 
      else if (algType[0]=='J') {
         //call your Javis March algorithm to solve the problem
         std::clock_t start = std::clock();
         jarvisMarch(points, points.size(), "hull_J.txt");
         double duration = (std::clock() - start);
         std::cout << "Jarvis's March on " << dataFilename << " took " << duration << " ms" << std::endl;
      }
      else { //default 
         //call your Quickhull algorithm to solve the problem
         std::clock_t start = std::clock();
         quickHull(quick_points, points.size(), "hull_Q.txt");
         double duration = (std::clock() - start);
         std::cout << "Quickhull on " << dataFilename << " took " << duration << " ms" << std::endl;
      }
   }
	return 0;
}