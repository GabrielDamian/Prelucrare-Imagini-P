#pragma once
#include <iostream>
#include <cmath>
#include <limits.h>
#include <algorithm>
#include <vector>
#include <numeric>
#include <map>
#include <utility>
#include <list>

#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>


#define ROWS 12
#define COLS 21
#define ROWSRESIZE 12
#define COLSRESIZE 21
#define NROFCHARACTERS 63

struct Character {
	char ch;
	std::vector<int> fr = std::vector<int>(ROWS* COLS,0);

	Character(const char ch, std::vector<int> valori, int marime = ROWS*COLS) {
		this->ch = ch;
		for (int i = 0; i < marime; ++i) {
			this->fr[i] = valori[i];
		}
	}
};

template<typename T>
void printArray(std::vector<T> array, int size);
void printMatrix(std::vector<std::vector<int>> mat, int height, int width);

std::string calculateCharacterValues(cv::Mat wordImage);
std::vector<int> calculareFrecventa(cv::Mat img);
int similarityIndex(std::vector<int> sectionValues1, std::vector<int> sectionValues2);
char getCharacterBySectionValues(std::vector<int> values, int size);
cv::Mat resizeTo(cv::Mat img, uint width, uint height);

void aplicareThreshold(cv::Mat img, uint8_t threshold);
int automaticThreshold(cv::Mat img);
std::vector<int> blackPixelsOnEachRow(cv::Mat img);
std::vector<int> blackPixelsOnEachColumn(cv::Mat img);
int findMostFreqBlackFromHist(std::vector<int> arr, int size);

std::vector<std::vector<int>> generateBoxesForText(cv::Mat img, int pixelsBetweenBoxes);
std::vector<int> blackPixelsOnEachColumnWithBorderedRows(cv::Mat img, int y0, int y1, int width);
std::vector<std::vector<int>> heightCoordsOfEachTextFoundOnRows(std::vector<int> heightFrec, int size, int verificari);
std::vector<std::vector<int>> widthCoordsOfEachTextFoundOnRows(std::vector<int> heightFrec, int size, int verificari);

void drawReactagles(cv::Mat img, std::vector<std::vector<int>> rectangles, int nrOfRectangles);
cv::Mat RGB2GRAY(cv::Mat input);
void textDetector(cv::Mat original, cv::Mat output);
std::list<std::vector<int>> btnDetector(cv::Mat original, cv::Mat output);
std::map<int, std::map<std::string, std::vector<int>>> characterDetector(cv::Mat original);
std::list<std::vector<int>> checkboxDetector(cv::Mat original, cv::Mat output);
void generateHtmlFile(std::map<int, std::map<std::string, std::vector<int>>> text, std::list<std::vector<int>> btns, std::list<std::vector<int>> checkBoxes);

cv::Mat generateLegendCustom(int w, int h);
cv::Mat3b ataseazaLegenda(cv::Mat img_rez, int width_legenda);