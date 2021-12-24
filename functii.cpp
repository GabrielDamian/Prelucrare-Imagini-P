#include"Header.h"
#include "Caractere.h"

void printInFile(std::vector<int> array, int size) {
	FILE* f;
	fopen_s(&f, "input.txt", "a");
	fprintf(f,"Array : ");
	for (int i = 0; i < size; ++i) {
		fprintf(f,"%d ", array[i]);
	}
	fprintf(f,"\n");
	fclose(f);
}

template <typename T>
void printArray(std::vector<T> array, int size) {
	printf("Array : ");
	for (int i = 0; i < size; ++i) {
		std::cout << array[i] << " ";
	}
	printf("\n");
}

void printMatrix(std::vector<std::vector<int>> mat, int height, int width) {
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			std::cout << mat[i][j] << " ";
		}
		std::cout << endl;
	}
}

cv::Mat resizeTo(cv::Mat img, uint width, uint height) {
	cv::Mat rez;
	resize(img, rez, cv::Size(width, height), cv::INTER_CUBIC);
	return rez;
}

cv::Mat eliminatePadding(cv::Mat img) {
	cv::Mat output(img);

	std::transform(output.begin<uint8_t>(), output.end<uint8_t>(), output.begin<uint8_t>(), [](uint8_t p) {return (uint8_t)(255 - p); });
	cv::Mat nonZeroCoords;
	findNonZero(output, nonZeroCoords);
	cv::Rect boundsWithNonZeroElems = boundingRect(nonZeroCoords);

	if (boundsWithNonZeroElems.width && boundsWithNonZeroElems.height) {
		output = img(boundsWithNonZeroElems).clone();
	}

	std::transform(output.begin<uint8_t>(), output.end<uint8_t>(), output.begin<uint8_t>(), [](uint8_t p) {return (uint8_t)(255 - p); });
	
	return output;
}

int similarityIndex(std::vector<int> sectionValues1,std::vector<int> sectionValues2) {
	int sum = 0;
	for (int i = 0; i < ROWS*COLS; ++i) {
		sum += abs(sectionValues1[i] - sectionValues2[i]) ;
	}
	return sum;
}

char getCharacterBySectionValues(std::vector<int> values, int size=NROFCHARACTERS) {

	char resultedCharacter = characters[0].ch;
	int min = similarityIndex(values,characters[0].fr);
	for (int i = 1; i < size; ++i) {
		int similarity = similarityIndex(values, characters[i].fr);
		if (similarity < min) {
			min = similarity;
			resultedCharacter = characters[i].ch;
		}
	}
	return resultedCharacter;
}

int findMostFreqBlackFromHist(std::vector<int> arr, int size) {
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

cv::Mat RGB2GRAY(cv::Mat input) {
	int width = input.size().width;
	int height = input.size().height;
	cv::Mat rez(height, width, CV_8UC1, cv::Scalar(0,0,0));

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			cv::Vec3b pixelRGB = input.at<cv::Vec3b>(i, j);
			rez.at<uint8_t>(i, j) =  0.299 * pixelRGB[0] + 0.587 * pixelRGB[1] + 0.114 * pixelRGB[2];
		}
	}
	return rez;
}

std::vector<int> calculareFrecventa(cv::Mat img) {
	std::vector<int> result(256, 0);

	for (int i = 0; i < img.rows; ++i) {
		for (int j = 0; j < img.cols; ++j) {
			auto pixel = img.at<uint8_t>(i, j);
			result[pixel] ++;
		}
	}

	return result;
}

void aplicareThreshold(cv::Mat img, uint8_t threshold) {
	std::transform(img.begin<uint8_t>(), img.end<uint8_t>(), img.begin<uint8_t>(),
		[threshold](uint8_t px) {return (uint8_t)(px > threshold ? 255 : 0); });
}

std::vector<int> blackPixelsOnEachRow(cv::Mat img) {
	int width = img.cols;
	int height = img.rows;
	std::vector<int> v(height, 0);
	
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (img.at<uint8_t>(i, j) == 0) {
				v[i]++;
			}
		}
	}
	return v;
}

std::vector<int> blackPixelsOnEachColumn(cv::Mat img) {
	int width = img.size().width;
	int height = img.size().height;
	std::vector<int> v(width, 0);

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (img.at<uint8_t>(j, i) == 0) {
				v[i]++;
			}
		}
	}
	return v;
}

int automaticThreshold(cv::Mat img) {
	return findMostFreqBlackFromHist(calculareFrecventa(img),255);
}

std::vector<std::vector<int>> heightCoordsOfEachTextFoundOnRows(std::vector<int> heightFrec, int size, int verificari = 6)
{
	//initalizare matrice auxiliara de rezultate
	std::vector<std::vector<int>> matrix(size, std::vector<int>(2,0));
	int k_matrix = 0;
	int marime_matrix = 0; // la aflarea marimii reale a matricii rezultat

	//calculare valori
	for (int i = 0; i < size; ++i)
	{
		if (heightFrec[i] != 0)
		{
			int i_start = i;
			int component_height = 0;
			bool verificare = 0;
			do {
				component_height++;
				verificare = 0;
				for (int j = 0; j < verificari; ++j) {
					verificare = verificare || heightFrec[i + component_height + j] != 0; // daca macar una e adv e bine
				}
			} while (verificare);

			int i_finish = i + component_height;
			i = i_finish;

			std::vector<int> temp{ i_start,i_finish };
			matrix[k_matrix++] = temp;
			marime_matrix++;
		}
	}

	//init matrice finala de valori
	std::vector<std::vector<int>> finalMatrix(marime_matrix, std::vector<int>(2, 0));

	//copiere din matricea aux in cea finala
	for (int i = 0; i < marime_matrix; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			finalMatrix[i][j] = matrix[i][j];
		}
	}

	return finalMatrix;
}

std::vector<std::vector<int>> widthCoordsOfEachTextFoundOnRows(std::vector<int> heightFrec, int size, int verificari = 6)
{
	//initalizare matrice auxiliara de rezultate
	std::vector<std::vector<int>> matrix(size, std::vector<int>(2, 0));
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
					verificare = verificare || heightFrec[i + l + j] != 0; // daca macar una e adv e bine
				}
				
			} while (verificare);

			int i_finish = i + l;
			i = i_finish;

			std::vector<int> temp{ i_start,i_finish };
			matrix[k_matrix++] = temp;
			marime_matrix++;
		}
	}

	//init matrice finala de valori
	std::vector<std::vector<int>> finalMatrix(marime_matrix, std::vector<int>(2, 0));

	//copiere din matricea aux in cea finala
	for (int i = 0; i < marime_matrix; ++i)
	{
		for (int j = 0; j < 2; j++)
		{
			finalMatrix[i][j] = matrix[i][j];
		}
	}

	return finalMatrix;
}

void drawReactagles(cv::Mat img, std::vector<std::vector<int>> rectangles, int nrOfRectangles) {
	for (int i = 0; i < nrOfRectangles; ++i)
	{
		cv::Rect r = cv::Rect(rectangles[i][0]-2, rectangles[i][1]-2, rectangles[i][3]+4, rectangles[1][2]+4);
		cv::rectangle(img, r, cv::Scalar(0, 0, 255), 1);
	}
}

std::vector<int> blackPixelsOnEachColumnWithBorderedRows(cv::Mat img, int y0, int y1, int width)
{
	std::vector<int> frecv(width, 0);

	for (int i = 0; i < width; ++i)
	{
		for (int j = y0; j < y1; ++j)
		{
			if ((int)img.at<uint8_t>(j, i) == 0)
			{
				frecv[i]++;
			}
		}
	}
	return frecv;
}

std::vector<std::vector<int>> generateBoxesForText(cv::Mat img, int pixelsBetweenBoxes = 6)
{
	int width = img.size().width;
	int height = img.size().height;

	//generare coord y pt text
	std::vector<int> rowFreq = blackPixelsOnEachRow(img);
	std::vector<std::vector<int>> frecv = heightCoordsOfEachTextFoundOnRows(rowFreq, height, pixelsBetweenBoxes);

	//matrice rezultat (x,y,width, height);
	std::vector<std::vector<int>> matrix(width*height/10, std::vector<int>(4, 0));
	int k = 0;

	//parcurs std::vector mov
	int nrOfRectangles=0;
	for (int i = 0; i < frecv.size(); ++i)
	{
		int y0 = frecv[i][0];
		int y1 = frecv[i][1];

		std::vector<int> frecvBorder = blackPixelsOnEachColumnWithBorderedRows(img, y0, y1, width);
		std::vector<std::vector<int>> intervale_width = widthCoordsOfEachTextFoundOnRows(frecvBorder, width, pixelsBetweenBoxes);
		nrOfRectangles += intervale_width.size();

		for (int j = 0; j < intervale_width.size(); ++j)
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
	matrix.resize(nrOfRectangles);
	return matrix;
}

std::map<int, std::pair<std::string,std::vector<int>>> characterDetector(cv::Mat original) {
	cv::Mat img = original.clone();
	cv::Mat imgGray = RGB2GRAY(img);

	aplicareThreshold(imgGray, automaticThreshold(imgGray));

	std::map<int, std::pair<std::string, std::vector<int>>> text;
	std::vector<std::vector<int>> words = generateBoxesForText(imgGray);

	for (int wordIndex = 0; wordIndex < words.size(); ++wordIndex) {
		//segmentarea cuvantului
		int x = (words[wordIndex][0]-2) < 0 ? words[wordIndex][0] : words[wordIndex][0] - 2;
		int y = (words[wordIndex][1] - 2) < 0 ? words[wordIndex][1] : words[wordIndex][1] - 2;
		int w = words[wordIndex][3]+2;
		int h = words[wordIndex][2]+2;

		cv::Mat wordImage = img(cv::Rect(x, y, w, h));
		string concat = "Cropped/" + to_string(wordIndex) + ".jpg"; // sa il faci "Cropped/aux.jpg" in stadiu final
		imwrite(concat, wordImage);
		wordImage = cv::imread(concat);

		std::string wordString = calculateCharacterValues(wordImage.clone());
		text.insert(
			std::make_pair(
				wordIndex,
				std::make_pair(wordString, vector<int>{ x, y, w, h })
			)
		);
	}
	return text;
}

std::string calculateCharacterValues(cv::Mat wordImage) { 
	std::string wordString = "";
	cv::Mat imgGray = RGB2GRAY(wordImage);
	threshold(imgGray, imgGray, 0, 255, cv::THRESH_OTSU);
	
	cv::Mat cuvantBordat;
	copyMakeBorder(imgGray, imgGray, 5, 5, 5, 5, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
	std::vector<std::vector<int>> characters = generateBoxesForText(imgGray, 1);

	//segmenarea literei
	for (int characterIndex = 0; characterIndex < characters.size(); ++characterIndex) {
		int xCharacter = (characters[characterIndex][0] - 1) == -1 ? characters[characterIndex][0]+1 : characters[characterIndex][0] - 1;
		int yCharacter = (characters[characterIndex][1] - 1) == -1 ? characters[characterIndex][1]+1 : characters[characterIndex][1] - 2;
		int wCharacter = characters[characterIndex][3]+2;
		int hCharacter = characters[characterIndex][2]+2;

		cv::Mat imgCharacter = imgGray(cv::Rect(xCharacter, yCharacter, wCharacter, hCharacter)).clone();
		imgCharacter = eliminatePadding(imgCharacter);
		cv::Mat imgResizedCharacter = resizeTo(imgCharacter, ROWSRESIZE, COLSRESIZE);
		cv::GaussianBlur(imgResizedCharacter, imgResizedCharacter, cv::Size(3, 3), 0);
		threshold(imgResizedCharacter, imgResizedCharacter, 0, 255, cv::THRESH_OTSU);

		int wRegion = ceil((double)imgResizedCharacter.cols / COLS);
		int hRegion = ceil((double)imgResizedCharacter.rows / ROWS);

		std::vector<int> sectionValues(ROWS * COLS, 0 );
		
		for (int rowIndexPixel = 0; rowIndexPixel < imgResizedCharacter.rows ; ++rowIndexPixel) {
			for (int columnIndexPixel = 0; columnIndexPixel < imgResizedCharacter.cols ; ++columnIndexPixel) {
				int pixel = (int)imgResizedCharacter.at<uint8_t>(rowIndexPixel, columnIndexPixel);
				if (pixel == 0) {
					int regionX = columnIndexPixel / wRegion;
					int regionY = rowIndexPixel / hRegion;
					int position = regionY * COLS + regionX;
					sectionValues[ position ] ++;
				}
			}
		}
		//printInFile(sectionValues,ROWS*COLS);
		wordString += getCharacterBySectionValues(sectionValues);
	}
	return wordString;
}

void textDetector(cv::Mat original, cv::Mat output) {

	cv::Mat img = original.clone();
	cv::Mat imgGray = RGB2GRAY(img);

	//threshold(imgGray, imgGray, 0, 255, THRESH_BINARY + THRESH_OTSU);
	aplicareThreshold(imgGray, automaticThreshold(imgGray));

	std::vector<std::vector<int>> rectangles_ = generateBoxesForText(imgGray);
	drawReactagles(output, rectangles_, rectangles_.size());
}

std::list<std::vector<int>> btnDetector(cv::Mat original, cv::Mat output) {
	cv::Mat img = original.clone(), imgBlur, imgCanny, imgDil;
	std::list<std::vector<int>> btnList;

	//preprocesare
	GaussianBlur(img, imgBlur, cv::Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	dilate(imgCanny, imgDil, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

	//procesare
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours(imgDil, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE); //gaseste contururi

	for (int i = 0; i < contours.size(); ++i) {
		std::vector<std::vector<cv::Point>> poligoane(contours.size());
		approxPolyDP(contours[i], poligoane[i], 0.02 * arcLength(contours[i], true), true); //generare poligoane

		if (!(contourArea(contours[i], true) > 500 && poligoane[i].size() == 4)) continue; //eliminare trash

		//coordonate puncte si lungime laturi
		cv::Point ss = cv::Point(poligoane[i][0].x, poligoane[i][0].y);
		cv::Point ds = cv::Point(poligoane[i][1].x, poligoane[i][1].y);
		cv::Point dj = cv::Point(poligoane[i][2].x, poligoane[i][2].y);
		cv::Point sj = cv::Point(poligoane[i][3].x, poligoane[i][3].y);
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
		double inclinareaStangii = abs(ss.x - sj.y);
		double inclinareaDreptei = abs(ds.x - dj.y);
		double inclinareaSus = abs(ss.y - ds.y);
		double inclinareaJos = abs(sj.y - dj.y);
		double trasholdVertical = lungimeDreapta * 0.05;
		double trasholdOrizontal = lungimeSus * 0.05;
			if (inclinareaDreptei > trasholdVertical || inclinareaStangii > trasholdVertical)//verificare inclinare pe parti
				if (inclinareaSus > trasholdOrizontal || inclinareaJos > trasholdOrizontal)//verificare inclinare sus si jos
					continue;

		if (lungimeSus > img.cols * 0.35 || lungimeStanga > img.rows * 0.2) continue;

		drawContours(output, poligoane, i, cv::Scalar(0, 255, 0), 2);
		btnList.push_back(std::vector<int>{ss.x, ss.y, lungimeSus, lungimeStanga}); //x,y,w,h
	}
	return btnList;
}

std::list<std::vector<int>> checkboxDetector(cv::Mat original, cv::Mat output) {
	cv::Mat img = original.clone(), imgCanny, imgDil;
	std::list<std::vector<int>> checkBoxList;

	Canny(img, imgCanny, 25, 75);
	dilate(imgCanny, imgDil, getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));

	//procesare
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours(imgCanny, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE); //gaseste contururi

	for (int i = 0; i < contours.size(); ++i) {
		std::vector<std::vector<cv::Point>> poligoane(contours.size());
		approxPolyDP(contours[i], poligoane[i], 0.02 * arcLength(contours[i], true), true); //generare poligoane

		if (!(contourArea(contours[i], true) > 100 && poligoane[i].size() == 4)) continue; //eliminare trash

		//coordonate puncte si lungime laturi
		cv::Point ss = cv::Point(poligoane[i][0].x, poligoane[i][0].y);
		cv::Point ds = cv::Point(poligoane[i][1].x, poligoane[i][1].y);
		cv::Point dj = cv::Point(poligoane[i][2].x, poligoane[i][2].y);
		cv::Point sj = cv::Point(poligoane[i][3].x, poligoane[i][3].y);
		int lungimeStanga = sj.y - ss.y;
		int lungimeSus = ds.x - ss.x;
		int lungimeDreapta = dj.y - ds.y;
		int lungimeJos = dj.y - ds.y;
		//verif conditie patrat (laturi egale + paralele)
		//laturi aprox egale
		int diferenteLungimi = abs(lungimeStanga - lungimeSus) + abs(lungimeStanga - lungimeDreapta) + abs(lungimeStanga - lungimeJos);
		if ( diferenteLungimi > 0.05) continue;

		//laturi aprox paralele

		int inclinareaStangii = abs(ss.x - sj.y);
		int inclinareaDreptei = abs(ds.x - dj.y);
		int inclinareaSus = abs(ss.y - ds.y);
		int inclinareaJos = abs(sj.y - dj.y);
		int trasholdVertical = lungimeDreapta * 0.09;
		int trasholdOrizontal = lungimeSus * 0.09;
		if (inclinareaDreptei > trasholdVertical || inclinareaStangii > trasholdVertical)//verificare inclinare pe parti
			if (inclinareaSus > trasholdOrizontal || inclinareaJos > trasholdOrizontal)//verificare inclinare sus si jos
				continue;

		drawContours(output, poligoane, i, cv::Scalar(0, 0, 0), 2);
		checkBoxList.push_back(std::vector<int>{ss.x, ss.y, lungimeSus, lungimeStanga}); //x,y,w,h
	}
	return checkBoxList;
}

cv::Mat generateLegendCustom(int w, int h)
{
	cv::Mat img(w, h, CV_8UC3, cv::Scalar(255, 255,255));
	if (img.empty()) {
		std::cout << "Could not load image" << endl;
	}

	cv::Rect r = cv::Rect(180,20,50,30);
	rectangle(img, r, cv::Scalar(0, 0, 255), 2);
	putText(img, "Text", cv::Point(50, 43), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 1, false);

	cv::Rect r_2 = cv::Rect(180, 80, 50, 30);
	rectangle(img, r_2, cv::Scalar(66, 245, 66), 2);
	putText(img, "Buton", cv::Point(50, 105), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 1, false);

	return img;
}

cv::Mat3b ataseazaLegenda(cv::Mat rez, int width_legenda)
{
	//width_leganda = 280
	cv::Mat legenda = generateLegendCustom(rez.rows, width_legenda);
	cv::Mat3b final(rez.rows, rez.cols + legenda.cols, cv::Vec3b(0, 0, 0));

	rez.copyTo(final(cv::Rect(0, 0, rez.cols, rez.rows)));
	legenda.copyTo(final(cv::Rect(rez.cols, 0, legenda.cols, legenda.rows)));
	
	return final;
}	

void generateHtmlFile(cv::Mat img, std::map<int, std::pair<std::string, std::vector<int>>> text, std::list<std::vector<int>> btns, std::list<std::vector<int>> checkBoxes) {
	
	string html = "<!DOCTYPE html>\n<html>\n<head>\n<link rel='stylesheet' href='style.css'>\n</head>\n<body>\n<div class='body-containter' style = 'width: ";
	html += to_string(img.cols);
	html += "px; height: ";
	html += to_string(img.rows);
	html += "px; '>\n";

	//TEXT LOGIC
	for (auto e : text) {	
		string xValue = to_string(e.second.second[1]);
		string yValue = to_string(e.second.second[0]);
		string widthValue = to_string(e.second.second[2]);
		string heightValue = to_string(e.second.second[3]);

		string newSpan = "<span style='top: " + xValue + "px; left:" + yValue + "px;'>" + e.second.first + "</span>";
		html += newSpan + "\n";
	}

	//BTN LOGIC
	for (auto e : btns)
	{
		int xValue = e[0];
		int yValue = e[1];
		int widthBtn = e[2];
		int heightBtn = e[3];

		//treci prin toate cuvintele si vezi care se regasete la x + offset, y+offset
		string btnInnerText = "default";
		for (auto a : text)
		{
			int xText = a.second.second[0];
			int yText = a.second.second[1];
			int widthCuvant = a.second.second[2];
			int heightCuvant = a.second.second[3];

			if (xText > xValue-10 && xText < xValue-10 + widthBtn &&
				yText > yValue-10 && yText < yValue-10 + heightBtn)
			{
				btnInnerText = a.second.first;
			}
		}
		string newButton = "<button style='top:" + to_string(yValue) + "px; left:" + to_string(xValue) + "px;'>" + btnInnerText +"</button>\n";
		html += newButton;
	}

	//CHECKBOX LOGIC
	for (auto e : checkBoxes)
	{
		int xValue = e[0];
		int yValue = e[1];

		string newCheckBox = "<input type='checkbox' style='top: " + to_string(yValue) + "px; left:" + to_string(xValue) + "px;' />\n";
		html += newCheckBox;
	}
	html += "</div>\n</body>\n</html>\n";

	ofstream myFile("index.html");
	myFile << html;
	myFile.close();
}