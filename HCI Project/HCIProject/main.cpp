
#include <opencv2/opencv.hpp>
#include "Util.h"

using namespace cv;

Mat IDVerifyingFunc(Mat src_img,bool &flag);
void MyCalcHist(Mat input_img1, Mat input_img2, double &count, bool &flag);
int main(){
	
	Mat idcard = imread("IDcard4.jpg",1);
	
	//이미지 크기 변경(500x312크기)
	resize(idcard, idcard, Size(500, 312), 0, 0, CV_INTER_LINEAR);

	std::cout << " /$$$$$$ /$$$$$$$        /$$    /$$ /$$$$$$$$ /$$$$$$$  /$$$$$$ /$$$$$$$$ /$$     /$$ /$$$$$$ /$$   /$$  /$$$$$$         /$$$$$$  /$$     /$$ /$$$$$$  /$$$$$$$$ /$$$$$$$$ /$$      /$$\n";
	std::cout << "|_  $$_/| $$__  $$      | $$   | $$| $$_____/| $$__  $$|_  $$_/| $$_____/|  $$   /$$/|_  $$_/| $$$ | $$ /$$__  $$       /$$__  $$|  $$   /$$//$$__  $$|__  $$__/| $$_____/| $$$    /$$$\n";
	std::cout << "  | $$  | $$  \\ $$      | $$   | $$| $$      | $$  \\ $$  | $$  | $$       \\  $$ /$$/   | $$  | $$$$| $$| $$  \\__/      | $$   \\__/\ \  $$ /$$/| $$  \\__/   | $$   | $$      | $$$$  /$$$$\n";
	std::cout << "  | $$  | $$  | $$      |  $$ / $$/| $$$$$   | $$$$$$$/  | $$  | $$$$$     \\  $$$$/    | $$  | $$ $$ $$| $$ /$$$$      |  $$$$$$   \\  $$$$/ |  $$$$$$    | $$   | $$$$$   | $$ $$/$$ $$\n";
	std::cout << "  | $$  | $$  | $$       \\  $$ $$/ | $$__/   | $$__  $$  | $$  | $$__/      \\  $$/     | $$  | $$  $$$$| $$|_  $$       \\____  $$   \\  $$/   \\____  $$   | $$   | $$__/   | $$  $$$| $$\n";
	std::cout << "  | $$  | $$  | $$        \\  $$$/  | $$      | $$  \\ $$  | $$  | $$          | $$      | $$  | $$\\  $$$| $$  \\ $$       /$$  \\ $$    | $$    /$$  \\ $$   | $$   | $$      | $$\\  $ | $$\n";
	std::cout << " /$$$$$$| $$$$$$$/         \\  $/   | $$$$$$$$| $$  | $$ /$$$$$$| $$          | $$     /$$$$$$| $$ \\  $$|  $$$$$$/      |  $$$$$$/    | $$   |  $$$$$$/   | $$   | $$$$$$$$| $$ \\/  | $$\n";
	std::cout << "|______/|_______/           \\_/    |________/|__/  |__/|______/|__/          |__/    |______/|__/  \\__/ \\______/        \\______/     |__/    \\______/    |__/   |________/|__/     |__/\n\n";



	//객체 생성
	bool accept = false;
	std::cout << "\n학생증 입력, 동국대학교 학생증이 맞는지 검사 시작.\n";


	Mat Result = IDVerifyingFunc(idcard, accept);
	if (accept)
		std::cout << "본교 학생증이 맞습니다! 가입을 환영합니다!\n";
	else
		std::cout << "본교 학생증이 아닙니다. 요청이 거절됩니다.\n";
	imshow("Result",Result );

	waitKey();
	return 0;
}
/* 
 * 입력받은 학생증 이미지를 템플릿 학생증과 비교하여 학생증을 인증하는 함수
 * 입력 : 학생증 이미지, 출력 : 템플릿 매칭이 된 결과 이미지
 */
Mat IDVerifyingFunc(Mat src_img, bool &flag) {


	//1. template매칭을 진행
	std::cout << "....템플릿 매칭 시작....\n\n";
	CUtil util;
	//템플릿 매칭에 사용될 5개의 템플릿
	Mat templates[5];
	templates[0] = imread("template1.jpg", 1);
	templates[1] = imread("template2.jpg", 1);
	templates[2] = imread("template3.jpg", 1);
	templates[3] = imread("template4.jpg", 1);
	templates[4] = imread("template5.jpg", 1);


	if (src_img.channels() != 3){
		printf("src_image의 채널이 컬러채널이 아닙니다");
		exit(0);
	}
	//검사 결과를 저장할 이미지 객체
	Mat dst_image = src_img.clone();

	//각템플릿이 일치하는지 여부를 저장할 bool 배열(false : 불일치, true : 일치)
	bool flags[5];
	
	//일치하는 템플릿 개수 저장용 double 변수
	double count = 0;
	for(int i = 0; i<5; i++){
		//template 매칭할 때 
		Mat matching_image = util.GS_createImage(Size(src_img.cols - templates[i].cols + 1, src_img.rows - templates[i].rows + 1), CV_32F, 1);
		/* Measures similarity between template and overlapped windows in the source image
		and fills the resultant image with the measurements */
		//openCV 템플릿 매칭 함수 사용.
		matchTemplate(src_img, templates[i], matching_image, TM_SQDIFF_NORMED);
		
		//최소/최대 값
		double  min_val, max_val;
		//최소/최대 위치
		Point min_loc, max_loc;
		
		cv::minMaxLoc(matching_image, &min_val, &max_val, &min_loc, &max_loc, cv::Mat());
		
		//자르기 위한 사각형
		Rect rect(min_loc.x, min_loc.y, templates[i].cols, templates[i].rows);

		//템플릿 크기만큼 자름.
		Mat cut_image = src_img(rect);
		//template과 자른 이미지를 HSV채널로 바꾸어 저장하기 위한 객체 생성
		Mat tempT, tempC;

		//HSV 채널로 변경
		cv::cvtColor(templates[i], tempT, COLOR_BGR2HSV);
		cv::cvtColor(cut_image, tempC, COLOR_BGR2HSV);
		//히스토그램 비교 진행
		MyCalcHist(tempT, tempC, count,flags[i]);
		std::cout << "\n템플릿 " << i + 1 << " 검사중.. ";
		if (flags[i]) {
			std::cout << "일치\n";
			rectangle(dst_image, min_loc, Point(min_loc.x + templates[i].cols, min_loc.y + templates[i].rows), Scalar(0, 0, 255), 2, 8, 0);
		}
		else
			std::cout << "불일치\n";
	}
	double accuracy = (count / 5) * 100;

	std::cout << "\n템플릿 일치율 : " << accuracy <<"%\n\n";
	if (accuracy > 50.0)
		flag = true;
	else 
		flag = false;

	return dst_image;
}


/* 각 이미지의 히스토그램을 계산하여 히스토그램을 비교한다.
 * 히스토그램을 비교하는 방법은 총 네 가지로, 네 가지중 세 가지가 통과 수준이면 일치하는 것으로 판단한다.
 * 입력 : input_img1:템플릿 이미지, input_img2 : 자른 이미지, count : 일치 템플릿을 세기 위한 변수, flag : 템플릿이 일치하는지 여부를 전달하기 위핸 bool 변수
 */
void MyCalcHist(Mat input_img1, Mat input_img2, double& count, bool& flag)
{
	std::cout << "\n템플릿과 부분 이미지 히스토그램 비교 시작..\n\n";
	//input1 : 템플릿 이미지, input2 : 자른 이미지
	Mat input1 = input_img1.clone();
	Mat input2 = input_img2.clone();
	
	//Hist1 : 템플릿 이미지의 히스토그램
	//Hist2 : 자른 이미지의 히스토그램
	MatND Hist1, Hist2;
	/*
	number_bins : histSize
	Channel_S : 히스토그램을 검사할 채널
	channel_ranges : 히스토그램 범위(0~255)
	*/
	const int number_bins = 50;
	const int channel_S[] = { 1 };
	float channel_ranges[] = { 0.0, 255 };
	const float* channelrange = channel_ranges;

	//openCV함수인 calcHist로 히스토그램 게산 및 생성
	calcHist(&input1, 1, channel_S, Mat(), Hist1, 1, &number_bins, &channelrange);
	calcHist(&input2, 1, channel_S, Mat(), Hist2, 1, &number_bins, &channelrange);

	//히스토그램 normalize. NORM_MINMAX를 사용
	normalize(Hist1, Hist1, 1, 0, NORM_MINMAX);
	normalize(Hist2, Hist2, 1, 0, NORM_MINMAX);

	/*
	* 총 네 가지 방법으로 비교.
	* 0: Correlation (COMP_CORREL)
	* 1: Chi-Square(COMP_CHISQR)
	* 2: Intersection(COMP_INTERSECT)
	* 3: Bhattacharyya distance(COMP_BHATTACHARYYA)
	*/
	int count_result = 0;
	double result[4];
	for (int i = 0; i < 4; i++) {
		//compareHist함수를 사용하여 두 이미지의 히스토그램을 방법 i로 비교한다
		result[i] = compareHist(Hist1, Hist2, i);
		switch(i) {
		case 0:
			//Correlation 방법의 결과가 0.8이상이면 일치하는것으로 판별
			if (result[i] >= 0.8)
				count_result++;
			break;
		case 1:
			//Chi-Square 방법의 결과가 0.3이하이면 일치하는것으로 판별
			if (result[i] <= 0.3)
				count_result++;
			break;
		case 2:
			//Intersection 방법의 결과가 0.8이상이면 일치하는것으로 판별
			if (result[i] >= 0.8)
				count_result++;
			break;
		case 3:
			//Bhattacharyya distance 방법의 결과가 0.3이하이면 일치하는 것으로 판별
			if (result[i] <= 0.3)
				count_result++;
			break;
		}
	}
	//만약 일치하는 것이 3개 이상일 때
	if (count_result >= 3) {
		flag = true;//부분 이미지가 현재 템플릿 이미지와 일치하다고 판단.
		count++;//일치 템플릿 개수 증가
	}
	else
		flag = false;
}