#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stack>

using namespace cv;
using namespace std;

String imgname = "Original Image";
String resultname = "Result Image";

// 이미지 데이터 읽기 및 결과값을 저장할 Mat생성
Mat orgimg = imread("./img/number.jpg", CV_LOAD_IMAGE_COLOR);
Mat resultimg = Mat(orgimg.rows, orgimg.cols, CV_8UC3);

// 좌표값을 저장하기 위한 구조체
struct location {
	int y;
	int x;
};

stack<location> Stack;	// 버퍼 스택
int current_label = 0;	// 라벨의 번호를 붙여주기 위한 변수
stack<location> Label[6];	// 라벨 별 스택 배열

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
}

int main(int argc, char** argv) {	

	// 이미지 데이터가 없을 경우
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	//원본 영상 보여주기
	imshow(imgname, orgimg);
	// 원본 영상 일반화
	Normalize();
	
	// 현재 위치의 블루 픽셀값 저장
	int pixel;
	
	for (int y = 0; y < orgimg.rows; y++) {
		for (int x = 0; x < orgimg.cols; x++) {

			// 현재 위치를 저장할 변수
			location vector = { y, x };
			// 현재위치의 블루값을 저장할 변수
			pixel = orgimg.at<Vec3b>(y, x)[0];

			// 라벨링이 안된 객체의 시작점을 처음 발견했을 경우
			if (Stack.empty() && pixel == 0) {
				// 재탐색을 방지하기 위하여 발견 즉시 해당 위치 픽셀을 지움
				orgimg.at<Vec3b>(y, x)[0] = 255;
				orgimg.at<Vec3b>(y, x)[1] = 255;
				orgimg.at<Vec3b>(y, x)[2] = 255;
				current_label += 1;	// 라벨링
				Stack.push(vector);	// 버퍼스택에 push
			}

			// 배경색을 검정색으로 바꿈
			if (pixel == 255) {			
				resultimg.at<Vec3b>(y, x)[0] = 0;
				resultimg.at<Vec3b>(y, x)[1] = 0;
				resultimg.at<Vec3b>(y, x)[2] = 0;			
			}

			// 해당 라벨의 객체를 전부 탐색할 때 까지
			while (!Stack.empty()) {
				
				// 버퍼스택의 top에 저장된 위치를 탐색 시작지점으로 바꾼다
				int j = Stack.top().y;
				int i = Stack.top().x;
				// 버퍼스택의 top에 저장된 위치를 해당라벨 스택에 push한 후 버퍼스택 pop
				Label[current_label].push(Stack.top());
				Stack.pop();
				
				// Left 탐색
				if (i > 0) {
					if (orgimg.at<Vec3b>(j, i - 1)[0] == 0) {
						orgimg.at<Vec3b>(j, i - 1)[0] = 255;
						orgimg.at<Vec3b>(j, i - 1)[1] = 255;
						orgimg.at<Vec3b>(j, i - 1)[2] = 255;
						Stack.push({ j, i - 1 });
					}
				}
				
				// Right 탐색
				if (i < orgimg.cols) {
					if (orgimg.at<Vec3b>(j, i + 1)[0] == 0) {
						orgimg.at<Vec3b>(j, i + 1)[0] = 255;
						orgimg.at<Vec3b>(j, i + 1)[1] = 255;
						orgimg.at<Vec3b>(j, i + 1)[2] = 255;
						Stack.push({ j, i + 1 });
					}
				}
				
				// Up 탐색
				if (j > 0) {
					if (orgimg.at<Vec3b>(j - 1, i)[0] == 0) {
						orgimg.at<Vec3b>(j - 1, i)[0] = 255;
						orgimg.at<Vec3b>(j - 1, i)[1] = 255;
						orgimg.at<Vec3b>(j - 1, i)[2] = 255;
						Stack.push({ j - 1, i });
					}
				}

				// Down 탐색
				if (j < orgimg.rows) {
					if (orgimg.at<Vec3b>(j + 1, i)[0] == 0) {
						orgimg.at<Vec3b>(j + 1, i)[0] = 255;
						orgimg.at<Vec3b>(j + 1, i)[1] = 255;
						orgimg.at<Vec3b>(j + 1, i)[2] = 255;
						Stack.push({ j + 1, i });
					}					
				}
				//// 라벨링이 제대로 되는 지 확인하기 위한 출력문
				//if (!Stack.empty()) {
				//	cout << Stack.top().x << endl << Stack.top().y << endl << "\n" << endl;
				//	cout << current_label << endl << Label[current_label].top().x << endl << Label[current_label].top().y << "\n" << endl;
				//}
				
			}
		}
	}	
	
	// 라벨링한 객체를 라벨별로 색상을 바꾼다.
	for (int i = 1; i < current_label + 1; i++) {		
		while (!Label[i].empty()) {

			int y = Label[i].top().y;
			int x = Label[i].top().x;
			Label[i].pop();

			if (i == 1) {
				resultimg.at<Vec3b>(y, x)[0] = 255;
				resultimg.at<Vec3b>(y, x)[1] = 0;
				resultimg.at<Vec3b>(y, x)[2] = 0;
			}

			if (i == 2) {
				resultimg.at<Vec3b>(y, x)[0] = 0;
				resultimg.at<Vec3b>(y, x)[1] = 255;
				resultimg.at<Vec3b>(y, x)[2] = 0;
			}

			if (i == 3) {
				resultimg.at<Vec3b>(y, x)[0] = 0;
				resultimg.at<Vec3b>(y, x)[1] = 0;
				resultimg.at<Vec3b>(y, x)[2] = 255;
			}

			if (i == 4) {
				resultimg.at<Vec3b>(y, x)[0] = 0;
				resultimg.at<Vec3b>(y, x)[1] = 255;
				resultimg.at<Vec3b>(y, x)[2] = 255;
			}

			if (i == 5) {
				resultimg.at<Vec3b>(y, x)[0] = 255;
				resultimg.at<Vec3b>(y, x)[1] = 0;
				resultimg.at<Vec3b>(y, x)[2] = 255;
			}
		}		
	}	
	
	// 결과영상 출력
	imshow(resultname, resultimg);
	waitKey();
}
