#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat src, src_gray,src_copy;
int thresh = 130;
int max_thresh = 255;
const char* source_window = "Source image";
const char* corners_window = "Corners detected";
void cornerHarris_demo(int, void*);
int main(int argc, char** argv)
{
	
	src = imread("re.jpeg", 1);
	src_copy = imread("re.jpeg", 1);
	if (src.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		cout << "Usage: " << argv[0] << " <Input image>" << endl;
		return -1;
	}
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	src_gray.convertTo(src_gray, CV_32FC1);
	namedWindow(source_window);
	createTrackbar("Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo);
	cornerHarris_demo(0, 0);
	imshow(source_window, src);
	waitKey();
	return 0;
	

} void cornerHarris_demo(int, void*)
{
	int blockSize = 5;
	int apertureSize = 6;
	double k = 0.04;
	int size = 2;
	Mat dst = Mat::zeros(src.size(), CV_32FC1);
	Mat Ix = Mat::zeros(src.size(), CV_32FC1);
	Mat Iy = Mat::zeros(src.size(), CV_32FC1);
	float R;
	Sobel(src_gray, Ix, -1, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	Sobel(src_gray, Iy, -1, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	//cornerHarris(src_gray, dst, blockSize, apertureSize, k);
	for (int i = 0; i < src_gray.rows - blockSize; i++)
	{
		for (int j = 0; j < src_gray.cols - blockSize; j++)
		{
			Mat M = Mat::zeros(size, size, CV_32FC1);
			for (int k = 0; k < blockSize; k++)
			{
				for (int l = 0; l < blockSize; l++)
				{
					Mat temp = Mat::zeros(size, size, CV_32FC1);
					temp.at<float>(0, 0) =  (Ix.at<float>(i + k, j + l) * Ix.at<float>(i + k, j + l));
					temp.at<float>(1, 0) = (Ix.at<float>(i + k, j + l) * Iy.at<float>(i + k, j + l));
					temp.at<float>(0, 1) = (Ix.at<float>(i + k, j + l) * Iy.at<float>(i + k, j + l));
					temp.at<float>(1, 1) = (Iy.at<float>(i + k, j + l) * Iy.at<float>(i + k, j + l));
					add(temp, M, M, noArray(), -1);
				}
			}
			float sum = M.at<float>(1, 1) + M.at<float>(0, 0);
			

			R = determinant(M) - (k * (sum * sum));
			dst.at<float>(i, j) = R;
		}
	}
	Mat dst_norm, dst_norm_scaled;
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);
	for (int i = 0; i < dst_norm.rows; i++)
	{
		for (int j = 0; j < dst_norm.cols; j++)
		{
			if ((int)dst_norm.at<float>(i, j) > thresh)
			{
				circle(src_copy, Point(j, i), 5, Scalar(0), 2, 8, 0);
			}
		}
	}
	namedWindow(corners_window);
	imshow(corners_window, src_copy);
};
