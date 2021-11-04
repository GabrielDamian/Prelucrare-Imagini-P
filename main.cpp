#include"Header.h"

int main() {
	Mat img, imgGray;

	img = imread("Images/ceva1.png");
	imshow("Inainte", img);

	imgGray = RGB2GRAY(img);

	imshow("InainteGray", imgGray);
	int width = imgGray.size().width;
	int height = imgGray.size().height;

	filterNoise(imgGray, 30);
	int* rowFreq = pixelsBlackRows(imgGray);
	int* colFreq = pixelsBlackColumns(imgGray);

	imshow("Intermediar2", imgGray);

	int reactangleCoordsRow, reactangleCoordsCol;
	int** rows_text = defineRows(rowFreq, height, reactangleCoordsRow);

	int** rectangles = generate_rectangles(imgGray, rows_text, reactangleCoordsRow, width, height);

	int** rectangles_ = generate_rectangles(imgGray, rows_text, reactangleCoordsRow, width, height);

	drawReactagles(img, rectangles_, 420);

	imshow("Dupa", img);


	waitKey(0);

	return 0;
}
