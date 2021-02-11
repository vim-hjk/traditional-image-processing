#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

String result = "Histogram Matching Result";

// 이미지 데이터 읽기
Mat s_img = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
Mat ref_img = imread("flower.jpg", CV_LOAD_IMAGE_COLOR);
Mat resultimg = Mat(s_img.rows, s_img.cols, CV_8UC3);

// 평활화한 영상의 누적 히스토그램을 저장할 배열 선언
int result_cumulative_histogram_B[256] = { 0, };
int result_cumulative_histogram_G[256] = { 0, };
int result_cumulative_histogram_R[256] = { 0, };

// 원본 영상의 누적 히스토그램을 저장할 배열 선언
int cumulative_histogram_B[256] = { 0, };
int cumulative_histogram_G[256] = { 0, };
int cumulative_histogram_R[256] = { 0, };

// 히스토그램을 그리는 함수
void Draw_Histogram(String histogram_name, Mat histogram, int histogram_B[], int histogram_G[], int histogram_R[], int cumulative_histogram_B[], int cumulative_histogram_G[], int cumulative_histogram_R[]) {
	// 영상의 히스토그램 최대값 계산
	int histogramMax_B = 0, histogramMax_G = 0, histogramMax_R = 0;

	for (int i = 0; i < 256; i++) {
		histogramMax_B = max(histogramMax_B, histogram_B[i]);
		histogramMax_G = max(histogramMax_G, histogram_G[i]);
		histogramMax_R = max(histogramMax_R, histogram_R[i]);
	}

	// 히스토그램 그리기
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

	// 히스토그램 출력
	imshow(histogram_name, histogram);	
}

// 평활화한 영상의 히스토그램을 계산하는 함수
void Result_Histogram(String result_histogram_name, Mat result_img) {
	// 평활화한 영상의 히스토그램 계산	
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

	// 평활화한 영상의 누적 히스토그램 계산
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

	// 평활화한 영상의 히스토그램 그리기
	Mat result_histogram = Mat(300, 600, CV_8UC3, Scalar(255, 255, 255));
	Draw_Histogram(result_histogram_name, result_histogram, result_histogram_B, result_histogram_G, result_histogram_R, result_cumulative_histogram_B, result_cumulative_histogram_G, result_cumulative_histogram_R);	
}

// 히스토그램 평활화 함수
void Histogram_Equalization(String imgname, String resultname, String histogram_name, String result_histogram_name, Mat input_img) {
	imshow(imgname, input_img);

	//입력 그레이스케일 영상의 히스토그램 계산
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

	//입력 영상의 누적 히스토그램 계산	
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

	// 입력 영상의 정규화된 누적 히스토그램 계산
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

	// 히스토그램 평활화 적용
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

	// 평활화한 영상 출력
	imshow(resultname, result_img);

	// 평활화한 영상 히스토그램 계산
	Result_Histogram(result_histogram_name, result_img);

	// 원본 영상의 히스토그램 그리기
	Mat histogram = Mat(300, 600, CV_8UC3, Scalar(255, 255, 255));
	Draw_Histogram(histogram_name, histogram, histogram_B, histogram_G, histogram_R, cumulative_histogram_B, cumulative_histogram_G, cumulative_histogram_R);
}

// 히스토그램 명세화 함수
void Histogram_Matching(int source_normalized_cumulative_histogram_B[], int source_normalized_cumulative_histogram_G[], int source_normalized_cumulative_histogram_R[], int ref_normalized_cumulative_histogram_B[], int ref_normalized_cumulative_histogram_G[], int ref_normalized_cumulative_histogram_R[]) {
	
	// Lookup Table 선언 및 초기화
	int lookup_B[256] = { 0, };
	int lookup_G[256] = { 0, };
	int lookup_R[256] = { 0, };

	// 두 비교영상의 누적 히스토그램이 가장 가까운 픽셀끼리 변환
	for (int i = 0; i < 256; i++) {

		int k_B = 255;
		int k_G = 255;
		int k_R = 255;

		while (source_normalized_cumulative_histogram_B[i] < ref_normalized_cumulative_histogram_B[k_B]) k_B -= 1;

		if (k_B < 0)	lookup_B[i] = 0;
		else lookup_B[i] = k_B;

		while (source_normalized_cumulative_histogram_G[i] < ref_normalized_cumulative_histogram_G[k_G]) k_G -= 1;

		if (k_G < 0)	lookup_G[i] = 0;
		else lookup_G[i] = k_G;

		while (source_normalized_cumulative_histogram_R[i] < ref_normalized_cumulative_histogram_R[k_R]) k_R -= 1;

		if (k_R < 0)	lookup_R[i] = 0;
		else lookup_R[i] = k_R;
	}

	// Lookup Table을 적용하여 히스토그램 매칭
	for (int y = 0; y < s_img.rows; y++) {
		for (int x = 0; x < s_img.cols; x++) {
			resultimg.at<Vec3b>(y, x)[0] = lookup_B[s_img.at<Vec3b>(y, x)[0]];
			resultimg.at<Vec3b>(y, x)[1] = lookup_B[s_img.at<Vec3b>(y, x)[1]];
			resultimg.at<Vec3b>(y, x)[2] = lookup_B[s_img.at<Vec3b>(y, x)[2]];
		}
	}

	// 결과 영상의 히스토그램 계산	
	int result_histogram_B[256] = { 0, };
	int result_histogram_G[256] = { 0, };
	int result_histogram_R[256] = { 0, };

	for (int y = 0; y < resultimg.rows; y++) {
		for (int x = 0; x < resultimg.cols; x++) {
			result_histogram_B[resultimg.at<Vec3b>(y, x)[0]] += 1;
			result_histogram_G[resultimg.at<Vec3b>(y, x)[1]] += 1;
			result_histogram_R[resultimg.at<Vec3b>(y, x)[2]] += 1;
		}
	}

	// 결과 영상의 누적 히스토그램 계산
	int resultimg_cumulative_histogram_B[256] = { 0, };
	int sum_B = 0;
	int resultimg_cumulative_histogram_G[256] = { 0, };
	int sum_G = 0;
	int resultimg_cumulative_histogram_R[256] = { 0, };
	int sum_R = 0;

	for (int i = 1; i < 256; i++)
	{
		sum_B += result_histogram_B[i];
		resultimg_cumulative_histogram_B[i] = sum_B;
		sum_G += result_histogram_G[i];
		resultimg_cumulative_histogram_G[i] = sum_G;
		sum_R += result_histogram_R[i];
		resultimg_cumulative_histogram_R[i] = sum_R;
	}

	// 결과 영상의 히스토그램 그리기
	Mat resultimg_histogram = Mat(resultimg.rows, resultimg.cols, CV_8UC3, Scalar(255, 255, 255));
	Draw_Histogram("Matching Result Histogram", resultimg_histogram, result_histogram_B, result_histogram_G, result_histogram_R, resultimg_cumulative_histogram_B, resultimg_cumulative_histogram_G, resultimg_cumulative_histogram_R);
}

int main(int argc, char** argv) {	
	
	// 이미지 데이터가 없을 경우
	if (!s_img.data || !ref_img.data) {
		cout << "No Image data" << endl;
		return -1;
	}

	// 원본 영상의 누적 히스토그램값을 저장할 배열 선언 및 초기화
	int s_B[256] = { 0, };
	int s_G[256] = { 0, };
	int s_R[256] = { 0, };

	// 원본 영상의 누적 히스토그램값을 배열에 저장
	Histogram_Equalization("Source", "Source Result", "Source Histogram", "Source Result Histogram", s_img);
	for (int i = 0; i < 256; i++) {
		s_B[i] = result_cumulative_histogram_B[i];
		s_G[i] = result_cumulative_histogram_G[i];
		s_R[i] = result_cumulative_histogram_R[i];
	}

	// 참조 영상의 누적 히스토그램값을 저장할 배열 선언 및 초기화
	int ref_B[256] = { 0, };
	int ref_G[256] = { 0, };
	int ref_R[256] = { 0, };

	// 참조 영상의 누적 히스토그램값을 배열에 저장
	Histogram_Equalization("Reference", "Reference Result", "Reference Histogram", "Reference Result Histogram", ref_img);
	for (int i = 0; i < 256; i++) {
		ref_B[i] = result_cumulative_histogram_B[i];
		ref_G[i] = result_cumulative_histogram_G[i];
		ref_R[i] = result_cumulative_histogram_R[i];
	}

	// 히스토그램 매칭
	Histogram_Matching(s_B, s_G, s_R, ref_B, ref_G, ref_R);

	// 결과 출력
	imshow("Matching Result", resultimg);
	waitKey();
}
