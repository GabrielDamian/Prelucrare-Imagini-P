#include"Header.h"

int main() {
	Mat img = imread("Images/0.jpg");
	Mat rez = img.clone();
	//textDetector(img,rez);
	//btnDetector(img,rez);
	
	characterDetector(img,rez);
	
	//from unicodes
	//calculateCharacterValues(img);

	//Mat3b imagineFinala = ataseazaLegenda(rez, 280);

	//imshow("FINAL", imagineFinala);
	waitKey(0);
	destroyAllWindows();
}
