#include"Header.h"

int main() {
	Mat img = imread("Images/test1.png");
	Mat rez = img.clone();
	textDetector(img,rez);
	btnDetector(img,rez);

	Mat3b imagine_finala = ataseazaLegenda(rez, 280);

	imshow("FINAL", imagine_finala);
	waitKey(0);
	destroyAllWindows();

	return 0;
}
