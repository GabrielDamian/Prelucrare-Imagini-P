#pragma once
#include <iostream>
#include <cmath>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

void printArray(int* array, int size);
void printMatrix(int** mat, int width, int height);


void aplicareThreshold(Mat img, uint8_t threshold);
int automaticThreshold(Mat img);
int* blackPixelsOnEachRow(Mat img);
int* blackPixelsOnEachColumn(Mat img);
int findMostFreqBlackFromHist(int* arr, int size);

int** generateBoxesForText(Mat rez,int &OutputNrOfRectangles, int pixelsBetweenBoxes);
int* blackPixelsOnEachColumnWithBorderedRows(Mat img, int y0, int y1, int width);
int** heightCoordsOfEachTextFoundOnRows(int* heightFrec, int size, int& matrix_size);
int** widthCoordsOfEachTextFoundOnRows(int* heightFrec, int size, int& matrix_size,int verificari);

void drawReactagles(Mat img, int** rectangles, int nrOfRectangles);
Mat RGB2GRAY(Mat input);
void textDetector(Mat original, Mat output);
void btnDetector(Mat original, Mat output);
void characterDetector(Mat original, Mat output);

Mat generateLegendCustom(int w, int h);
Mat3b ataseazaLegenda(Mat img_rez, int width_legenda);


