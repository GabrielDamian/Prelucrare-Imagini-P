#include"Header.h"

int main() {
	Mat img = imread("Images/test4.png");
	Mat imgText = textDetector(img);

	imshow("Inainte", img);
	imshow("Dupa", imgText);
	
	waitKey(0);
	destroyAllWindows();

	return 0;
}
