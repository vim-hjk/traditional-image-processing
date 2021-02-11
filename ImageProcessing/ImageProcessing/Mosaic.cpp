#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	String imgname = "Original Image";
	int window_size = 100;	// WINDOW ������
	int X_startpoint = 0;	// X��ǥ ������
	int Y_startpoint = 0;	// Y��ǥ ������
	int X_endpoint = X_startpoint + window_size;	// WINDOW�� ������ X�� ���κ�
	int Y_endpoint = Y_startpoint + window_size;	// WINDOW�� ������ Y�� ���κ�
	int B = 0, G = 0, R = 0;	// RGB�� �ʱ�ȭ

	// �̹��� ������ �б�
	Mat orgimg;
	orgimg = imread("./img/flower.jpg", CV_LOAD_IMAGE_COLOR);
	
	// �̹��� �����Ͱ� ���� ���
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	for (int y = 0; y < orgimg.rows / window_size + 1; y++) {

		for (int x = 0; x < orgimg.cols / window_size + 1; x++) {

			for (int m_y = Y_startpoint; m_y < Y_endpoint; m_y++) {
				for (int m_x = X_startpoint; m_x < X_endpoint; m_x++) {

					// �̹����� ������ WINDOW���� RGB���� ��� ���ϴ� �κ�
					B += orgimg.at<Vec3b>(m_y, m_x)[0];
					G += orgimg.at<Vec3b>(m_y, m_x)[1]; 
					R += orgimg.at<Vec3b>(m_y, m_x)[2];
				}
			}

			for (int m_y = Y_startpoint; m_y < Y_endpoint; m_y++) {
				for (int m_x = X_startpoint; m_x < X_endpoint; m_x++) {

					// �̹����� ������ WINDOW���� RGB���� ��� ���� ���� ����� ����Ͽ� ������ũ ����
					orgimg.at<Vec3b>(m_y, m_x)[0] = B / ((Y_endpoint - Y_startpoint) * (X_endpoint - X_startpoint));
					orgimg.at<Vec3b>(m_y, m_x)[1] = G / ((Y_endpoint - Y_startpoint) * (X_endpoint - X_startpoint));
					orgimg.at<Vec3b>(m_y, m_x)[2] = R / ((Y_endpoint - Y_startpoint) * (X_endpoint - X_startpoint));
				}
			}

			X_startpoint = X_endpoint;	// ������ WINDOW�� X��ǥ�� ����
			X_endpoint += window_size;
			if (X_endpoint > orgimg.cols)	X_endpoint = orgimg.cols;	// �̹����� �ʺ񺸴� WINDOW�� ������ X��ǥ �� �κ��� ũ�ٸ� X��ǥ�� �̹��� �ʺ�ũ��� ����
	
			B = 0, G = 0, R = 0;	// ���� WINDOW�� ������ �ȼ��� RGB�� �ʱ�ȭ
		}
		X_startpoint = 0;	// X��ǥ �� WINDOW�� ��� �������Ƿ� X��ǥ�� ����Ű�� �����͸� 0���� ����
		X_endpoint = X_startpoint + window_size;	
		Y_startpoint = Y_endpoint;	// Y��ǥ ���۰� ���� �� ���� ����
		Y_endpoint += window_size;
		if (Y_endpoint > orgimg.rows)	Y_endpoint = orgimg.rows;	// �̹����� ���̺��� WINDOW�� ������ Y��ǥ �� �κ��� ũ�ٸ� Y��ǥ �� �κ��� �̹����� ���̷� ����
	}
	
	imshow(imgname, orgimg);
	waitKey();
}