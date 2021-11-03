#pragma once
#pragma once
#include <iostream>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

void printArray(int* array, int size);
void filterNoise(Mat img, uint8_t threshold);
int* pixelsBlackRows(Mat img);
int* pixelsBlackColumns(Mat img);

int** defineRows(int* heightFrec, int size, int& matrix_size);
int** define_rectangles(int** width_pairs, int** height_pairs, int size_width_pairs, int size_height_pairs);
void drawReactagles(Mat img, int** rectangles, int nrOfRectangles);