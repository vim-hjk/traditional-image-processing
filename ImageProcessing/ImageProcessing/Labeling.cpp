#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stack>

using namespace cv;
using namespace std;

String imgname = "Original Image";
String resultname = "Result Image";

// �̹��� ������ �б� �� ������� ������ Mat����
Mat orgimg = imread("./img/number.jpg", CV_LOAD_IMAGE_COLOR);
Mat resultimg = Mat(orgimg.rows, orgimg.cols, CV_8UC3);

// ��ǥ���� �����ϱ� ���� ����ü
struct location {
	int y;
	int x;
};

stack<location> Stack;	// ���� ����
int current_label = 0;	// ���� ��ȣ�� �ٿ��ֱ� ���� ����
stack<location> Label[6];	// �� �� ���� �迭

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
}

int main(int argc, char** argv) {	

	// �̹��� �����Ͱ� ���� ���
	if (!orgimg.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	//���� ���� �����ֱ�
	imshow(imgname, orgimg);
	// ���� ���� �Ϲ�ȭ
	Normalize();
	
	// ���� ��ġ�� ��� �ȼ��� ����
	int pixel;
	
	for (int y = 0; y < orgimg.rows; y++) {
		for (int x = 0; x < orgimg.cols; x++) {

			// ���� ��ġ�� ������ ����
			location vector = { y, x };
			// ������ġ�� ��簪�� ������ ����
			pixel = orgimg.at<Vec3b>(y, x)[0];

			// �󺧸��� �ȵ� ��ü�� �������� ó�� �߰����� ���
			if (Stack.empty() && pixel == 0) {
				// ��Ž���� �����ϱ� ���Ͽ� �߰� ��� �ش� ��ġ �ȼ��� ����
				orgimg.at<Vec3b>(y, x)[0] = 255;
				orgimg.at<Vec3b>(y, x)[1] = 255;
				orgimg.at<Vec3b>(y, x)[2] = 255;
				current_label += 1;	// �󺧸�
				Stack.push(vector);	// ���۽��ÿ� push
			}

			// ������ ���������� �ٲ�
			if (pixel == 255) {			
				resultimg.at<Vec3b>(y, x)[0] = 0;
				resultimg.at<Vec3b>(y, x)[1] = 0;
				resultimg.at<Vec3b>(y, x)[2] = 0;			
			}

			// �ش� ���� ��ü�� ���� Ž���� �� ����
			while (!Stack.empty()) {
				
				// ���۽����� top�� ����� ��ġ�� Ž�� ������������ �ٲ۴�
				int j = Stack.top().y;
				int i = Stack.top().x;
				// ���۽����� top�� ����� ��ġ�� �ش�� ���ÿ� push�� �� ���۽��� pop
				Label[current_label].push(Stack.top());
				Stack.pop();
				
				// Left Ž��
				if (i > 0) {
					if (orgimg.at<Vec3b>(j, i - 1)[0] == 0) {
						orgimg.at<Vec3b>(j, i - 1)[0] = 255;
						orgimg.at<Vec3b>(j, i - 1)[1] = 255;
						orgimg.at<Vec3b>(j, i - 1)[2] = 255;
						Stack.push({ j, i - 1 });
					}
				}
				
				// Right Ž��
				if (i < orgimg.cols) {
					if (orgimg.at<Vec3b>(j, i + 1)[0] == 0) {
						orgimg.at<Vec3b>(j, i + 1)[0] = 255;
						orgimg.at<Vec3b>(j, i + 1)[1] = 255;
						orgimg.at<Vec3b>(j, i + 1)[2] = 255;
						Stack.push({ j, i + 1 });
					}
				}
				
				// Up Ž��
				if (j > 0) {
					if (orgimg.at<Vec3b>(j - 1, i)[0] == 0) {
						orgimg.at<Vec3b>(j - 1, i)[0] = 255;
						orgimg.at<Vec3b>(j - 1, i)[1] = 255;
						orgimg.at<Vec3b>(j - 1, i)[2] = 255;
						Stack.push({ j - 1, i });
					}
				}

				// Down Ž��
				if (j < orgimg.rows) {
					if (orgimg.at<Vec3b>(j + 1, i)[0] == 0) {
						orgimg.at<Vec3b>(j + 1, i)[0] = 255;
						orgimg.at<Vec3b>(j + 1, i)[1] = 255;
						orgimg.at<Vec3b>(j + 1, i)[2] = 255;
						Stack.push({ j + 1, i });
					}					
				}
				// �󺧸��� ����� �Ǵ� �� Ȯ���ϱ� ���� ��¹�
				if (!Stack.empty()) {
					cout << Stack.top().x << endl << Stack.top().y << endl << "\n" << endl;
					cout << current_label << endl << Label[current_label].top().x << endl << Label[current_label].top().y << "\n" << endl;
				}
				
			}
		}
	}	
	
	// �󺧸��� ��ü�� �󺧺��� ������ �ٲ۴�.
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
	
	// ������� ���
	imshow(resultname, resultimg);
	waitKey();
}