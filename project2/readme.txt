Willia Hardy 2905326
Algorithms Project 2 README

Program must be:
Compiled: g++ -std=c++11 convexhull435.cc -o project2
Ran: ./project2 [algType] datafile_name

Graham Scan was adapted from: https://www.geeksforgeeks.org/convex-hull-set-2-graham-scan/
Jarvis March was adapted from: https://www.geeksforgeeks.org/convex-hull-set-1-jarviss-algorithm-or-wrapping/
Quick Hull was adapted from: https://www.geeksforgeeks.org/quickhull-algorithm-convex-hull/

- The preliminary functions used for this program included swap, orientation, nextToTop, compare, findSide, dist, lineDist
- The Graham Scan
    - Phase 1
        - Sorting the points in the array. IF two point have the same y value, the point with the smaller x-coordinate will be considered.
    - Phase 2
        - Accepts or rejects points due to orientation (clockwise or counterclockwise, right or left turn, respecitvely). If it is not counterclockwise, it is discarded.
- Jarvis March
    - Start from the leftmost point (or point with minimum x-coordinate value) and keep wrapping points making left turns. Use the orientation adapted from Graham Scan here.
- Quick Hull
    - The implementation uses set to store points so that points can be printed in sorted order. A point is represented as a pair.