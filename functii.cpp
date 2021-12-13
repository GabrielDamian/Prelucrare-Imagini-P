#include"Header.h"

template <typename T>
void printArray(T* array, int size) {
	FILE* f;
	fopen_s(&f, "coduri_caractere.txt", "a");
	fprintf(f,"Array : ");
	for (int i = 0; i < size; ++i) {
		fprintf(f,"%d ",array[i]);
	}
	fprintf(f,"\n");
	fclose(f);
}

void printMatrix(int** mat, int height, int width) {
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			cout << mat[i][j] << " ";
		}
		cout << endl;
	}
}

// TEXT


int findMostFreqBlackFromHist(int* arr, int size) {
	//Merg pana la size/3 pentru ca in acel range sunt culorile de negru
	int val = arr[0];
	int pos = 0;
	for (int i = 1; i < size/3; ++i) {
		if (arr[i] > val) {
			val = arr[i];
			pos = i;
		}
	}
	return pos;
}

Mat RGB2GRAY(Mat input) {
	int width = input.size().width;
	int height = input.size().height;
	Mat rez(height, width, CV_8UC1, Scalar(0,0,0));

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			Vec3b pixelRGB = input.at<Vec3b>(i, j);
			rez.at<uint8_t>(i, j) =  0.299 * pixelRGB[0] + 0.587 * pixelRGB[1] + 0.114 * pixelRGB[2];
		}
	}
	return rez;
}

int* calculareFrecventa(Mat img) {
	int width = img.size().width;
	int height = img.size().height;
	int* result = new int[255];
	for (int i = 0; i < 255; ++i) result[i] = 0;

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			result[img.at<uint8_t>(i,j)] ++;

	return result;
}

void aplicareThreshold(Mat img, uint8_t threshold) {
	int width = img.size().width;
	int height = img.size().height;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			uint8_t* pixel = &(img.at<uint8_t>(i, j));
			if (*pixel > threshold) {
				*pixel = 255;
			}
			else {
				*pixel = 0;
			}
		}
	}
}

int* blackPixelsOnEachRow(Mat img) {
	int width = img.size().width;
	int height = img.size().height;
	int* v = new int[height];
	for (int i = 0; i < height; ++i) v[i] = 0;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (img.at<uint8_t>(i, j) == 0) {
				v[i]++;
			}
		}
	}
	return v;
}

int* blackPixelsOnEachColumn(Mat img) {
	int width = img.size().width;
	int height = img.size().height;
	int* v = new int[width];
	for (int i = 0; i < width; ++i) v[i] = 0;

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (img.at<uint8_t>(j, i) == 0) {
				v[i]++;
			}
		}
	}
	return v;
}

int automaticThreshold(Mat img) {
	int* histogram = calculareFrecventa(img);
	return findMostFreqBlackFromHist(histogram,255);
}

int** heightCoordsOfEachTextFoundOnRows(int* heightFrec, int size, int& matrixSize)
{
	//initalizare matrice auxiliara de rezultate
	int** matrix = new int* [size];
	for (int i = 0; i < size; ++i)
		matrix[i] = new int[2];
	int k_matrix = 0;

	int marime_matrix = 0; // la aflarea marimii reale a matricii rezultat

	//calculare valori
	for (int i = 0; i < size; ++i)
	{
		if (heightFrec[i] != 0)
		{
			int i_start = i;
			int component_height = 0;
			do {
				component_height++;
			} while (heightFrec[i + component_height] != 0 || heightFrec[i + component_height + 1] != 0 || heightFrec[i + component_height + 2] != 0);

			int i_finish = i + component_height;
			i = i_finish;

			int* temp = new int[2]{ i_start,i_finish };
			matrix[k_matrix++] = temp;
			marime_matrix++;
		}
	}

	//init matrice finala de valori
	int** finalMatrix = new int* [marime_matrix];
	for (int i = 0; i < marime_matrix; ++i)
		finalMatrix[i] = new int[2];

	//copiere din matricea aux in cea finala
	for (int i = 0; i < marime_matrix; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			finalMatrix[i][j] = matrix[i][j];
		}
	}
	matrixSize = marime_matrix;

	//dealocare matrice aux
	for (int i = 0; i < size; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;

	return finalMatrix;
}

int** widthCoordsOfEachTextFoundOnRows(int* heightFrec, int size, int& matrix_size, int verificari = 6)
{
	//initalizare matrice auxiliara de rezultate
	int** matrix = new int* [size];
	for (int i = 0; i < size; ++i)
		matrix[i] = new int[2];
	int k_matrix = 0;

	int marime_matrix = 0; // la aflarea marimii reale a matricii rezultat

	for (int i = 0; i < size; ++i)
	{
		if (heightFrec[i] != 0)
		{
			int i_start = i;
			int l = 0;
			bool verificare = 0;
			do {
				l++;
				verificare = 0;
				
				for (int j = 0; j < verificari; ++j) {
					verificare = verificare || heightFrec[i + l + j]; // daca macar una e adv e bine
				}
				
			} while (verificare);

			int i_finish = i + l;
			i = i_finish;

			int* temp = new int[2]{ i_start,i_finish };
			matrix[k_matrix++] = temp;
			marime_matrix++;
		}
	}

	//init matrice finala de valori
	int** final_matrix = new int* [marime_matrix];
	for (int i = 0; i < marime_matrix; ++i)
		final_matrix[i] = new int[2];

	//copiere din matricea aux in cea finala
	for (int i = 0; i < marime_matrix; ++i)
	{
		for (int j = 0; j < 2; j++)
		{
			final_matrix[i][j] = matrix[i][j];
		}
	}
	matrix_size = marime_matrix;

	//dealocare matrice aux
	for (int i = 0; i < size; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;

	return final_matrix;
}

void drawReactagles(Mat img, int** rectangles, int nrOfRectangles) {
	for (int i = 0; i < nrOfRectangles; ++i)
	{
		//Rect r = Rect(rectangles[i][0]-2, rectangles[i][1]-2, rectangles[i][3]+4, rectangles[1][2]+4);
		Rect r = Rect(rectangles[i][0], rectangles[i][1], rectangles[i][3], rectangles[1][2]);
		cv::rectangle(img, r, Scalar(0, 0, 255), 1);
	}
}

int* blackPixelsOnEachColumnWithBorderedRows(Mat img, int y0, int y1, int width)
{
	int* frecv = new int[width];
	for (int i = 0; i < width; ++i)
	{
		frecv[i] = 0;
	}

	for (int i = 0; i < width; ++i)
	{
		for (int j = y0; j < y1; ++j)
		{
			if (img.at<uint8_t>(j, i) == 0)
			{
				frecv[i]++;
			}
		}
	}
	return frecv;
}

int** generateBoxesForText(Mat img, int &OutputNrOfRectagles, int pixelsBetweenBoxes = 6)
{
	int width = img.size().width;
	int height = img.size().height;

	//generare coord y pt text
	int* rowFreq = blackPixelsOnEachRow(img);
	int nrOfComponentsOnRows;
	int** frecv = heightCoordsOfEachTextFoundOnRows(rowFreq, height, nrOfComponentsOnRows);

	//matrice rezultat (x,y,width, height);
	int** matrix = new int* [width * height];
	int k = 0;
	for (int i = 0; i < width * height; ++i)
		matrix[i] = new int[4];

	//parcurs vector mov
	int nrOfRectangles=0;
	for (int i = 0; i < nrOfComponentsOnRows; ++i)
	{
		int y0 = frecv[i][0];
		int y1 = frecv[i][1];

		int* frecvBorder = blackPixelsOnEachColumnWithBorderedRows(img, y0, y1, width);
		int exact_size_width_intervals;
		int** intervale_width = widthCoordsOfEachTextFoundOnRows(frecvBorder, width, exact_size_width_intervals, pixelsBetweenBoxes);
		nrOfRectangles += exact_size_width_intervals;
		for (int j = 0; j < exact_size_width_intervals; ++j)
		{
			int x0 = intervale_width[j][0];
			int x1 = intervale_width[j][1];

			matrix[k][0] = x0; 
			matrix[k][1] = y0; // coord punctului din stanga sus (x0,y0)
			matrix[k][2] = y1 - y0; //latime
			matrix[k][3] = x1 - x0; //lungime
			k++;
		}
	}
	OutputNrOfRectagles = nrOfRectangles;
	return matrix;
}

void characterDetector(Mat original, Mat output) {
	Mat img = original.clone();
	Mat imgGray = RGB2GRAY(img);

	aplicareThreshold(imgGray, automaticThreshold(imgGray));
	int nrOfReactangles;
	int** words = generateBoxesForText(imgGray, nrOfReactangles, 7);
	
	for (int wordIndex = 0; wordIndex < nrOfReactangles; ++wordIndex) {
		//segmentarea cuvantului
		int x = words[wordIndex][0];
		int y = words[wordIndex][1];
		int w = words[wordIndex][3];
		int h = words[wordIndex][2];
		Mat wordImage = img(Rect(x, y, w, h));

		string concat = "Cropped/" + to_string(wordIndex) + ".jpg"; // sa il faci "Cropped/aux.jpg" in stadiu final
		imwrite(concat, wordImage);
		wordImage = imread(concat);

		copyMakeBorder(wordImage, wordImage, 5, 5, 5, 5, BORDER_CONSTANT, Scalar(255,255,255));
		Mat wordImageBinary;
		threshold(RGB2GRAY(wordImage), wordImageBinary, 0, 255, THRESH_OTSU);

		int nrOfCharacters;
		int** characters = generateBoxesForText(wordImageBinary, nrOfCharacters, 1);

		//segmenarea literei
		for (int characterIndex = 0; characterIndex < nrOfCharacters; ++characterIndex) {
			int xCharacter = characters[characterIndex][0];
			int yCharacter = characters[characterIndex][1];
			int wCharacter = characters[characterIndex][3];
			int hCharacter = characters[characterIndex][2];
			int xVerical = xCharacter + ceil((double)wCharacter / 2);
			int y0Horizontal = yCharacter + hCharacter / 4;
			int y1Horizontal = yCharacter + hCharacter / 2;
			int y2Horizontal = yCharacter + hCharacter * (3/4);

			uint* sectionValues = new uint[8]{ 0,0,0,0,0,0,0,0 };
			/* parcugerea celor 8 zone din fiecare caracter
			     0    |		1
			----------|---------- y0Horizontal
				 2    |		3
			----------|---------- y1Horizontal
				 4	  |		5
			----------|---------- y2Horizontal
				 6	  |		7
				 xVeritical	
			*/
			for (int rowIndexPixel = yCharacter; rowIndexPixel < yCharacter + hCharacter - 1; ++rowIndexPixel) {
				for (int columnIndexPixel = xCharacter; columnIndexPixel < xCharacter + wCharacter - 1; ++columnIndexPixel) {
					uint8_t pixel = wordImageBinary.at<uint8_t>(rowIndexPixel, columnIndexPixel);
					if (pixel == 0) {
						if (columnIndexPixel <= xVerical && rowIndexPixel < y0Horizontal) {  // 0
							sectionValues[0]++;
						}
						else if (columnIndexPixel > xVerical && rowIndexPixel < y0Horizontal) {  // 1
							sectionValues[1]++;
						}
						else if (columnIndexPixel <= xVerical && rowIndexPixel >= y0Horizontal && rowIndexPixel < y1Horizontal) {  // 2
							sectionValues[2]++;
						}
						else if (columnIndexPixel > xVerical && rowIndexPixel >= y0Horizontal && rowIndexPixel < y1Horizontal) {  // 3
							sectionValues[3]++;
						}
						else if (columnIndexPixel <= xVerical && rowIndexPixel >= y1Horizontal && rowIndexPixel < y2Horizontal) {  // 4
							sectionValues[4]++;
						}
						else if (columnIndexPixel > xVerical && rowIndexPixel >= y1Horizontal && rowIndexPixel < y2Horizontal) {  // 5
							sectionValues[5]++;
						}
						else if (columnIndexPixel <= xVerical && rowIndexPixel >= y2Horizontal) {  // 6
							sectionValues[6]++;
						}
						else if (columnIndexPixel > xVerical && rowIndexPixel >= y2Horizontal) {
							sectionValues[7]++;
						}
					}
				}
			}

			printArray(sectionValues, 8);
			printf("\nx=%d y=%d w=%d h=%d", xCharacter, yCharacter, wCharacter, hCharacter);
			printf("\nv=%d o1=%d o2=%d o3=%d\n", xVerical, y0Horizontal, y1Horizontal, y2Horizontal);
			imshow("Char", wordImage(Rect(xCharacter, yCharacter, wCharacter, hCharacter)));
			waitKey(0);
		}
	}
}

void calculateCharacterValues(Mat img) {
	Mat imgGray = RGB2GRAY(img);

	threshold(imgGray, imgGray, 0, 255, THRESH_OTSU);
	int nrOfCharacters;
	int** characters = generateBoxesForText(imgGray, nrOfCharacters, 7);

	drawReactagles(img, characters, nrOfCharacters);
	cv::imshow("all", img);

	//segmenarea literei
	for (int characterIndex = 0; characterIndex < nrOfCharacters; ++characterIndex) {
		int xCharacter = characters[characterIndex][0];
		int yCharacter = characters[characterIndex][1];
		int wCharacter = characters[characterIndex][3];
		int hCharacter = characters[characterIndex][2];
		int xVerical = xCharacter + ceil((double)wCharacter / 2);
		int y0Horizontal = yCharacter + hCharacter / 4;
		int y1Horizontal = yCharacter + hCharacter / 2;
		int y2Horizontal = yCharacter + hCharacter * (3 / 4);

		uint* sectionValues = new uint[8]{ 0,0,0,0,0,0,0,0 };
		/* parcugerea celor 8 zone din fiecare caracter
			 0    |		1
		----------|---------- y0Horizontal
			 2    |		3
		----------|---------- y1Horizontal
			 4	  |		5
		----------|---------- y2Horizontal
			 6	  |		7
			 xVeritical
		*/
		for (int rowIndexPixel = yCharacter; rowIndexPixel < yCharacter + hCharacter; ++rowIndexPixel) {
			for (int columnIndexPixel = xCharacter; columnIndexPixel < xCharacter + wCharacter; ++columnIndexPixel) {
				uint8_t pixel = imgGray.at<uint8_t>(rowIndexPixel, columnIndexPixel);
				if (pixel == 0) {
					if (columnIndexPixel <= xVerical && rowIndexPixel < y0Horizontal) {  // 0
						sectionValues[0]++;
					}
					else if (columnIndexPixel > xVerical && rowIndexPixel < y0Horizontal) {  // 1
						sectionValues[1]++;
					}
					else if (columnIndexPixel <= xVerical && rowIndexPixel >= y0Horizontal && rowIndexPixel < y1Horizontal) {  // 2
						sectionValues[2]++;
					}
					else if (columnIndexPixel > xVerical && rowIndexPixel >= y0Horizontal && rowIndexPixel < y1Horizontal) {  // 3
						sectionValues[3]++;
					}
					else if (columnIndexPixel <= xVerical && rowIndexPixel >= y1Horizontal && rowIndexPixel < y2Horizontal) {  // 4
						sectionValues[4]++;
					}
					else if (columnIndexPixel > xVerical && rowIndexPixel >= y1Horizontal && rowIndexPixel < y2Horizontal) {  // 5
						sectionValues[5]++;
					}
					else if (columnIndexPixel <= xVerical && rowIndexPixel >= y2Horizontal) {  // 6
						sectionValues[6]++;
					}
					else if (columnIndexPixel > xVerical && rowIndexPixel >= y2Horizontal) {
						sectionValues[7]++;
					}
				}
			}
		}

		printArray(sectionValues, 8);
		printf("\nx=%d y=%d w=%d h=%d", xCharacter, yCharacter, wCharacter, hCharacter);
		printf("\nv=%d o1=%d o2=%d o3=%d\n", xVerical, y0Horizontal, y1Horizontal, y2Horizontal);
		string nume = "char" + to_string(characterIndex);
		imshow(nume, imgGray(Rect(xCharacter, yCharacter, wCharacter, hCharacter)));
		//waitKey(0);
		//destroyAllWindows();
	}
}

void textDetector(Mat original, Mat output) {

	Mat img=original.clone();
	Mat imgGray = RGB2GRAY(img);

	//threshold(imgGray, imgGray, 0, 255, THRESH_BINARY + THRESH_OTSU);
	aplicareThreshold(imgGray, automaticThreshold(imgGray));
	imshow("Intermediar", imgGray);

	int nrOfReactangles;
	int** rectangles_ = generateBoxesForText(imgGray,nrOfReactangles);
	drawReactagles(output, rectangles_, nrOfReactangles);
}

void btnDetector(Mat original, Mat output) {
	Mat img = original.clone(), imgBlur, imgCanny, imgDil;

	//preprocesare
	GaussianBlur(img, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	dilate(imgCanny, imgDil, getStructuringElement(MORPH_RECT, Size(3, 3)));

	//procesare
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(imgDil, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); //gaseste contururi

	for (int i = 0; i < contours.size(); ++i) {
		vector<vector<Point>> poligoane(contours.size());
		approxPolyDP(contours[i], poligoane[i], 0.02 * arcLength(contours[i], true), true); //generare poligoane

		if (!(contourArea(contours[i], true) > 500 && poligoane[i].size() == 4)) continue; //eliminare trash

		//coordonate puncte si lungime laturi
		Point ss = Point(poligoane[i][0].x, poligoane[i][0].y);
		Point ds = Point(poligoane[i][1].x, poligoane[i][1].y);
		Point dj = Point(poligoane[i][2].x, poligoane[i][2].y);
		Point sj = Point(poligoane[i][3].x, poligoane[i][3].y);
		int lungimeStanga = sj.y - ss.y;
		int lungimeSus = ds.x - ss.x;
		int lungimeDreapta = dj.y - ds.y;
		int lungimeJos = dj.y - ds.y;
		//verif conditie dreptunghi (laturi egale + paralele)
		//laturi aprox egale
		int diferentaLatime = abs(lungimeStanga - lungimeDreapta);
		int diferentaLungime = abs(lungimeSus - lungimeJos);

		if (diferentaLatime > lungimeDreapta * 0.1 || diferentaLatime > lungimeStanga * 0.1) // diferenta prea mare de latime
			if (diferentaLungime > lungimeSus * 0.1 || diferentaLungime > lungimeJos * 0.1) continue; //diferenta prea mare de lungime

		//laturi aprox paralele
		
		int inclinareaStangii = abs(ss.x - sj.y);
		int inclinareaDreptei = abs(ds.x - dj.y);
		int inclinareaSus = abs(ss.y - ds.y);
		int inclinareaJos = abs(sj.y - dj.y);
		int trasholdVertical = lungimeDreapta * 0.05;
		int trasholdOrizontal = lungimeSus * 0.05;
			if (inclinareaDreptei > trasholdVertical || inclinareaStangii > trasholdVertical)//verificare inclinare pe parti
				if (inclinareaSus > trasholdOrizontal || inclinareaJos > trasholdOrizontal)//verificare inclinare sus si jos
					continue;
		drawContours(output, poligoane, i, Scalar(0, 255, 0), 2);
	}
}

Mat generateLegendCustom(int w, int h)
{
	Mat img(w, h, CV_8UC3, Scalar(255, 255,255));
	if (img.empty()) {
		cout << "Could not load image" << endl;
	}

	Rect r = Rect(180,20,50,30);
	rectangle(img, r, Scalar(0, 0, 255), 2);
	putText(img, "Text", cv::Point(50, 43), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 1, false);

	Rect r_2 = Rect(180, 80, 50, 30);
	rectangle(img, r_2, Scalar(66, 245, 66), 2);
	putText(img, "Buton", cv::Point(50, 105), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 1, false);

	return img;
}


Mat3b ataseazaLegenda(Mat rez, int width_legenda)
{
	//width_leganda = 280
	Mat legenda = generateLegendCustom(rez.rows, width_legenda);
	Mat3b final(rez.rows, rez.cols + legenda.cols, Vec3b(0, 0, 0));

	rez.copyTo(final(Rect(0, 0, rez.cols, rez.rows)));
	legenda.copyTo(final(Rect(rez.cols, 0, legenda.cols, legenda.rows)));
	
	return final;
}	