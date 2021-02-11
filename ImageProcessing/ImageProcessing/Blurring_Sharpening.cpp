#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

String imgname = "Original Image";
String resultname = "Result Image";

// 이미지 데이터 읽기
Mat orgimg = imread("./img/flower.jpg", CV_LOAD_IMAGE_COLOR);
Mat resultimg = Mat(orgimg.rows, orgimg.cols, CV_8UC3);

void Blurring(int masksize) {
	int** mean_mask = new int*[masksize];

	cout << endl << "****Mask****" << endl << endl;

	for (int i = 0; i < masksize; i++) {
		mean_mask[i] = new int[masksize];
	}

	for (int i = 0; i < masksize; i++) {
		for (int j = 0; j < masksize; j++) {
			mean_mask[i][j] = 1;
			cout << mean_mask[i][j] << " ";
		}
		cout << "\n";
	}

	int B;
	int G;
	int R;

	for (int y = 0; y < orgimg.rows; y++)
	{
		for (int x = 0; x < orgimg.cols; x++)
		{
			B = 0;
			G = 0;
			R = 0;

			for (int i = -1 * masksize / 2; i <= masksize / 2; i++) {
				for (int j = -1 * masksize / 2; j <= masksize / 2; j++) {
					int b_y = y + i;
					int b_x = x + j;

					if (b_y < 0) b_y = 0;
					else if (b_y > orgimg.rows - 1) b_y = orgimg.rows - 1;

					if (b_x < 0) b_x = 0;
					else if (b_x > orgimg.cols - 1) b_x = orgimg.cols - 1;

					B += orgimg.at<Vec3b>(b_y, b_x)[0] * mean_mask[masksize / 2 + i][masksize / 2 + j];
					G += orgimg.at<Vec3b>(b_y, b_x)[1] * mean_mask[masksize / 2 + i][masksize / 2 + j];
					R += orgimg.at<Vec3b>(b_y, b_x)[2] * mean_mask[masksize / 2 + i][masksize / 2 + j];
				}
			}

			B /= (masksize * masksize);	if (B > 255) B = 255;	if (B < 0) B = 0;
			G /= (masksize * masksize);	if (G > 255) G = 255;	if (G < 0) G = 0;
			R /= (masksize * masksize);	if (R > 255) R = 255;	if (R < 0) R = 0;

			resultimg.at<Vec3b>(y, x)[0] = B;
			resultimg.at<Vec3b>(y, x)[1] = G;
			resultimg.at<Vec3b>(y, x)[2] = R;
		}
	}

	imshow(imgname, orgimg);
	imshow(resultname, resultimg);
	waitKey();
}

void Sharpening(int menu) {
	int laplacian_mask[3][3] = { 0 };
	int laplacian_mask2[3][3] = { {0,-1,0},
								 {-1,5,-1},
								 {0,-1,0} };
	int masksize = 3;
	int B;
	int G;
	int R;

	if (menu == 2) {
		cout << endl << "****Mask****" << endl << endl;

		for (int i = 0; i < masksize; i++) {
			for (int j = 0; j < masksize; j++) {
				//if (i == masksize / 2 && j == masksize / 2)	laplacian_mask[i][j] = masksize * masksize - 1;
				if (i == masksize / 2 && j == masksize / 2)	laplacian_mask[i][j] = masksize * masksize;
				else laplacian_mask[i][j] = -1;
				cout << laplacian_mask[i][j] << " ";
			}
			cout << "\n";
		}
	}

	if (menu == 3) {
		cout << endl << "****Mask****" << endl << endl;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				cout << laplacian_mask2[i][j] << " ";
			}
			cout << "\n";
		}
	}

	for (int y = masksize - 2; y < orgimg.rows - masksize / 2; y++)
	{
		for (int x = masksize - 2; x < orgimg.cols - masksize / 2; x++)
		{
			B = 0;
			G = 0;
			R = 0;

			for (int i = -1 * masksize / 2; i <= masksize / 2; i++) {
				for (int j = -1 * masksize / 2; j <= masksize / 2; j++) {
					int s_y = y + i;
					int s_x = x + j;
					if (menu == 2) {
						B += orgimg.at<Vec3b>(s_y, s_x)[0] * laplacian_mask[masksize / 2 + i][masksize / 2 + j];
						G += orgimg.at<Vec3b>(s_y, s_x)[1] * laplacian_mask[masksize / 2 + i][masksize / 2 + j];
						R += orgimg.at<Vec3b>(s_y, s_x)[2] * laplacian_mask[masksize / 2 + i][masksize / 2 + j];
					}
					if (menu == 3) {
						B += orgimg.at<Vec3b>(s_y, s_x)[0] * laplacian_mask2[masksize / 2 + i][masksize / 2 + j];
						G += orgimg.at<Vec3b>(s_y, s_x)[1] * laplacian_mask2[masksize / 2 + i][masksize / 2 + j];
						R += orgimg.at<Vec3b>(s_y, s_x)[2] * laplacian_mask2[masksize / 2 + i][masksize / 2 + j];
					}
				}
			}

			if (B > 255) B = 255;	if (B < 0) B = 0;
			if (G > 255) G = 255;	if (G < 0) G = 0;
			if (R > 255) R = 255;	if (R < 0) R = 0;

			resultimg.at<Vec3b>(y, x)[0] = B;
			resultimg.at<Vec3b>(y, x)[1] = G;
			resultimg.at<Vec3b>(y, x)[2] = R;
		}
	}

	imshow(imgname, orgimg);
	imshow(resultname, resultimg);
	waitKey();
}

int main(int argc, char** argv) {

	// 이미지 데이터가 없을 경우
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	int menu = 0;

	while (1) {
		int masksize = 0;

		cout << endl;
		cout << "***Select Menu***" << endl;
		cout << "\n" << endl;
		cout << "1. Blurring" << endl;
		cout << "2. Sharpening(Laplacian1)" << endl;
		cout << "3. Sharpening(Laplacian2)" << endl;
		cout << "4. EXIT" << endl;
		cout << "\n";
		cout << "Enter Number : ";

		cin >> menu;

		if (menu == 0 || menu > 6)
			cout << "Wrong Input!!!";

		else {
			switch (menu) {
			case 1:
				cout << endl;
				cout << "****Blurring****" << endl << endl;
				cout << "마스크 사이즈를 입력하세요. 홀수만 입력 가능합니다. : ";
				cin >> masksize;
				while (1) {
					if (masksize % 2 == 0) {
						cout << endl << "마스크 사이즈 홀수로 입력하세요 : ";
						cin >> masksize;
					}

					else {
						Blurring(masksize);
						break;
					}
				}
				break;
			case 2:
				Sharpening(menu);
				break;
			case 3:
				Sharpening(menu);
				break;
			case 4:
				exit(0);
			}
		}
	}
}