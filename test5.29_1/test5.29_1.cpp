// test5.29_1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

void getHist(Mat srcMat, float * hist, int nX, int nY, int cellsize);
float getDistance(float * hist1, float * hist2, int bins);

int main()
{
	Mat objMat = imread("template.png", 0);
	Mat srcMat = imread("img.png", 0);

	if (objMat.empty() || srcMat.empty())
	{
		cout << "找不到图片！" << endl;
		return 0;
	}
	Point Loc;
	float min_dis = 3.4e38;
	int cellsize = 16;
	int nX = objMat.cols / cellsize;
	int nY = objMat.rows / cellsize;
	int bins = 8 * nX * nY;
	float * obj_hist = new float[bins];
	memset(obj_hist, 0, sizeof(float)*bins);
	//float * block_hist = new float[bins];
	//memset(block_hist, 0, sizeof(float)*bins);
	getHist(objMat, obj_hist, nX, nY, cellsize);
	for (int i = 0; i < srcMat.rows - objMat.rows + 1; i++)  //遍历图片
	{
		for (int j = 0; j < srcMat.cols - objMat.cols + 1; j++)
		{
			Rect rect(j, i, objMat.cols, objMat.rows);
			Mat block = srcMat(rect);
			float * block_hist = new float[bins];
			memset(block_hist, 0, sizeof(float)*bins);
			getHist(block, block_hist, nX, nY, cellsize);
			float dis = getDistance(obj_hist, block_hist, bins);

			if (dis < min_dis)
			{
				min_dis = dis;
				Loc.x = j;
				Loc.y = i;
			}
			delete[] block_hist;
		}
	}
	delete[] obj_hist;
	cout << "最小距离：" << min_dis << endl;
	cout << "坐标：" <<	 Loc << endl;
	rectangle(srcMat, Loc, Point(Loc.x + objMat.cols, Loc.y + objMat.rows), Scalar(255, 255, 0), 1, 8);
	imshow("result", srcMat);
	waitKey(0);
	return 0;
}

void getHist(Mat srcMat, float * hist, int nX, int nY, int cellsize)
{
	Mat gx, gy;
	Mat mag, angle;
	Sobel(srcMat, gx, CV_32F, 1, 0, 1);
	Sobel(srcMat, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	for (int i = 0; i < nY * cellsize; i++)
	{
		for (int j = 0; j < nX * cellsize; j++)
		{
			int angle_q = angle.at<float>(i, j) / 45;
			*(hist + (i / cellsize * nX + j / cellsize) * 8 + angle_q) += mag.at<float>(i, j);
		}
	}
}

float getDistance(float * hist1, float * hist2, int bins)
{
	float sum = 0;
	for (int i = 0; i < bins; i++)
	{
		sum += pow(*(hist1 + i) - *(hist2 + i), 2);
	}
	return sqrt(sum);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
