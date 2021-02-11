#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	String imgname = "Original Image";
	String resultname = "Result Image";

	// �̹��� ������ �б� �� ������� ������ Mat����
	Mat orgimg, resultimg;
	orgimg = imread("./img/heq.jpg", CV_LOAD_IMAGE_COLOR);
	resultimg = Mat(orgimg.rows, orgimg.cols, CV_8UC3);

	// �̹��� �����Ͱ� ���� ���
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	// RGB���� ������ ������ �迭 ���� �� �ʱ�ȭ
	int m_histogramBlue[256] = { 0, };
	int m_histogramGreen[256] = { 0, };
	int m_histogramRed[256] = { 0, };
	int B, G, R;

	// �Ӱ谪 ����
	int th_low_B = 255, th_high_B = 0;
	int th_low_G = 255, th_high_G = 0;
	int th_low_R = 255, th_high_R = 0;
	

	for (int y = 0; y < orgimg.rows; y++) {
		for (int x = 0; x < orgimg.cols; x++) {
			// ���� ��
			B = orgimg.at<Vec3b>(y, x)[0];
			G = orgimg.at<Vec3b>(y, x)[1];
			R = orgimg.at<Vec3b>(y, x)[2];		

			// ��� ���� ���� ���� �� ã��
			if (B < th_low_B)	th_low_B = B;
			if (G < th_low_G)	th_low_G = G;
			if (R < th_low_R)	th_low_R = R;

			// ��� ���� ���� ���� �� ã��
			if (B > th_high_B) th_high_B = B;
			if (G > th_high_G) th_high_G = G;
			if (R > th_high_R) th_high_R = R;

			// ������׷� �迭�� �߰�
			m_histogramBlue[B] += 1;
			m_histogramGreen[G] += 1;
			m_histogramRed[R] += 1;
		}
	}

	// ���������� RGB�ִ밪�� ������ ���� �ʱ�ȭ 
	int m_histogramMaxB = 0;
	int m_histogramMaxG = 0;
	int m_histogramMaxR = 0;

	// ���������� RGB�ִ밪 ���
	for (int i = 0; i < 256; i++) {
		m_histogramMaxB = max(m_histogramMaxB, m_histogramBlue[i]);
		m_histogramMaxG = max(m_histogramMaxG, m_histogramGreen[i]);
		m_histogramMaxR = max(m_histogramMaxR, m_histogramRed[i]);
	}

	int B_LUT[256] = { 0 }, G_LUT[256] = { 0 }, R_LUT[256] = { 0 };	// Look Up Table ���� ������ �迭 �ʱ�ȭ

	// Look Up Table ���
	for (int i = 0; i < 256; i++) {
		if (i <= th_low_B) B_LUT[i] = 0;
		else if (th_low_B < i < th_high_B) B_LUT[i] = 255 * (i - th_low_B) / (th_high_B - th_low_B);
		else if (th_high_B <= i) B_LUT[i] = 255;

		if (i <= th_low_G) G_LUT[i] = 0;
		else if (th_low_G < i < th_high_G) G_LUT[i] = 255 * (i - th_low_G) / (th_high_G - th_low_G);
		else if (th_high_G <= i) G_LUT[i] = 255;

		if (i <= th_low_R) R_LUT[i] = 0;
		else if (th_low_R < i < th_high_R) R_LUT[i] = 255 * (i - th_low_R) / (th_high_R - th_low_R);
		else if (th_high_R <= i) R_LUT[i] = 255;
	}

	// ������� ������׷��� ������ �迭 ���� �� �ʱ�ȭ
	int m_histogramBlue2[256] = { 0, };
	int m_histogramGreen2[256] = { 0, };
	int m_histogramRed2[256] = { 0, };
	
	// ������� Look UP Table ���� �� ������׷� �迭�� �� �߰�
	for (int y = 0; y < orgimg.rows; y++) {
		for (int x = 0; x < orgimg.cols; x++) {
			// ���� ��
			B = orgimg.at<Vec3b>(y, x)[0];
			G = orgimg.at<Vec3b>(y, x)[1];
			R = orgimg.at<Vec3b>(y, x)[2];

			resultimg.at<Vec3b>(y, x)[0] = B_LUT[B];
			resultimg.at<Vec3b>(y, x)[1] = G_LUT[G];
			resultimg.at<Vec3b>(y, x)[2] = R_LUT[R];

			m_histogramBlue2[resultimg.at<Vec3b>(y, x)[0]] += 1;
			m_histogramGreen2[resultimg.at<Vec3b>(y, x)[1]] += 1;
			m_histogramRed2[resultimg.at<Vec3b>(y, x)[2]] += 1;
		}
	}

	// ��� ������ �ִ� RGB���� ������ ���� ���� �� �ʱ�ȭ
	int m_histogramMaxB2 = 0;
	int m_histogramMaxG2 = 0;
	int m_histogramMaxR2 = 0;

	// ��� ������ �ִ� RGB���� ���
	for (int i = 0; i < 256; i++) {
		m_histogramMaxB2 = max(m_histogramMaxB2, m_histogramBlue2[i]);
		m_histogramMaxG2 = max(m_histogramMaxG2, m_histogramGreen2[i]);
		m_histogramMaxR2 = max(m_histogramMaxR2, m_histogramRed2[i]);
	}

	// ������׷� Mat ����
	Mat img_histogram = Mat(300, 300, CV_8UC3, Scalar(0));
	Mat img_histogram2 = Mat(300, 300, CV_8UC3, Scalar(0));

	// ������׷� �׸���
	for (int i = 0; i < 256; i++)
	{
		int histoB = 300 * m_histogramBlue[i] / (float)m_histogramMaxB;
		int histoG = 300 * m_histogramGreen[i] / (float)m_histogramMaxG;
		int histoR = 300 * m_histogramRed[i] / (float)m_histogramMaxR;

		line(img_histogram, cvPoint(i + 10, 300), cvPoint(i + 10, 300 - histoB), Scalar(255, 0, 0));		
		line(img_histogram, cvPoint(i + 10, 300), cvPoint(i + 10, 300 - histoG), Scalar(0, 255, 0));
		line(img_histogram, cvPoint(i + 10, 300), cvPoint(i + 10, 300 - histoR), Scalar(0, 0, 255));

		int histoB2 = 300 * m_histogramBlue2[i] / (float)m_histogramMaxB2;
		int histoG2 = 300 * m_histogramGreen2[i] / (float)m_histogramMaxG2;
		int histoR2 = 300 * m_histogramRed2[i] / (float)m_histogramMaxR2;

		line(img_histogram2, cvPoint(i + 10, 300), cvPoint(i + 10, 300 - histoB2), Scalar(255, 0, 0));
		line(img_histogram2, cvPoint(i + 10, 300), cvPoint(i + 10, 300 - histoG2), Scalar(0, 255, 0));
		line(img_histogram2, cvPoint(i + 10, 300), cvPoint(i + 10, 300 - histoR2), Scalar(0, 0, 255));
	}

	// ��� ���
	imshow(imgname, orgimg);
	imshow("���� ������ ������׷�", img_histogram);
	imshow(resultname, resultimg);
	imshow("��� ������ ������׷�", img_histogram2);
	waitKey();
}