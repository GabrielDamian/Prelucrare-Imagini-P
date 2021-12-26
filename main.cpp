#include"Header.h"

int main() {
	cv::Mat img = cv::imread("Images/test2.png");
	cv::Mat rez = img.clone();
	textDetector(img,rez);
	auto btns = btnDetector(img,rez);
	auto checkboxes = checkboxDetector(img, rez);
	auto text = characterDetector(img);
	
	generateHtmlFile(img, text, btns, checkboxes);

	cv::Mat3b imagineFinala = ataseazaLegenda(rez, 280);
	cv::imshow("FINAL", imagineFinala);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
