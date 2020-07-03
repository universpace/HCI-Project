
#include <opencv2/opencv.hpp>
#include "Util.h"

using namespace cv;

Mat IDVerifyingFunc(Mat src_img,bool &flag);
void MyCalcHist(Mat input_img1, Mat input_img2, double &count, bool &flag);
int main(){
	
	Mat idcard = imread("IDcard4.jpg",1);
	
	//�̹��� ũ�� ����(500x312ũ��)
	resize(idcard, idcard, Size(500, 312), 0, 0, CV_INTER_LINEAR);

	std::cout << " /$$$$$$ /$$$$$$$        /$$    /$$ /$$$$$$$$ /$$$$$$$  /$$$$$$ /$$$$$$$$ /$$     /$$ /$$$$$$ /$$   /$$  /$$$$$$         /$$$$$$  /$$     /$$ /$$$$$$  /$$$$$$$$ /$$$$$$$$ /$$      /$$\n";
	std::cout << "|_  $$_/| $$__  $$      | $$   | $$| $$_____/| $$__  $$|_  $$_/| $$_____/|  $$   /$$/|_  $$_/| $$$ | $$ /$$__  $$       /$$__  $$|  $$   /$$//$$__  $$|__  $$__/| $$_____/| $$$    /$$$\n";
	std::cout << "  | $$  | $$  \\ $$      | $$   | $$| $$      | $$  \\ $$  | $$  | $$       \\  $$ /$$/   | $$  | $$$$| $$| $$  \\__/      | $$   \\__/\ \  $$ /$$/| $$  \\__/   | $$   | $$      | $$$$  /$$$$\n";
	std::cout << "  | $$  | $$  | $$      |  $$ / $$/| $$$$$   | $$$$$$$/  | $$  | $$$$$     \\  $$$$/    | $$  | $$ $$ $$| $$ /$$$$      |  $$$$$$   \\  $$$$/ |  $$$$$$    | $$   | $$$$$   | $$ $$/$$ $$\n";
	std::cout << "  | $$  | $$  | $$       \\  $$ $$/ | $$__/   | $$__  $$  | $$  | $$__/      \\  $$/     | $$  | $$  $$$$| $$|_  $$       \\____  $$   \\  $$/   \\____  $$   | $$   | $$__/   | $$  $$$| $$\n";
	std::cout << "  | $$  | $$  | $$        \\  $$$/  | $$      | $$  \\ $$  | $$  | $$          | $$      | $$  | $$\\  $$$| $$  \\ $$       /$$  \\ $$    | $$    /$$  \\ $$   | $$   | $$      | $$\\  $ | $$\n";
	std::cout << " /$$$$$$| $$$$$$$/         \\  $/   | $$$$$$$$| $$  | $$ /$$$$$$| $$          | $$     /$$$$$$| $$ \\  $$|  $$$$$$/      |  $$$$$$/    | $$   |  $$$$$$/   | $$   | $$$$$$$$| $$ \\/  | $$\n";
	std::cout << "|______/|_______/           \\_/    |________/|__/  |__/|______/|__/          |__/    |______/|__/  \\__/ \\______/        \\______/     |__/    \\______/    |__/   |________/|__/     |__/\n\n";



	//��ü ����
	bool accept = false;
	std::cout << "\n�л��� �Է�, �������б� �л����� �´��� �˻� ����.\n";


	Mat Result = IDVerifyingFunc(idcard, accept);
	if (accept)
		std::cout << "���� �л����� �½��ϴ�! ������ ȯ���մϴ�!\n";
	else
		std::cout << "���� �л����� �ƴմϴ�. ��û�� �����˴ϴ�.\n";
	imshow("Result",Result );

	waitKey();
	return 0;
}
/* 
 * �Է¹��� �л��� �̹����� ���ø� �л����� ���Ͽ� �л����� �����ϴ� �Լ�
 * �Է� : �л��� �̹���, ��� : ���ø� ��Ī�� �� ��� �̹���
 */
Mat IDVerifyingFunc(Mat src_img, bool &flag) {


	//1. template��Ī�� ����
	std::cout << "....���ø� ��Ī ����....\n\n";
	CUtil util;
	//���ø� ��Ī�� ���� 5���� ���ø�
	Mat templates[5];
	templates[0] = imread("template1.jpg", 1);
	templates[1] = imread("template2.jpg", 1);
	templates[2] = imread("template3.jpg", 1);
	templates[3] = imread("template4.jpg", 1);
	templates[4] = imread("template5.jpg", 1);


	if (src_img.channels() != 3){
		printf("src_image�� ä���� �÷�ä���� �ƴմϴ�");
		exit(0);
	}
	//�˻� ����� ������ �̹��� ��ü
	Mat dst_image = src_img.clone();

	//�����ø��� ��ġ�ϴ��� ���θ� ������ bool �迭(false : ����ġ, true : ��ġ)
	bool flags[5];
	
	//��ġ�ϴ� ���ø� ���� ����� double ����
	double count = 0;
	for(int i = 0; i<5; i++){
		//template ��Ī�� �� 
		Mat matching_image = util.GS_createImage(Size(src_img.cols - templates[i].cols + 1, src_img.rows - templates[i].rows + 1), CV_32F, 1);
		/* Measures similarity between template and overlapped windows in the source image
		and fills the resultant image with the measurements */
		//openCV ���ø� ��Ī �Լ� ���.
		matchTemplate(src_img, templates[i], matching_image, TM_SQDIFF_NORMED);
		
		//�ּ�/�ִ� ��
		double  min_val, max_val;
		//�ּ�/�ִ� ��ġ
		Point min_loc, max_loc;
		
		cv::minMaxLoc(matching_image, &min_val, &max_val, &min_loc, &max_loc, cv::Mat());
		
		//�ڸ��� ���� �簢��
		Rect rect(min_loc.x, min_loc.y, templates[i].cols, templates[i].rows);

		//���ø� ũ�⸸ŭ �ڸ�.
		Mat cut_image = src_img(rect);
		//template�� �ڸ� �̹����� HSVä�η� �ٲپ� �����ϱ� ���� ��ü ����
		Mat tempT, tempC;

		//HSV ä�η� ����
		cv::cvtColor(templates[i], tempT, COLOR_BGR2HSV);
		cv::cvtColor(cut_image, tempC, COLOR_BGR2HSV);
		//������׷� �� ����
		MyCalcHist(tempT, tempC, count,flags[i]);
		std::cout << "\n���ø� " << i + 1 << " �˻���.. ";
		if (flags[i]) {
			std::cout << "��ġ\n";
			rectangle(dst_image, min_loc, Point(min_loc.x + templates[i].cols, min_loc.y + templates[i].rows), Scalar(0, 0, 255), 2, 8, 0);
		}
		else
			std::cout << "����ġ\n";
	}
	double accuracy = (count / 5) * 100;

	std::cout << "\n���ø� ��ġ�� : " << accuracy <<"%\n\n";
	if (accuracy > 50.0)
		flag = true;
	else 
		flag = false;

	return dst_image;
}


/* �� �̹����� ������׷��� ����Ͽ� ������׷��� ���Ѵ�.
 * ������׷��� ���ϴ� ����� �� �� ������, �� ������ �� ������ ��� �����̸� ��ġ�ϴ� ������ �Ǵ��Ѵ�.
 * �Է� : input_img1:���ø� �̹���, input_img2 : �ڸ� �̹���, count : ��ġ ���ø��� ���� ���� ����, flag : ���ø��� ��ġ�ϴ��� ���θ� �����ϱ� ���� bool ����
 */
void MyCalcHist(Mat input_img1, Mat input_img2, double& count, bool& flag)
{
	std::cout << "\n���ø��� �κ� �̹��� ������׷� �� ����..\n\n";
	//input1 : ���ø� �̹���, input2 : �ڸ� �̹���
	Mat input1 = input_img1.clone();
	Mat input2 = input_img2.clone();
	
	//Hist1 : ���ø� �̹����� ������׷�
	//Hist2 : �ڸ� �̹����� ������׷�
	MatND Hist1, Hist2;
	/*
	number_bins : histSize
	Channel_S : ������׷��� �˻��� ä��
	channel_ranges : ������׷� ����(0~255)
	*/
	const int number_bins = 50;
	const int channel_S[] = { 1 };
	float channel_ranges[] = { 0.0, 255 };
	const float* channelrange = channel_ranges;

	//openCV�Լ��� calcHist�� ������׷� �Ի� �� ����
	calcHist(&input1, 1, channel_S, Mat(), Hist1, 1, &number_bins, &channelrange);
	calcHist(&input2, 1, channel_S, Mat(), Hist2, 1, &number_bins, &channelrange);

	//������׷� normalize. NORM_MINMAX�� ���
	normalize(Hist1, Hist1, 1, 0, NORM_MINMAX);
	normalize(Hist2, Hist2, 1, 0, NORM_MINMAX);

	/*
	* �� �� ���� ������� ��.
	* 0: Correlation (COMP_CORREL)
	* 1: Chi-Square(COMP_CHISQR)
	* 2: Intersection(COMP_INTERSECT)
	* 3: Bhattacharyya distance(COMP_BHATTACHARYYA)
	*/
	int count_result = 0;
	double result[4];
	for (int i = 0; i < 4; i++) {
		//compareHist�Լ��� ����Ͽ� �� �̹����� ������׷��� ��� i�� ���Ѵ�
		result[i] = compareHist(Hist1, Hist2, i);
		switch(i) {
		case 0:
			//Correlation ����� ����� 0.8�̻��̸� ��ġ�ϴ°����� �Ǻ�
			if (result[i] >= 0.8)
				count_result++;
			break;
		case 1:
			//Chi-Square ����� ����� 0.3�����̸� ��ġ�ϴ°����� �Ǻ�
			if (result[i] <= 0.3)
				count_result++;
			break;
		case 2:
			//Intersection ����� ����� 0.8�̻��̸� ��ġ�ϴ°����� �Ǻ�
			if (result[i] >= 0.8)
				count_result++;
			break;
		case 3:
			//Bhattacharyya distance ����� ����� 0.3�����̸� ��ġ�ϴ� ������ �Ǻ�
			if (result[i] <= 0.3)
				count_result++;
			break;
		}
	}
	//���� ��ġ�ϴ� ���� 3�� �̻��� ��
	if (count_result >= 3) {
		flag = true;//�κ� �̹����� ���� ���ø� �̹����� ��ġ�ϴٰ� �Ǵ�.
		count++;//��ġ ���ø� ���� ����
	}
	else
		flag = false;
}