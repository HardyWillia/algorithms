// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h>

// Libraries for this project
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <ctime>
#include <bits/stdc++.h>



// Size the matrix correctly based on input image width and height
void populateVector(std::vector<std::vector<int>> &matrix, int width, int height)
{
    matrix.resize(height);
    for (int i = 0; i < height; i++)
    {
        matrix[i].resize(width);
    }
}

// Transpose the matrix
std::vector<std::vector<int>> transpose(std::vector<std::vector<int>> &matrix)
{
    int width = matrix[0].size();
    int height = matrix.size();

    std::vector<std::vector<int>> transposedMatrix;
    populateVector(transposedMatrix, height, width);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            transposedMatrix[j][i] = matrix[i][j];
    return transposedMatrix;
}

// Print the matrix out for quick debugging
// Careful when using with large inputs
void printMatrix(std::vector<std::vector<int>> &matrix, std::string name)
{
    int height = matrix.size();
    int width = matrix[0].size();
    std::cout << name << std::endl;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


// Returns the value of the cell with the least value of the 3 above the given i, j
int minFromAbove(std::vector<std::vector<int>> &cumulative, int i, int j)
{
    int min = INT_MAX;

    if (j != 0)
        min = cumulative[i - 1][j - 1];
    if (cumulative[i - 1][j] < min)
        min = cumulative[i - 1][j];
    if (j != cumulative[i].size() - 1)
        if (cumulative[i - 1][j + 1] < min)
            min = cumulative[i - 1][j + 1];

    return min;
}

// Returns the coordinates in the cumulative energy matrix of the least of the 3 entries above the number at i, j
std::pair<int, int> minCoodsFromAbove(std::vector<std::vector<int>> &cumulative, int i, int j)
{
    std::pair<int, int> min;
    min.first = i - 1;
    // There will always be an element above this one, so default min j to it just in case
    min.second = j;
    if (j != 0)
        min.second = j - 1;
    if (cumulative[i - 1][j] < cumulative[min.first][min.second])
        min.second = j;
    if (j != cumulative[i].size() - 1)
        if (cumulative[i - 1][j + 1] < cumulative[min.first][min.second])
            min.second = j + 1;

    return min;
}

// Returns the coordinates in the cumulative energy matrix of the minimum value in one row
std::pair<int, int> minCoords(std::vector<std::vector<int>> &cumulative, int i)
{
    std::pair<int, int> min;
    min.first = i;
    min.second = 0;
    for (int j = 1; j < cumulative[i].size(); j++)
        if (cumulative[i][j] < cumulative[i][min.second])
            min.second = j;

    return min;
}

// Calculate the energy matrix
std::vector<std::vector<int>> calculateEnergyMatrix(std::vector<std::vector<int>> &matrix)
{
    int height = matrix.size();
    int width = matrix[0].size();
    std::vector<std::vector<int>> energyMatrix;
    populateVector(energyMatrix, width, height);
    int left, right, top, bottom;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                left = 0;
            else
                left = abs(matrix[i][j] - matrix[i][j - 1]);
            ;

            if (j == width - 1)
                right = 0;
            else
                right = abs(matrix[i][j] - matrix[i][j + 1]);

            if (i == 0)
                top = 0;
            else
                top = abs(matrix[i][j] - matrix[i - 1][j]);

            if (i == height - 1)
                bottom = 0;
            else
                bottom = abs(matrix[i][j] - matrix[i + 1][j]);

            energyMatrix[i][j] = left + right + top + bottom;
        }
    }

    return energyMatrix;
}


// Calculate cumulative matrix
std::vector<std::vector<int>> calculateCumulativeMatrix(std::vector<std::vector<int>> &energyMatrix)
{

    int height = energyMatrix.size();
    int width = energyMatrix[0].size();
    std::vector<std::vector<int>> cumulativeMatrix;
    populateVector(cumulativeMatrix, width, height);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0)
            {
                cumulativeMatrix[i][j] = energyMatrix[i][j];
            }
            else
            {
                cumulativeMatrix[i][j] = energyMatrix[i][j] + minFromAbove(cumulativeMatrix, i, j);
            }
        }
    }

    return cumulativeMatrix;
}

// Trace up from bottom and find the path of the lowest numbers
std::vector<std::pair<int, int>> findMinPath(std::vector<std::vector<int>> &cumulativeMatrix)
{
    int height = cumulativeMatrix.size();
    std::vector<std::pair<int, int>> path;
    path.push_back(minCoords(cumulativeMatrix, height - 1));
    for (int i = height - 1; i > 0; i--)
        path.push_back(minCoodsFromAbove(cumulativeMatrix, path.back().first, path.back().second));

    return path;
}


// Overload for matrices
void removeMinPath(std::vector<std::vector<int>> &matrix, std::vector<std::pair<int, int>> &minPath)
{
    for (int i = 0; i < minPath.size(); i++)
        matrix[minPath[i].first].erase(matrix[minPath[i].first].begin() + minPath[i].second);
}

// Main function to consolidate seam-removal functions
void removeSeams(std::vector<std::vector<int>> &matrix, int seams)
{
    std::vector<std::vector<int>> energyMatrix, cumulativeMatrix;
    std::vector<std::pair<int, int>> minPath;

    // remove "seams" amount of seams
    for (int count = 0; count < seams; count++)
    {
        energyMatrix = calculateEnergyMatrix(matrix);
        cumulativeMatrix = calculateCumulativeMatrix(energyMatrix);
        minPath = findMinPath(cumulativeMatrix);
        removeMinPath(matrix, minPath);
    }
}


