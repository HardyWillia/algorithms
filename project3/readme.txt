Willia Hardy
2905326
Algorithms Project 3

Run the program: 
g++ -std=c++11 project3.cpp -o project3
./project3 testcases/image_name.pgm [vseams] [hseams]

The program outputs a file with the same name of the input file with "_processed". It keeps the same file extension. So if you give it a .ppm, it will export a .ppm to the same location as the original file. 
EX- 	input: "imageName.ppm"
	    output: "imageName_processed.ppm"
Same with a .pgm:
EX- 	input: "imageName.pgm"
	    output: "imageName_processed.pgm"

The bonus was not completed. So the program will only remove the horizontal and vertical seams and make the image smaller. Which are the initial project requirements.

