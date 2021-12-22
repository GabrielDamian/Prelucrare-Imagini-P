#include"Header.h"

int main() {
	cv::Mat img = cv::imread("Images/test1.png");
	cv::Mat rez = img.clone();
	//textDetector(img,rez);
	//btnDetector(img,rez);
	//checkboxDetector(img, rez);
	
	characterDetector(img,rez);
	
	//from unicodes
	//calculateCharacterValues(img);

	cv::Mat3b imagineFinala = ataseazaLegenda(rez, 280);

	cv::imshow("FINAL", imagineFinala);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
