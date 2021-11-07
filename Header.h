#pragma once
#include <iostream>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

void printArray(int* array, int size);
void printMatrix(int** mat, int width, int height);


void filterNoise(Mat img, uint8_t threshold);
int automaticThreshold(Mat img);
int* pixelsBlackRows(Mat img);
int* pixelsBlackColumns(Mat img);


int** generate_rectangles(Mat rez);
int* frecvWithBorder(Mat img, int y0, int y1, int width);
int** defineRows(int* heightFrec, int size, int& matrix_size);
int** defineCols(int* heightFrec, int size, int& matrix_size);
int** define_rectangles(int** width_pairs, int** height_pairs, int size_width_pairs, int size_height_pairs);


void drawReactagles(Mat img, int** rectangles, int nrOfRectangles);
Mat RGB2GRAY(Mat input);
Mat textDetector(Mat img);

