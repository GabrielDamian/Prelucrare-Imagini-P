#pragma once
#include <iostream>
#include <cmath>
#include <limits.h>
#include <algorithm>
#include <vector>
#include <numeric>

#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

#define ROWS 12
#define COLS 21
#define ROWSRESIZE 12
#define COLSRESIZE 21
#define NROFCHARACTERS 62

struct Character {
	char ch;
	int fr[ROWS*COLS] = { 0 };
	Character(const char ch, int valori[], int marime = ROWS*COLS) {
		this->ch = ch;
		for (int i = 0; i < marime; ++i) {
			this->fr[i] = valori[i];
		}
	}
};

template<typename T>
void printArray(T* array, int size);
void printMatrix(int** mat, int width, int height);
void calculateCharacterValues(Mat);
int similarityIndex(int* sectionValues1, int* sectionValues2);
char getCharacterBySectionValues(int* values, int size);
Mat resizeTo(Mat img, uint width, uint height);

void aplicareThreshold(Mat img, uint8_t threshold);
int automaticThreshold(Mat img);
int* blackPixelsOnEachRow(Mat img);
int* blackPixelsOnEachColumn(Mat img);
int findMostFreqBlackFromHist(int* arr, int size);

int** generateBoxesForText(Mat rez,int &OutputNrOfRectangles, int pixelsBetweenBoxes);
int* blackPixelsOnEachColumnWithBorderedRows(Mat img, int y0, int y1, int width);
int** heightCoordsOfEachTextFoundOnRows(int* heightFrec, int size, int& matrixSize, int verificari);
int** widthCoordsOfEachTextFoundOnRows(int* heightFrec, int size, int& matrix_size,int verificari);

void drawReactagles(Mat img, int** rectangles, int nrOfRectangles);
Mat RGB2GRAY(Mat input);
void textDetector(Mat original, Mat output);
void btnDetector(Mat original, Mat output);
void characterDetector(Mat original, Mat output);
void checkboxDetector(Mat original, Mat output);

Mat generateLegendCustom(int w, int h);
Mat3b ataseazaLegenda(Mat img_rez, int width_legenda);