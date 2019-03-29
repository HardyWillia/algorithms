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

The files to test each convex were generated with the main function of the ConvexHull_GUI.java file:
  
   public static void main(String[] args) {
      ConvexHull_GUI CH = new ConvexHull_GUI();
      boolean generateData = false; //set to true to generate the file of random data
      if (generateData){
         CH.testFileGen("triangle1000000.txt", "triangle", 1000000); //generate data points
         System.out.println("done");
         System.exit(0);
      }
      else {
         //visulizing the points and convex hull      
         String dataFile = "test.txt";
         String hullFile = "hull.txt";

         //The number attached to the files below were changed based on the number (n) of data points
         String circleFile = "circle10.txt"; 
         String oncircleFile = "onCircle10.txt";
         String rectangleFile = "rectangle10.txt";
         String triangleFile = "triangle10.txt";
      
         CH.readDataFile(dataFile, 'd');
         System.out.println(dataFile + " has "+CH.getData().size()+" points.");
         CH.readDataFile(hullFile, 'h');
         System.out.println(hullFile + " has "+CH.getHull().size()+" points.");
         CH.readDataFile(circleFile, 'c');
         System.out.println(circleFile + " has "+CH.getData().size()+" points.");
         CH.readDataFile(oncircleFile, 'o');
         System.out.println(oncircleFile + " has "+CH.getHull().size()+" points.");
         CH.readDataFile(rectangleFile, 'r');
         System.out.println(rectangleFile + " has "+CH.getData().size()+" points.");
         CH.readDataFile(triangleFile, 't');
         System.out.println(triangleFile + " has "+CH.getHull().size()+" points.");
      
         JFrame jf = new JFrame("Convex Hull");
         jf.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
         jf.add(CH);
         jf.pack();
         jf.setVisible(true);
      }
   } 