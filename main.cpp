#include"Header.h"

int main() {
	Mat img, rez;

	img = imread("Images/test.png");
	cvtColor(img, rez, COLOR_RGB2GRAY);
	imwrite("Images/ceva.png", rez);
	int width = rez.size().width;
	int height = rez.size().height;

	filterNoise(rez, 50);
	int* rowFreq = pixelsBlackRows(rez);
	int* colFreq = pixelsBlackColumns(rez);

	int reactangleCoordsRow, reactangleCoordsCol;
	int** rows_text = defineRows(rowFreq, height, reactangleCoordsRow);
	printMatrix(rows_text, reactangleCoordsRow, 2);

	int** rectangles = generate_rectangles(rez,rows_text, reactangleCoordsRow,width, height);

	//int** column_text = defineRows(colFreq, width, reactangleCoordsCol);

	int** rectangles_ = generate_rectangles(rez, rows_text, reactangleCoordsRow, width, height);

	cvtColor(rez, img, COLOR_GRAY2RGB);

	drawReactagles(img, rectangles_, 420);

	imshow("img", img);

	waitKey(0);

	return 0;
}
