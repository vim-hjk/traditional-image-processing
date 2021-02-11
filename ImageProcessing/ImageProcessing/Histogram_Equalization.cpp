#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

// �̹��� ������ �б�
Mat s_img = imread("./img/lena2.jpg", CV_LOAD_IMAGE_COLOR);

// ������׷��� �׸��� �Լ�
void Draw_Histogram(String histogram_name, Mat histogram, int histogram_B[], int histogram_G[], int histogram_R[], int cumulative_histogram_B[], int cumulative_histogram_G[], int cumulative_histogram_R[]) {
	// ������ ������׷� �ִ밪 ���
	int histogramMax_B = 0, histogramMax_G = 0, histogramMax_R = 0;

	for (int i = 0; i < 256; i++) {
		histogramMax_B = max(histogramMax_B, histogram_B[i]);
		histogramMax_G = max(histogramMax_G, histogram_G[i]);
		histogramMax_R = max(histogramMax_R, histogram_R[i]);
	}

	// ������׷� �׸���
	for (int i = 0; i < 256; i++)
	{
		int histo_B = 300 * histogram_B[i] / (float)histogramMax_B;
		int cumulative_histo_B = 300 * cumulative_histogram_B[i] / (float)cumulative_histogram_B[255];
		int histo_G = 300 * histogram_G[i] / (float)histogramMax_G;
		int cumulative_histo_G = 300 * cumulative_histogram_G[i] / (float)cumulative_histogram_G[255];
		int histo_R = 300 * histogram_R[i] / (float)histogramMax_R;
		int cumulative_histo_R = 300 * cumulative_histogram_R[i] / (float)cumulative_histogram_R[255];

		line(histogram, cvPoint(i + 10, 300), cvPoint(i + 10, 300 - histo_B), Scalar(255, 0, 0));
		line(histogram, cvPoint(i + 300, 300), cvPoint(i + 300, 300 - cumulative_histo_B), Scalar(255, 0, 0));
		line(histogram, cvPoint(i + 10, 300), cvPoint(i + 10, 300 - histo_G), Scalar(0, 255, 0));
		line(histogram, cvPoint(i + 300, 300), cvPoint(i + 300, 300 - cumulative_histo_G), Scalar(0, 255, 0));
		line(histogram, cvPoint(i + 10, 300), cvPoint(i + 10, 300 - histo_R), Scalar(0, 0, 255));
		line(histogram, cvPoint(i + 300, 300), cvPoint(i + 300, 300 - cumulative_histo_R), Scalar(0, 0, 255));
	}

	// ������׷� ���
	imshow(histogram_name, histogram);
}

// ��Ȱȭ�� ������ ������׷��� ����ϴ� �Լ�
void Result_Histogram(String result_histogram_name, Mat result_img) {
	// ��Ȱȭ�� ������ ������׷� ���
	int result_histogram_B[256] = { 0, };
	int result_histogram_G[256] = { 0, };
	int result_histogram_R[256] = { 0, };

	for (int y = 0; y < result_img.rows; y++) {
		for (int x = 0; x < result_img.cols; x++) {
			result_histogram_B[result_img.at<Vec3b>(y, x)[0]] += 1;
			result_histogram_G[result_img.at<Vec3b>(y, x)[1]] += 1;
			result_histogram_R[result_img.at<Vec3b>(y, x)[2]] += 1;
		}
	}

	// ��Ȱȭ�� ������ ���� ������׷��� ������ �迭 ����
	int result_cumulative_histogram_B[256] = { 0, };
	int result_cumulative_histogram_G[256] = { 0, };
	int result_cumulative_histogram_R[256] = { 0, };

	// ��Ȱȭ�� ������ ���� ������׷� ���
	int sum_B = 0;
	int sum_G = 0;
	int sum_R = 0;

	for (int i = 1; i < 256; i++)
	{
		sum_B += result_histogram_B[i];
		result_cumulative_histogram_B[i] = sum_B;
		sum_G += result_histogram_G[i];
		result_cumulative_histogram_G[i] = sum_G;
		sum_R += result_histogram_R[i];
		result_cumulative_histogram_R[i] = sum_R;
	}

	// ��Ȱȭ�� ������ ������׷� �׸���
	Mat result_histogram = Mat(300, 600, CV_8UC3, Scalar(255, 255, 255));
	Draw_Histogram(result_histogram_name, result_histogram, result_histogram_B, result_histogram_G, result_histogram_R, result_cumulative_histogram_B, result_cumulative_histogram_G, result_cumulative_histogram_R);
}

// ������׷� ��Ȱȭ �Լ�
void Histogram_Equalization(String imgname, String resultname, String histogram_name, String result_histogram_name, Mat input_img) {
	imshow(imgname, input_img);

	//�Է� �׷��̽����� ������ ������׷� ���
	int histogram_B[256] = { 0, };
	int histogram_G[256] = { 0, };
	int histogram_R[256] = { 0, };

	for (int y = 0; y < input_img.rows; y++)
	{
		for (int x = 0; x < input_img.cols; x++)
		{
			int value_B = input_img.at<Vec3b>(y, x)[0];
			histogram_B[value_B] += 1;
			int value_G = input_img.at<Vec3b>(y, x)[1];
			histogram_G[value_G] += 1;
			int value_R = input_img.at<Vec3b>(y, x)[2];
			histogram_R[value_R] += 1;
		}
	}

	// ���� ������ ���� ������׷��� ������ �迭 ����
	int cumulative_histogram_B[256] = { 0, };
	int cumulative_histogram_G[256] = { 0, };
	int cumulative_histogram_R[256] = { 0, };

	//�Է� ������ ���� ������׷� ���
	int sum_B = 0;
	int sum_G = 0;
	int sum_R = 0;

	for (int i = 1; i < 256; i++)
	{
		sum_B += histogram_B[i];
		cumulative_histogram_B[i] = sum_B;
		sum_G += histogram_G[i];
		cumulative_histogram_G[i] = sum_G;
		sum_R += histogram_R[i];
		cumulative_histogram_R[i] = sum_R;
	}

	// �Է� ������ ����ȭ�� ���� ������׷� ���
	float normalized_cumulative_histogram_B[256] = { 0.0, };
	float normalized_cumulative_histogram_G[256] = { 0.0, };
	float normalized_cumulative_histogram_R[256] = { 0.0, };
	int image_size = input_img.rows * input_img.cols;

	for (int i = 0; i < 256; i++)
	{
		normalized_cumulative_histogram_B[i] = cumulative_histogram_B[i] / (float)image_size;
		normalized_cumulative_histogram_G[i] = cumulative_histogram_G[i] / (float)image_size;
		normalized_cumulative_histogram_R[i] = cumulative_histogram_R[i] / (float)image_size;
	}

	// ������׷� ��Ȱȭ ����
	Mat result_img = Mat(input_img.rows, input_img.cols, CV_8UC3);
	for (int y = 0; y < input_img.rows; y++)
	{
		for (int x = 0; x < input_img.cols; x++)
		{
			result_img.at<Vec3b>(y, x)[0] = normalized_cumulative_histogram_B[input_img.at<Vec3b>(y, x)[0]] * 255;
			result_img.at<Vec3b>(y, x)[1] = normalized_cumulative_histogram_G[input_img.at<Vec3b>(y, x)[1]] * 255;
			result_img.at<Vec3b>(y, x)[2] = normalized_cumulative_histogram_R[input_img.at<Vec3b>(y, x)[2]] * 255;
		}
	}

	// ��Ȱȭ�� ���� ���
	imshow(resultname, result_img);

	// ��Ȱȭ�� ���� ������׷� ���
	Result_Histogram(result_histogram_name, result_img);

	// ���� ������ ������׷� �׸���
	Mat histogram = Mat(300, 600, CV_8UC3, Scalar(255, 255, 255));
	Draw_Histogram(histogram_name, histogram, histogram_B, histogram_G, histogram_R, cumulative_histogram_B, cumulative_histogram_G, cumulative_histogram_R);
}

int main(int argc, char** argv) {

	// �̹��� �����Ͱ� ���� ���
	if (!s_img.data) {
		cout << "No Image data" << endl;
		return -1;
	}
		
	Histogram_Equalization("Original Image", "Result Image", "Original Histogram", "Result Histogram", s_img);
	
	waitKey();
}