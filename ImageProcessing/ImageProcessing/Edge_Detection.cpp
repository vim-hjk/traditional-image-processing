#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int masksize = 3;	// ����ũ ũ��
// �̹��� ������ �б�
Mat orgimg = imread("./img/edd.jpg", CV_LOAD_IMAGE_COLOR);
Mat retimg = Mat(orgimg.rows, orgimg.cols, CV_8UC3);
String imgname = "Original Image";

void Grayscale() {
	int sum = 0;	// ���� ������ RGB���� ����� ������ ���� ���� �� �ʱ�ȭ	

	// ��� ������ ������ GrayScale�� ��ȯ
	for (int y = 0; y < orgimg.rows; y++) {
		for (int x = 0; x < orgimg.cols; x++) {

			sum = (orgimg.at<Vec3b>(y, x)[0] + orgimg.at<Vec3b>(y, x)[1] + orgimg.at<Vec3b>(y, x)[2]) / 3;

			retimg.at<Vec3b>(y, x)[0] = sum;
			retimg.at<Vec3b>(y, x)[1] = sum;
			retimg.at<Vec3b>(y, x)[2] = sum;
		}
	}
}

void HorizontalMask() {

	int horizontal_mask[3][3] = { {-1,-2,-1},
								 {0,0,0},
								 {1,2,1} };
	int B, G, R;

	// Convolution ���� �� �����ڸ��� ������ �� ���⿡ �׵θ��� �����ϴ� �ݺ��� �� ���� ó��
	for (int y = masksize - 2; y < orgimg.rows - masksize / 2; y++)
	{
		for (int x = masksize - 2; x < orgimg.cols - masksize / 2; x++)
		{
			// ���� �ʱ�ȭ
			B = 0, G = 0, R = 0;

			// Horizontal Edge Detection
			for (int i = -1 * masksize / 2; i <= masksize / 2; i++) {
				for (int j = -1 * masksize / 2; j <= masksize / 2; j++) {
					int s_y = y + i;
					int s_x = x + j;

					// Convolution ����
					B += orgimg.at<Vec3b>(s_y, s_x)[0] * horizontal_mask[masksize / 2 + i][masksize / 2 + j];
					G += orgimg.at<Vec3b>(s_y, s_x)[1] * horizontal_mask[masksize / 2 + i][masksize / 2 + j];
					R += orgimg.at<Vec3b>(s_y, s_x)[2] * horizontal_mask[masksize / 2 + i][masksize / 2 + j];
				}
			}

			// ���, �����÷ο� ó��
			if (B > 255) B = 255;	if (B < 0) B = 0;
			if (G > 255) G = 255;	if (G < 0) G = 0;
			if (R > 255) R = 255;	if (R < 0) R = 0;

			// ã�� ������ Ư�� ���̻� �̸� ���������� ��Ÿ���� ��� ���� ���
			if (R > 70 && G > 70 && B > 70) {
				retimg.at<Vec3b>(y, x)[0] = 0;
				retimg.at<Vec3b>(y, x)[1] = 0;
				retimg.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}
}

void VerticalMask() {

	int vertical_mask[3][3] = { {-1,0,1},
								 {-2,0,2},
								 {-1,0,1} };
	int B, G, R;

	for (int y = masksize - 2; y < orgimg.rows - masksize / 2; y++)
	{
		for (int x = masksize - 2; x < orgimg.cols - masksize / 2; x++)
		{
			// ���� �ʱ�ȭ
			B = 0, G = 0, R = 0;

			// Vertical Edge Detection
			for (int i = -1 * masksize / 2; i <= masksize / 2; i++) {
				for (int j = -1 * masksize / 2; j <= masksize / 2; j++) {
					int s_y = y + i;
					int s_x = x + j;

					// Convolution ����
					B += orgimg.at<Vec3b>(s_y, s_x)[0] * vertical_mask[masksize / 2 + i][masksize / 2 + j];
					G += orgimg.at<Vec3b>(s_y, s_x)[1] * vertical_mask[masksize / 2 + i][masksize / 2 + j];
					R += orgimg.at<Vec3b>(s_y, s_x)[2] * vertical_mask[masksize / 2 + i][masksize / 2 + j];
				}
			}

			// ���, �����÷ο� ó��
			if (B > 255) B = 255;	if (B < 0) B = 0;
			if (G > 255) G = 255;	if (G < 0) G = 0;
			if (R > 255) R = 255;	if (R < 0) R = 0;

			// ã�� ������ Ư�� ���̻� �̸� �Ķ������� ��Ÿ���� ��� ���� ���
			if (R > 70 && G > 70 && B > 70) {
				retimg.at<Vec3b>(y, x)[0] = 255;
				retimg.at<Vec3b>(y, x)[1] = 0;
				retimg.at<Vec3b>(y, x)[2] = 0;
			}
		}
	}	
}

void Diagonal1Mask() {

	int Diagonal1_mask[3][3] = { {0,1,2},
								 {-1,0,1},
								 {-2,-1,0} };
	int B, G, R;

	for (int y = masksize - 2; y < orgimg.rows - masksize / 2; y++)
	{
		for (int x = masksize - 2; x < orgimg.cols - masksize / 2; x++)
		{
			// ���� �ʱ�ȭ
			B = 0, G = 0, R = 0;

			// Diagonal1 Edge Detection
			for (int i = -1 * masksize / 2; i <= masksize / 2; i++) {
				for (int j = -1 * masksize / 2; j <= masksize / 2; j++) {
					int s_y = y + i;
					int s_x = x + j;

					// Convolution ����
					B += orgimg.at<Vec3b>(s_y, s_x)[0] * Diagonal1_mask[masksize / 2 + i][masksize / 2 + j];
					G += orgimg.at<Vec3b>(s_y, s_x)[1] * Diagonal1_mask[masksize / 2 + i][masksize / 2 + j];
					R += orgimg.at<Vec3b>(s_y, s_x)[2] * Diagonal1_mask[masksize / 2 + i][masksize / 2 + j];
				}
			}

			// ���, �����÷ο� ó��
			if (B > 255) B = 255;	if (B < 0) B = 0;
			if (G > 255) G = 255;	if (G < 0) G = 0;
			if (R > 255) R = 255;	if (R < 0) R = 0;

			// ã�� ������ Ư�� ���̻� �̸� ������� ��Ÿ���� ��� ���� ���
			if (R > 70 && G > 70 && B > 70) {
				retimg.at<Vec3b>(y, x)[0] = 0;
				retimg.at<Vec3b>(y, x)[1] = 255;
				retimg.at<Vec3b>(y, x)[2] = 0;
			}
		}
	}
}

void Diagonal2Mask() {

	int Diagonal2_mask[3][3] = { {-2,-1,0},
								 {-1,0,1},
								 {0,1,2} };
	int  B, G, R;

	for (int y = masksize - 2; y < orgimg.rows - masksize / 2; y++)
	{
		for (int x = masksize - 2; x < orgimg.cols - masksize / 2; x++)
		{
			// ���� �ʱ�ȭ
			B = 0, G = 0, R = 0;

			// Diagonal2 Edge Detection
			for (int i = -1 * masksize / 2; i <= masksize / 2; i++) {
				for (int j = -1 * masksize / 2; j <= masksize / 2; j++) {
					int s_y = y + i;
					int s_x = x + j;

					// Convolution ����
					B += orgimg.at<Vec3b>(s_y, s_x)[0] * Diagonal2_mask[masksize / 2 + i][masksize / 2 + j];
					G += orgimg.at<Vec3b>(s_y, s_x)[1] * Diagonal2_mask[masksize / 2 + i][masksize / 2 + j];
					R += orgimg.at<Vec3b>(s_y, s_x)[2] * Diagonal2_mask[masksize / 2 + i][masksize / 2 + j];
				}
			}

			// ���, �����÷ο� ó��
			if (B > 255) B = 255;	if (B < 0) B = 0;
			if (G > 255) G = 255;	if (G < 0) G = 0;
			if (R > 255) R = 255;	if (R < 0) R = 0;

			// ã�� ������ Ư�� ���̻� �̸� ��������� ��Ÿ���� ��� ���� ���
			if (R > 70 && G > 70 && B > 70) {
				retimg.at<Vec3b>(y, x)[0] = 0;
				retimg.at<Vec3b>(y, x)[1] = 255;
				retimg.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}	
}

void Showimg() {
	// �������� �� ������� ���
	imshow(imgname, orgimg);
	imshow("result", retimg);
	waitKey(0);
}

int main(int argc, char** argv) {

	// �̹��� �����Ͱ� ���� ���
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	int menu = 0;

	while (1) {
		cout << "***Choose Mask***" << endl;
		cout << "\n" << endl;
		cout << "1. Horizontal" << endl;
		cout << "2. Vertical" << endl;
		cout << "3. Diagonal1" << endl;
		cout << "4. Diagonal2" << endl;
		cout << "5. All" << endl;
		cout << "6. EXIT" << endl;
		cout << "\n";
		cout << "Enter Number : ";

		cin >> menu;

		if (menu == 0 || menu > 6) 
			cout << "Wrong Input!!!";
		else {
			switch (menu) {
			case 1:
				Grayscale();
				HorizontalMask();
				Showimg();
				break;
			case 2:
				Grayscale();
				VerticalMask();
				Showimg();
				break;
			case 3:
				Grayscale();
				Diagonal1Mask();
				Showimg();
				break;
			case 4:
				Grayscale();
				Diagonal2Mask();
				Showimg();
				break;
			case 5:
				Grayscale();
				HorizontalMask();
				VerticalMask();
				Diagonal1Mask();
				Diagonal2Mask();
				Showimg();
				break;
			case 6:
				exit(0);
			}
		}		
	}	
}