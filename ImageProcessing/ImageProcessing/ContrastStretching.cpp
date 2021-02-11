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

	// 이미지 데이터 읽기 및 결과값을 저장할 Mat생성
	Mat orgimg, resultimg;
	orgimg = imread("./img/heq.jpg", CV_LOAD_IMAGE_COLOR);
	resultimg = Mat(orgimg.rows, orgimg.cols, CV_8UC3);

	// 이미지 데이터가 없을 경우
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	// RGB값별 분포를 저장할 배열 선언 및 초기화
	int m_histogramBlue[256] = { 0, };
	int m_histogramGreen[256] = { 0, };
	int m_histogramRed[256] = { 0, };
	int B, G, R;

	// 임계값 설정
	int th_low_B = 255, th_high_B = 0;
	int th_low_G = 255, th_high_G = 0;
	int th_low_R = 255, th_high_R = 0;
	

	for (int y = 0; y < orgimg.rows; y++) {
		for (int x = 0; x < orgimg.cols; x++) {
			// 원본 값
			B = orgimg.at<Vec3b>(y, x)[0];
			G = orgimg.at<Vec3b>(y, x)[1];
			R = orgimg.at<Vec3b>(y, x)[2];		

			// 밝기 값이 제일 낮은 값 찾기
			if (B < th_low_B)	th_low_B = B;
			if (G < th_low_G)	th_low_G = G;
			if (R < th_low_R)	th_low_R = R;

			// 밝기 값이 제일 높은 값 찾기
			if (B > th_high_B) th_high_B = B;
			if (G > th_high_G) th_high_G = G;
			if (R > th_high_R) th_high_R = R;

			// 히스토그램 배열에 추가
			m_histogramBlue[B] += 1;
			m_histogramGreen[G] += 1;
			m_histogramRed[R] += 1;
		}
	}

	// 원본영상의 RGB최대값을 저장할 변수 초기화 
	int m_histogramMaxB = 0;
	int m_histogramMaxG = 0;
	int m_histogramMaxR = 0;

	// 원본영상의 RGB최대값 계산
	for (int i = 0; i < 256; i++) {
		m_histogramMaxB = max(m_histogramMaxB, m_histogramBlue[i]);
		m_histogramMaxG = max(m_histogramMaxG, m_histogramGreen[i]);
		m_histogramMaxR = max(m_histogramMaxR, m_histogramRed[i]);
	}

	int B_LUT[256] = { 0 }, G_LUT[256] = { 0 }, R_LUT[256] = { 0 };	// Look Up Table 값을 저장할 배열 초기화

	// Look Up Table 계산
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

	// 결과값의 히스토그램을 저장할 배열 선언 및 초기화
	int m_histogramBlue2[256] = { 0, };
	int m_histogramGreen2[256] = { 0, };
	int m_histogramRed2[256] = { 0, };
	
	// 결과값에 Look UP Table 적용 및 히스토그램 배열에 값 추가
	for (int y = 0; y < orgimg.rows; y++) {
		for (int x = 0; x < orgimg.cols; x++) {
			// 원본 값
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

	// 결과 영상의 최대 RGB값을 정장할 변수 선언 및 초기화
	int m_histogramMaxB2 = 0;
	int m_histogramMaxG2 = 0;
	int m_histogramMaxR2 = 0;

	// 결과 영상의 최대 RGB값을 계산
	for (int i = 0; i < 256; i++) {
		m_histogramMaxB2 = max(m_histogramMaxB2, m_histogramBlue2[i]);
		m_histogramMaxG2 = max(m_histogramMaxG2, m_histogramGreen2[i]);
		m_histogramMaxR2 = max(m_histogramMaxR2, m_histogramRed2[i]);
	}

	// 히스토그램 Mat 생성
	Mat img_histogram = Mat(300, 300, CV_8UC3, Scalar(0));
	Mat img_histogram2 = Mat(300, 300, CV_8UC3, Scalar(0));

	// 히스토그램 그리기
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

	// 결과 출력
	imshow(imgname, orgimg);
	imshow("원본 영상의 히스토그램", img_histogram);
	imshow(resultname, resultimg);
	imshow("결과 영상의 히스토그램", img_histogram2);
	waitKey();
}