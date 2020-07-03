// Pixel.h: interface for the CPixel class.
//
//////////////////////////////////////////////////////////////////////

#include <opencv2/opencv.hpp>

typedef unsigned char BYTE;

class CPixel  
{
public:
	CPixel();
	virtual ~CPixel();

	// 레벨별 명암 보기
	cv::Mat GS_makeGrayBand();

	// 명암 대비 보기
	cv::Mat GS_makeContrast();

	// 밝기 조절 - 덧셈/뺄셈
	cv::Mat GS_add_constant(cv::Mat src_image, int constant );

	// 두 영상간 덧셈
	cv::Mat GS_add_image(cv::Mat src_image1, cv::Mat src_image2 );

	// 두 영상간 뺄셈
	cv::Mat GS_subtract_image(cv::Mat src_image1, cv::Mat src_image2 );

	// 명암 대비 조절 - 곱셈
	cv::Mat GS_multiple_constant(cv::Mat src_image, double scale);

	// 명암 대비 조절 - 나눗셈
	cv::Mat GS_divide_constant(cv::Mat src_image, double scale);

	// 밝기 조절 + 명암 대비 조절
	cv::Mat GS_basic_contrast_brightness(cv::Mat src_image, double contrast, int brightness );

	// 선형 결합
	cv::Mat GS_blending_effect(cv::Mat src_image1, cv::Mat src_image2, double alpha );

	// 명암도 영상의 히스토그램 생성.
	cv::Mat GS_imhist(cv::Mat img, int max_length = 256);

	// 히스토그램 평활화
	cv::Mat GS_histeq(cv::Mat src_image);

	// 히스토그램 데이터를 로그 창에 출력
	void GS_view_hist_data(cv::Mat src_image);

	// 룩업 테이블을 이용한 밝기 조절 + 명암 대비 조절
	cv::Mat GS_LUT_basic_contrast_brightness(cv::Mat src_image, double contrast, int brightness );

	// 주어진 조건에 의한 이진 영상 변환
	cv::Mat GS_threshold(cv::Mat src_image, double thresh, int threshold_type);
	cv::Mat GS_threshold(cv::Mat src_image, double thresh, double max_value, int threshold_type);

	// 적응적 경계값 설정에 의한 이진 영상 변환
	cv::Mat GS_adaptive_threshold(cv::Mat src_image, double max_value,
									 int adaptive_method, int threshold_type,
									 int block_size = 3, double C = 5);

	// 이진 영상에서의 논리 연산 - 논리 연산(logical operator) 사용
	cv::Mat GS_binary_logic(cv::Mat src_image1, cv::Mat src_image2, int logic_method);

	// 이진 영상을 명암도 영상으로 변환
	cv::Mat GS_binary2gray(cv::Mat src_image);

	// 명암도 영상에서의 논리 연산 - 비트 연산자(bitwise operator) 사용
	cv::Mat GS_gray_logic(cv::Mat src_image1, cv::Mat src_image2, int logic_method);
};

