#include"Header.h"

int main() {
	cv::Mat img = cv::imread("Images/test1.png");
	cv::Mat rez = img.clone();
	textDetector(img,rez);
	auto btns = btnDetector(img,rez);
	auto checkboxes = checkboxDetector(img, rez);
	cv::Mat3b imagineFinala = ataseazaLegenda(rez, 280);
	cv::imshow("FINAL", imagineFinala);
	
	auto text = characterDetector(img);
	
	generateHtmlFile(text, btns, checkboxes);

	cv::waitKey(0);
	cv::destroyAllWindows();
}
