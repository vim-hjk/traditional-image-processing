#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	String imgname = "Original Image";
	int window_size = 100;	// WINDOW 사이즈
	int X_startpoint = 0;	// X좌표 시작점
	int Y_startpoint = 0;	// Y좌표 시작점
	int X_endpoint = X_startpoint + window_size;	// WINDOW를 적용할 X축 끝부분
	int Y_endpoint = Y_startpoint + window_size;	// WINDOW를 적용할 Y축 끝부분
	int B = 0, G = 0, R = 0;	// RGB값 초기화

	// 이미지 데이터 읽기
	Mat orgimg;
	orgimg = imread("./img/flower.jpg", CV_LOAD_IMAGE_COLOR);
	
	// 이미지 데이터가 없을 경우
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	for (int y = 0; y < orgimg.rows / window_size + 1; y++) {

		for (int x = 0; x < orgimg.cols / window_size + 1; x++) {

			for (int m_y = Y_startpoint; m_y < Y_endpoint; m_y++) {
				for (int m_x = X_startpoint; m_x < X_endpoint; m_x++) {

					// 이미지에 적용한 WINDOW영역 RGB값을 모두 더하는 부분
					B += orgimg.at<Vec3b>(m_y, m_x)[0];
					G += orgimg.at<Vec3b>(m_y, m_x)[1]; 
					R += orgimg.at<Vec3b>(m_y, m_x)[2];
				}
			}

			for (int m_y = Y_startpoint; m_y < Y_endpoint; m_y++) {
				for (int m_x = X_startpoint; m_x < X_endpoint; m_x++) {

					// 이미지에 적용한 WINDOW영역 RGB값을 모두 더한 값의 평균을 계산하여 모자이크 적용
					orgimg.at<Vec3b>(m_y, m_x)[0] = B / ((Y_endpoint - Y_startpoint) * (X_endpoint - X_startpoint));
					orgimg.at<Vec3b>(m_y, m_x)[1] = G / ((Y_endpoint - Y_startpoint) * (X_endpoint - X_startpoint));
					orgimg.at<Vec3b>(m_y, m_x)[2] = R / ((Y_endpoint - Y_startpoint) * (X_endpoint - X_startpoint));
				}
			}

			X_startpoint = X_endpoint;	// 적용할 WINDOW의 X좌표값 조정
			X_endpoint += window_size;
			if (X_endpoint > orgimg.cols)	X_endpoint = orgimg.cols;	// 이미지의 너비보다 WINDOW를 적용할 X좌표 끝 부분이 크다면 X좌표를 이미지 너비크기로 조정
	
			B = 0, G = 0, R = 0;	// 새로 WINDOW를 적용할 픽셀의 RGB값 초기화
		}
		X_startpoint = 0;	// X좌표 한 WINDOW가 모두 끝났으므로 X좌표를 가리키는 포인터를 0으로 조정
		X_endpoint = X_startpoint + window_size;	
		Y_startpoint = Y_endpoint;	// Y좌표 시작값 조정 및 끝값 증가
		Y_endpoint += window_size;
		if (Y_endpoint > orgimg.rows)	Y_endpoint = orgimg.rows;	// 이미지의 높이보다 WINDOW를 적용할 Y좌표 끝 부분이 크다면 Y좌표 끝 부분을 이미지의 높이로 조정
	}
	
	imshow(imgname, orgimg);
	waitKey();
}