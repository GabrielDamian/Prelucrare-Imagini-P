#include"Header.h"

int main() {
	Mat img = imread("Images/test1.png");
	Mat rez = img.clone();
	textDetector(img,rez);
	btnDetector(img,rez);

	/*
	* CEVA
	* altceva
	//imshow("Inainte", img);
	//imshow("Detectare text", rez);
	Mat legenda = generateLegendCustom(rez.rows,280);
	
	//imagine finala (compusa din: rez + legenda)
	Mat3b final(rez.rows, rez.cols+legenda.cols, Vec3b(0, 0, 0));
	
	rez.copyTo(final(Rect(0, 0, rez.cols, rez.rows)));
	legenda.copyTo(final(Rect(rez.cols,0, legenda.cols, legenda.rows)));
	*/
	Mat3b imagine_finala = ataseazaLegenda(rez, 280);

	imshow("FINAL", imagine_finala);
	waitKey(0);
	destroyAllWindows();

	return 0;
}
