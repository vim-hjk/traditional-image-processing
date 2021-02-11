#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	String imgname1 = "Image 1";	
	String imgname2 = "Image 2";
	int B = 0, G = 0, R = 0;

	// 이미지 데이터 읽기
	Mat img1 = imread("./img/cmp1.jpg", CV_LOAD_IMAGE_COLOR);

	Mat img2 = imread("./img/cmp2.jpg", CV_LOAD_IMAGE_COLOR);
	
	// 이미지 데이터가 없을 경우
	if (!img1.data || !img2.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	imshow(imgname1, img1);
	imshow(imgname2, img2);

	for (int y = 0; y < img1.rows; y++) {
		for (int x = 0; x < img1.cols; x++) {
			B = img1.at<Vec3b>(y, x)[0];
			G = img1.at<Vec3b>(y, x)[1];
			R = img1.at<Vec3b>(y, x)[2];

			if (B != 0) img2.at<Vec3b>(y, x)[0] = B;
			if (G != 0) img2.at<Vec3b>(y, x)[1] = G;
			if (R != 0) img2.at<Vec3b>(y, x)[2] = R;
		}
	}

	imshow("Result", img2);
	waitKey();
}