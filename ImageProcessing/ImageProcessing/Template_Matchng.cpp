#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// 이미지 데이터 읽기
Mat orgimg = imread("./img/template.jpg", CV_LOAD_IMAGE_COLOR);
Mat targetimg = imread("./img/matching.jpg", CV_LOAD_IMAGE_COLOR);

// 찾은 객체의 시작 좌표와 끝 좌표를 저장하기 위한 구조체
struct location {
	int start;
	int end;
};

// 좌표값을 저장하기 위한 벡터
vector<location> v[100];

// 데이터 일반화 240보다 밝으면 255로 그보다 어두우면 0으로 조정
void Normalize() {
	int B = 0, G = 0, R = 0;
	int sum = 0;
	for (int y = 0; y < orgimg.rows; y++) {
		for (int x = 0; x < orgimg.cols; x++) {
			B = orgimg.at<Vec3b>(y, x)[0];
			G = orgimg.at<Vec3b>(y, x)[1];
			R = orgimg.at<Vec3b>(y, x)[2];

			if (B > 240 || G > 240 || R > 240) {
				orgimg.at<Vec3b>(y, x)[0] = 255;
				orgimg.at<Vec3b>(y, x)[1] = 255;
				orgimg.at<Vec3b>(y, x)[2] = 255;
			}
			else {
				orgimg.at<Vec3b>(y, x)[0] = 0;
				orgimg.at<Vec3b>(y, x)[1] = 0;
				orgimg.at<Vec3b>(y, x)[2] = 0;
			}
		}
	}

	for (int y = 0; y < targetimg.rows; y++) {
		for (int x = 0; x < targetimg.cols; x++) {
			B = targetimg.at<Vec3b>(y, x)[0];
			G = targetimg.at<Vec3b>(y, x)[1];
			R = targetimg.at<Vec3b>(y, x)[2];

			if (B > 240 || G > 240 || R > 240) {
				targetimg.at<Vec3b>(y, x)[0] = 255;
				targetimg.at<Vec3b>(y, x)[1] = 255;
				targetimg.at<Vec3b>(y, x)[2] = 255;
			}
			else {
				targetimg.at<Vec3b>(y, x)[0] = 0;
				targetimg.at<Vec3b>(y, x)[1] = 0;
				targetimg.at<Vec3b>(y, x)[2] = 0;
			}
		}
	}
}

int main(int argc, char** argv) {
	int finalxs = 0, finalys = 0;
	int finalxe = 0, finalye = 0;

	String imgname = "Original Image";
	String targetname = "Target Image";
	String resultname = "Result Image";
	int window_height = targetimg.rows;	// 템플릿의 높이
	int window_width = targetimg.cols;	// 템플릿의 너비

	// 이미지 데이터가 없을 경우
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	Normalize();	// 데이터 일반화

	imshow(imgname, orgimg);	// 원본 이미지 출력

	int Y_startpoint = 0;	// Y좌표 시작점
	int X_startpoint = 0;	// X좌표 시작점
	int Y_endpoint = Y_startpoint + window_height;	// WINDOW를 적용할 Y축 끝부분
	int X_endpoint = X_startpoint + window_width;	// WINDOW를 적용할 X축 끝부분
	int B = 0;	// 템플릿을 찾을 영상의 픽셀값
	int t_B = 0;	// 템플릿의 픽셀값
	int label = -1;

	// 원본 영상을 전부 탐색할 때까지 반복
	while (Y_endpoint < orgimg.rows) {
		int Compare_count = 0;	// 비교하여 일치한 픽셀 수

		// 한 픽셀씩 원본 영상의 픽셀값과 템플릿 영상의 픽셀값을 비교
		for (int t_y = Y_startpoint; t_y < Y_endpoint; t_y++) {
			for (int t_x = X_startpoint; t_x < X_endpoint; t_x++) {
				B = orgimg.at<Vec3b>(t_y, t_x)[0];
				t_B = targetimg.at<Vec3b>(t_y - Y_startpoint, t_x - X_startpoint)[0];
				// 비교한 픽셀값이 같으면 Compare_count 1씩 증가
				if (B == t_B) {
					Compare_count += 1;
				}
			}
		}

		double avg;	// 일치율

		avg = ((double)Compare_count / (double)(window_height * window_width)) * 100;	// 일치율 계산

		// 일치율이 90% 넘으면 좌표값을 저장
		if (avg > 90) {
			label += 1;
			v[label].push_back({ X_startpoint, X_endpoint });
			v[label].push_back({ Y_startpoint, Y_endpoint });
		}

		X_startpoint += 1;

		X_endpoint = X_startpoint + window_width;

		// 너비 전부 탐색 시 
		if (X_endpoint > orgimg.cols) {
			X_startpoint = 0;
			X_endpoint = X_startpoint + window_width;
			Y_startpoint += 1;
			Y_endpoint = Y_startpoint + window_height;
		}
	}

	// 찾은 객체를 빨간색으로 변경
	for (int i = 0; i < label + 1; i++) {
		for (int y = v[i].back().start; y < v[i].back().end; y++) {
			for (int x = v[i].front().start; x < v[i].front().end; x++) {
				orgimg.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}
	

	// 이미지 출력
	imshow(targetname, targetimg);
	imshow("Result", orgimg);
	waitKey();
}