#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	String imgname = "Original Image";
	String resultname = "Result Image";

	Mat orgimg = imread("./img/flower.jpg", CV_LOAD_IMAGE_COLOR);
	Mat resultimg = Mat(orgimg.rows, orgimg.cols, CV_8UC3);
	
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}
	
	int B, G, R;
	int brightness;
	
	while (1) {
		cout << endl << "****Brightness****" << endl << endl;
		cout << "Enter the brightness : ";
		cin >> brightness;

		if (brightness == 0) {
			imshow(imgname, orgimg);
			waitKey();
			break;
		}

		for (int y = 0; y < orgimg.rows; y++) {
			for (int x = 0; x < orgimg.cols; x++) {
				//픽셀에서 값 가져오기
				B = orgimg.at<Vec3b>(y, x)[0];
				G = orgimg.at<Vec3b>(y, x)[1];
				R = orgimg.at<Vec3b>(y, x)[2];

				//이미지 밝게 하기
				B += brightness;	if (B > 255) B = 255;	if (B < 0) B = 0;
				G += brightness;	if (G > 255) G = 255;	if (G < 0) G = 0;
				R += brightness;	if (R > 255) R = 255;	if (R < 0) R = 0;

				resultimg.at<Vec3b>(y, x)[0] = B;
				resultimg.at<Vec3b>(y, x)[1] = G;
				resultimg.at<Vec3b>(y, x)[2] = R;
			}
		}

		imshow(imgname, orgimg);
		imshow(resultname, resultimg);
		waitKey();
	}
	
	
}