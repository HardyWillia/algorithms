/*
    Willia Hardy
    Algorithms Project 3
    2905326
*/

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "project3.h"

int main(int argc, char *argv[])
{

    if (argc < 4)
        std::cout << "Incorrect format: should be \"a.exe image.pgm/.ppm vseams hseams\"" << std::endl;
    else
    {
        int width = 0, height = 0, maxVal = 0;
        std::vector<std::vector<int>> pgm;
        std::vector<std::vector<Pixel>> ppm;
        std::string p_value;

        // Store arguments in variables
        std::string imageFilename = argv[1];
        std::string verticalString = argv[2];
        std::string horizontalString = argv[3];

        // turn arg seam #s into ints
        int vertical = std::stoi(argv[2]);
        int horizontal = std::stoi(argv[3]);


        std::string extension = imageFilename.substr(imageFilename.size() - 4, imageFilename.size());
        std::string outputFilename;

        //Define file
        if (extension == ".pgm")
            outputFilename = imageFilename.substr(0, imageFilename.size() - 4) + "_processed.pgm";
        else
            outputFilename = imageFilename.substr(0, imageFilename.size() - 4) + "_processed.ppm";

        std::ifstream file;
        file.open(imageFilename.c_str());
        if (file.is_open())
        {
            std::string chunk;
            int count = 0;
            // get first 3 values, p-number, width/height, and max greyscale val
            while (maxVal == 0)
            {
                file >> chunk;
                // if encounter a '#', then skip the rest of the line
                if (chunk[0] == '#')
                {
                    continue;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                std::stringstream myChunk(chunk);

                if (p_value.empty())
                    p_value = chunk;
                else if (width == 0)
                    myChunk >> width;
                else if (height == 0)
                    myChunk >> height;
                else
                    myChunk >> maxVal;
            }

            if (p_value == "P2")
            {
                // make 2d array of size gotten from top of pgm file
                populateVector(pgm, width, height);
                // fill the array
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        file >> chunk;
                        std::stringstream myVal(chunk);
                        myVal >> pgm[i][j];
                    }
                }
            }
            else
            {
                populateVector(ppm, width, height);
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        Pixel thisPixel;

                        file >> chunk;
                        std::stringstream myR(chunk);
                        myR >> thisPixel.R;

                        file >> chunk;
                        std::stringstream myG(chunk);
                        myG >> thisPixel.G;

                        file >> chunk;
                        std::stringstream myB(chunk);
                        myB >> thisPixel.B;

                        ppm[i][j] = thisPixel;
                    }
                }
            }
        }
        else
        {
            std::cout << "Failed to open file \"" << imageFilename << "\"" << std::endl;
        }

        if (maxVal != 0)
        {
            file.close();
            if (p_value == "P2")
            {

                // set pgm to the output of removing all the vertical seams
                removeSeams(pgm, vertical);

                // transpose the matrix so we can use the same function to remove horizontal seams
                pgm = transpose(pgm);

                // set pgm to the output of removing all the horizontal seams
                removeSeams(pgm, horizontal);

                // transpose back to get the original orientation
                pgm = transpose(pgm);

                // pgm is now the trimmed image, output to file with new dimensions
                std::ofstream outfile;
                outfile.open(outputFilename.c_str(), std::ofstream::out | std::ofstream::trunc);

                // export 4 initial entries
                outfile << p_value << "\n";
                outfile << "# Output after removing " << vertical << " vertical and " << horizontal << " seams from " << imageFilename << "\n";
                outfile << pgm[0].size() << " " << pgm.size() << "\n";
                outfile << maxVal << "\n";

                // export the matrix
                for (int i = 0; i < pgm.size(); i++)
                {
                    for (int j = 0; j < pgm[0].size(); j++)
                    {
                        outfile << pgm[i][j] << " ";
                    }
                    outfile << "\n";
                }
                outfile.close();
        }
        else
            {
                removeSeams(ppm, vertical);
                ppm = transpose(ppm);
                removeSeams(ppm, horizontal);
                ppm = transpose(ppm);

                std::ofstream outfile;
                outfile.open(outputFilename.c_str(), std::ofstream::out | std::ofstream::trunc);

                outfile << p_value << "\n";
                outfile << "# Output after removing " << vertical << " vertical and " << horizontal << " seams from " << imageFilename << "\n";
                outfile << ppm[0].size() << " " << ppm.size() << "\n";
                outfile << maxVal << "\n";
                for (int i = 0; i < ppm.size(); i++)
                {
                    for (int j = 0; j < ppm[0].size(); j++)
                    {
                        outfile << ppm[i][j].R << " " << ppm[i][j].G << " " << ppm[i][j].B << " ";
                    }
                    outfile << "\n";
                }
                outfile.close();
            }
            std::cout << "Output file exported to " << outputFilename << std::endl;
        }
    }
