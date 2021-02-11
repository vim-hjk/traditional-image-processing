#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// �̹��� ������ �б�
Mat orgimg = imread("./img/template.jpg", CV_LOAD_IMAGE_COLOR);
Mat targetimg = imread("./img/matching.jpg", CV_LOAD_IMAGE_COLOR);

// ã�� ��ü�� ���� ��ǥ�� �� ��ǥ�� �����ϱ� ���� ����ü
struct location {
	int start;
	int end;
};

// ��ǥ���� �����ϱ� ���� ����
vector<location> v[100];

// ������ �Ϲ�ȭ 240���� ������ 255�� �׺��� ��ο�� 0���� ����
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
	int window_height = targetimg.rows;	// ���ø��� ����
	int window_width = targetimg.cols;	// ���ø��� �ʺ�

	// �̹��� �����Ͱ� ���� ���
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	Normalize();	// ������ �Ϲ�ȭ

	imshow(imgname, orgimg);	// ���� �̹��� ���

	int Y_startpoint = 0;	// Y��ǥ ������
	int X_startpoint = 0;	// X��ǥ ������
	int Y_endpoint = Y_startpoint + window_height;	// WINDOW�� ������ Y�� ���κ�
	int X_endpoint = X_startpoint + window_width;	// WINDOW�� ������ X�� ���κ�
	int B = 0;	// ���ø��� ã�� ������ �ȼ���
	int t_B = 0;	// ���ø��� �ȼ���
	int label = -1;

	// ���� ������ ���� Ž���� ������ �ݺ�
	while (Y_endpoint < orgimg.rows) {
		int Compare_count = 0;	// ���Ͽ� ��ġ�� �ȼ� ��

		// �� �ȼ��� ���� ������ �ȼ����� ���ø� ������ �ȼ����� ��
		for (int t_y = Y_startpoint; t_y < Y_endpoint; t_y++) {
			for (int t_x = X_startpoint; t_x < X_endpoint; t_x++) {
				B = orgimg.at<Vec3b>(t_y, t_x)[0];
				t_B = targetimg.at<Vec3b>(t_y - Y_startpoint, t_x - X_startpoint)[0];
				// ���� �ȼ����� ������ Compare_count 1�� ����
				if (B == t_B) {
					Compare_count += 1;
				}
			}
		}

		double avg;	// ��ġ��

		avg = ((double)Compare_count / (double)(window_height * window_width)) * 100;	// ��ġ�� ���

		// ��ġ���� 90% ������ ��ǥ���� ����
		if (avg > 90) {
			label += 1;
			v[label].push_back({ X_startpoint, X_endpoint });
			v[label].push_back({ Y_startpoint, Y_endpoint });
		}

		X_startpoint += 1;

		X_endpoint = X_startpoint + window_width;

		// �ʺ� ���� Ž�� �� 
		if (X_endpoint > orgimg.cols) {
			X_startpoint = 0;
			X_endpoint = X_startpoint + window_width;
			Y_startpoint += 1;
			Y_endpoint = Y_startpoint + window_height;
		}
	}

	// ã�� ��ü�� ���������� ����
	for (int i = 0; i < label + 1; i++) {
		for (int y = v[i].back().start; y < v[i].back().end; y++) {
			for (int x = v[i].front().start; x < v[i].front().end; x++) {
				orgimg.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}
	

	// �̹��� ���
	imshow(targetname, targetimg);
	imshow("Result", orgimg);
	waitKey();
}