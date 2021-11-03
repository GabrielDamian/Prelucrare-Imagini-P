#include"Header.h"

void printArray(int* array, int size) {
	cout << endl;
	for (int i = 0; i < size; ++i) {
		cout << array[i] << endl;
	}
	cout << endl;
}

void printMatrix(int** mat, int height, int width) {
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			cout << mat[i][j] << " ";
		}
		cout << endl;
	}
}

void filterNoise(Mat img, uint8_t threshold) {
	int width = img.size().width;
	int height = img.size().height;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
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

int* pixelsBlackRows(Mat img) {
	int width = img.size().width;
	int height = img.size().height;

	int* v = new int[height];
	for (int i = 0; i < height; i++) v[i] = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (img.at<uint8_t>(i, j) == 0) {
				v[i]++;
			}
		}
	}
	return v;
}


int* pixelsBlackColumns(Mat img) {
	int width = img.size().width;
	int height = img.size().height;

	int* v = new int[width];
	for (int i = 0; i < width; i++) v[i] = 0;


	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

			if (img.at<uint8_t>(j, i) == 0) {
				v[i]++;
			}
		}
	}
	return v;
}

int** defineRows(int* heightFrec, int size, int& matrix_size)
{
	//initalizare matrice auxiliara de rezultate
	int** matrix = new int* [size];
	for (int i = 0; i < size; ++i)
		matrix[i] = new int[2];
	int k_matrix = 0;

	int marime_matrix = 0; // la aflarea marimii reale a matricii rezultat

	//calculare valori
	for (int i = 0; i < size; i++)
	{
		if (heightFrec[i] != 0 )
		{
			int i_start = i;
			int l = 0;
			do {
				l++;
			} while (heightFrec[i + l] != 0 || heightFrec[i + l + 1] != 0 || heightFrec[i + l + 2] != 0 || heightFrec[i + l + 3] != 0 || heightFrec[i + l + 4] != 0 || heightFrec[i + l + 5] != 0 || heightFrec[i + l + 6] != 0 || heightFrec[i + l + 7] != 0  );

			int i_finish = i+l;
			i = i_finish;

			int *temp = new int[2]{ i_start,i_finish };
			matrix[k_matrix++] = temp;
			marime_matrix++;
		}
	}

	//init matrice finala de valori
	int** final_matrix = new int* [marime_matrix];
	for (int i = 0; i < marime_matrix; ++i)
		final_matrix[i] = new int[2];

	//copiere din matricea aux in cea finala
	for (int i = 0; i < marime_matrix; i++)
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
	for (int i = 0; i < nrOfRectangles; i++)
	{
		Rect r = Rect(rectangles[i][0], rectangles[i][1], rectangles[i][3], rectangles[1][2]);
		cv::rectangle(img, r, Scalar(255, 0, 0), 1);
	}
}


int** define_rectangles(int** width_pairs, int** height_pairs, int size_width_pairs, int size_height_pairs)
{
	int size_matrix = size_width_pairs * size_height_pairs;
	int exact_matrix_size = 0;
	int** matrix = new int* [size_matrix];

	for (int i = 0; i < size_matrix; ++i)
		matrix[i] = new int[4];

	for (int i = 0; i < size_width_pairs; i++)
	{
		for (int j = 0; j < size_height_pairs; j++)
		{
			matrix[exact_matrix_size][0] = height_pairs[j][0];
			matrix[exact_matrix_size][1] = width_pairs[i][0];
			matrix[exact_matrix_size][2] = width_pairs[i][1] - width_pairs[i][0];
			matrix[exact_matrix_size++][3] = height_pairs[j][1] - height_pairs[j][0];
		}
	}

	return matrix;
}

int* frecvWithBorder(Mat img, int y0, int y1,int width)
{
	int* frecv = new int[width];
	for (int z = 0; z < width; z++)
	{
		frecv[z] = 0;
	}

	for (int i = 0; i < width; i++)
	{
		for (int j = y0; j < y1; j++)
		{
			if (img.at<uint8_t>(j, i) == 0)
			{
				frecv[i]++;
			}
		}
	}
	return frecv;
}
int** generate_rectangles(Mat img, int** frecv, int size, int width, int height)
{
	//matrice rezultat (x,y,width, height);
	int** matrix = new int* [width * height];
	int k = 0;
	for (int i = 0; i < width*height; ++i)
		matrix[i] = new int[4];



	//parcurs vector mov

	for (int i = 0; i < size; i++)
	{
		int y0 = frecv[i][0];
		int y1 = frecv[i][1];
		cout <<"y1-y0"<< y1 - y0 << endl;

		int* frecvBorder = frecvWithBorder(img, y0, y1, width);
		int exact_size_width_intervals;
		int** intervale_width = defineRows(frecvBorder, width, exact_size_width_intervals);
		
		for (int j = 0; j < exact_size_width_intervals; j++)
		{
			int x0 = intervale_width[j][0];
			int x1 = intervale_width[j][1];

			matrix[k][0] = x0;
			matrix[k][1] = y0;
			matrix[k][2] = y1 - y0;
			matrix[k][3] = x1 - x0;
			k++;
		}



	}
		//pt fiecare elem, aflat vector frec la dreapta

		//calcul perechi vector frec la dreapta

		//adaugare in matrice rectangles
		
	return matrix;

}