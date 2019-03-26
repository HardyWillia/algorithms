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

using namespace std;


struct Point{
    int x, y;
};

//Global variables
int count = 0;
Point p0;

//Specifics for Quick Hull
#define iPair pair<int, int>
set<iPair> quick_hull;
vector<Point> quick_hull_vector;

//Used for Jarvis March and QuickHull
vector<Point> jarvis_quick;

/*
    Funtions below would be more cohesive in a header file, but it is included in the main file for now.
*/

//Swap two points
int swap(Point &p1, Point &p2){
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

//Returns the point under the top point in the stack 
Point nextToTop(stack<Point> &S){
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}

//Returns distance between two points
int dist(Point p1, Point p2){
    return (p1.x - p2.x) * (p1.x-p2.x) + (p1.y - p2.y) * (p1.y- p2.y);
}

//Returns the orientation of the points
int orientation(Point p, Point q, Point r){
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
        return 0;
    return (val > 0) ? 1 : 2;
}

// Helps sort with respect to first point
int compare(const void *vp1, const void *vp2)
{
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;

    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (dist(p0, *p2) >= dist(p0, *p1)) ? -1 : 1;
    return (o == 2) ? -1 : 1;
}

// Returns side of a point that joins p1 and p2
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
// connecting p1 and p2
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

    // Set p0 to the lowest point we just found, then sort the rest of them
    swap(points[0], points[min]);
    p0 = points[0];

    qsort(&points[1], n - 1, sizeof(Point), compare);

    int m = 1;
    for (int i = 1; i < n; i++)
    {
        while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
            i++;
        points[m] = points[i];
        m++;
    }

    if (m < 3)
        return;

    // Push initial 3 points,
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

    // Export hull to file
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
    if (n < 3)
    {
        std::cout << "Must be more than 3 points" << std::endl;
        return;
    }

    // Find leftmost points
    int left = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[left].x)
            left = i;

    // Find all points on hull by finding the smallest angle point
    int p = left, q;
    do
    {
        jarvis_quick.push_back(points[p]);
        q = (p + 1) % n;

        for (int i = 0; i < n; i++)
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;

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
    int index = -1;
    int max_dist = 0;

    for (int i = 0; i < n; i++)
    {
        int temp = lineDist(p1, p2, points[i]);
        if (findSide(p1, p2, points[i]) == side && temp > max_dist)
        {
            index = i;
            max_dist = temp;
        }
    }

    if (index == -1)
    {
        quick_hull.insert(p1);
        quick_hull.insert(p2);
        return;
    }

    // recurse twice with smaller sets of points
    hullHelper(points, n, points[index], p1, -findSide(points[index], p1, p2));
    hullHelper(points, n, points[index], p2, -findSide(points[index], p2, p1));
}

// Find a convex hull using Quickhull
void quickHull(std::vector<iPair> points, int n, std::string outputFile)
{
    if (n < 3)
    {
        std::cout << "Must be more than 3 points" << std::endl;
        return;
    }

    // Find the largest and smallest X values
    int minX = 0, maxX = 0;
    for (int i = 1; i < n; i++)
    {
        if (points[i].first < points[minX].first)
            minX = i;
        if (points[i].first > points[maxX].first)
            maxX = i;
    }

    // Fun recursive function on each side of the dataset
    hullHelper(points, n, points[minX], points[maxX], 1);
    hullHelper(points, n, points[minX], points[maxX], -1);

    // Hull is done, now put in order to draw it right
    // **This is a time consuming process, and can skew results on hulls with many points**
    std::set<iPair>::iterator it;
    for (it = quick_hull.begin(); it != quick_hull.end(); ++it)
    {
        iPair point = *it;
        Point myPoint;
        myPoint.x = point.first;
        myPoint.y = point.second;
        quick_hull_vector.push_back(myPoint);
    }

    int ymin = quick_hull_vector[0].y, min = 0;

    // Go through all points and find one with least y value
    for (int i = 1; i < quick_hull_vector.size(); i++)
    {
        int y = quick_hull_vector[i].y;

        if ((y < ymin) || (ymin == y && quick_hull_vector[i].x < quick_hull_vector[min].x))
            ymin = quick_hull_vector[i].y, min = i;
    }

    // Set p0 to the lowest point we just found, then sort the rest of them
    swap(quick_hull_vector[0], quick_hull_vector[min]);
    p0 = quick_hull_vector[0];

    // Sort newly converted vector of points, just like beginning of graham scan
    qsort(&quick_hull_vector[1], quick_hull_vector.size() - 1, sizeof(Point), compare);

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
      std::string outputFile = "";
      //read your data points from dataFile (see class example for the format)
      
      if (algType[0]=='G') {
         //call your Graham Scan algorithm to solve the problem
         outputFile = "hull_G.txt";
      } 
      else if (algType[0]=='J') {
         //call your Javis March algorithm to solve the problem
         outputFile = "hull_J.txt";
      }
      else { //default 
         //call your Quickhull algorithm to solve the problem
         outputFile = "hull_Q.txt";
      }
      
      //write your convex hull to the outputFile (see class example for the format)
      //you should be able to visulize your convex hull using the "ConvexHull_GUI" program.
	
	return 0;
}