#include"Header.h"

int main() {
	Mat img = imread("Images/test1.png");
	Mat rez = img.clone();
	textDetector(img,rez);
	btnDetector(img,rez);

	imshow("Inainte", img);
	imshow("Detectare text", rez);
	
	waitKey(0);
	destroyAllWindows();

	return 0;
}
