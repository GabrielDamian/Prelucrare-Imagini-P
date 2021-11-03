#include"Header.h"

int main() {
	Mat img, rez;

	img = imread("Images/test.png");
	cvtColor(img, rez, COLOR_RGB2GRAY);

	int width = rez.size().width;
	int height = rez.size().height;

	filterNoise(rez, 50);
	int* rowFreq = pixelsBlackRows(rez);
	int* colFreq = pixelsBlackColumns(rez);

	int reactangleCoordsRow, reactangleCoordsCol;
	int** rows_text = defineRows(rowFreq, height, reactangleCoordsRow);
	int** column_text = defineRows(colFreq, width, reactangleCoordsCol);

	int** rectangles_ = define_rectangles(rows_text, column_text, reactangleCoordsRow, reactangleCoordsCol);

	cvtColor(rez, img, COLOR_GRAY2RGB);

	drawReactagles(img, rectangles_, 40);

	imshow("img", img);

	waitKey(0);

	return 0;
}
