/*
 * Pixel.cpp : Pixel Processing Class
 * Modified by Lee, Moon-Ho (conv2@korea.com), 2006/12/20
 * Additionally modified by Jung, Jin-Woo
 * Last Modified : 2015/03/27
 */

#include "Pixel.h"
#define cvQueryHistValue_1D( hist, idx0 ) cvGetReal1D((hist)->bins, (idx0))


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPixel::CPixel()
{

}

CPixel::~CPixel()
{

}

// Logical operation with two gray-scale images - bitwise operator
// Input: two 8bit gray-scale images + bitwise operator (AND 0, NAND 1, OR 2, NOR 3, XOR 4, Difference 5)
cv::Mat CPixel::GS_gray_logic(cv::Mat src_image1, cv::Mat src_image2, int logic_method)
{
	// Initialization
	cv::Mat dst_image = cv::Mat(src_image1.size(), src_image1.type());
	cv::Mat tmp_image = dst_image.clone();

	switch (logic_method)
	{
		// AND
	case 0:
		// var = (var1 & var2); 
		cv::bitwise_and(src_image1, src_image2, dst_image);
		break;
		// NAND
	case 1:
		//var = ~(var1 & var2); 
		cv::bitwise_and(src_image1, src_image2, tmp_image);
		cv::bitwise_not(tmp_image, dst_image);
		break;
		// OR
	case 2:
		//var = (var1 | var2);
		cv::bitwise_or(src_image1, src_image2, dst_image);
		break;
		// NOR
	case 3:
		//var = ~(var1 | var2);
		cv::bitwise_or(src_image1, src_image2, tmp_image, NULL);
		cv::bitwise_not(tmp_image, dst_image);
		break;
		// XOR
	case 4:
		//var = var1 ^ var2;
		cv::bitwise_xor(src_image1, src_image2, dst_image, NULL);
		break;
		// Difference
	case 5:
		//var = var & (~var2);
		cv::bitwise_not(src_image2, tmp_image);
		cv::bitwise_and(src_image1, tmp_image, dst_image, NULL);
		break;
	default:
		break;
	} // end switch

	if (!tmp_image.empty()) tmp_image.release();

	return dst_image;
}

// Logical operation with two binary images - logical operator
// Input: two binary images + logical operator (AND 0, NAND 1, OR 2, NOR 3, XOR 4, XNOR 5)
cv::Mat CPixel::GS_binary_logic(cv::Mat src_image1, cv::Mat src_image2, int logic_method)
{
	int i, j;
	BYTE var, var1, var2;

	// 초기화
	cv::Mat dst_image = cv::Mat(src_image1.size(), src_image1.type());

	int height = src_image1.rows;
	int width = src_image1.cols;

	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			var1 = ((BYTE)src_image1.at<uchar>(i, j) == 0 ? 1 : 0);
			var2 = ((BYTE)src_image2.at<uchar>(i, j) == 0 ? 1 : 0);

			switch (logic_method)
			{
				// AND
			case 0:
				var = (var1 && var2);
				break;
				// NAND
			case 1:
				var = !(var1 && var2);
				break;
				// OR
			case 2:
				var = (var1 || var2);
				break;
				// NOR
			case 3:
				var = !(var1 || var2);
				break;
				// XOR
			case 4:
				var = (var1 != var2 ? 1 : 0);
				break;
				// XNOR
			case 5:
				var = (var1 == var2 ? 1 : 0);
				break;
			default:
				break;
			} // end switch

			if(var == 0.0)
				dst_image.at<uchar>(i, j) = 1.0;
			else
				dst_image.at<uchar>(i, j) = 0.0;

		} // end for
	} // end for

	return dst_image;
}

// Convert binary image to gray-scale image
// Input: binary image
cv::Mat CPixel::GS_binary2gray(cv::Mat src_image)
{
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type());
	
	// Create an image with all 1 pixel values 
	cv::Mat tmp_image = cv::Mat(src_image.size(), src_image.type(), cv::Scalar(1));
	
	// multiplication : dst(I)=contrast*src1(I)*src2(I)
	cv::multiply(src_image, tmp_image, dst_image, 255.0);
	
	return dst_image;
}


// Binarization by adaptive thresholding
/* Input: 
src – Source 8-bit single-channel image.
dst – Destination image of the same size and the same type as src .
maxValue – Non-zero value assigned to the pixels for which the condition is satisfied. 
adaptive_method – Adaptive thresholding algorithm to use, ADAPTIVE_THRESH_MEAN_C or ADAPTIVE_THRESH_GAUSSIAN_C.
thresholdType – Thresholding type that must be either THRESH_BINARY or THRESH_BINARY_INV. Check ThresholdTypes
blockSize – Size of a pixel (default 5)
C - Constant subtracted from the mean or weighted mean (default 5)
*/
cv::Mat CPixel::GS_adaptive_threshold(cv::Mat src_image, double max_value, int adaptive_method, int threshold_type, int block_size, double C)
{
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type());

	cv::adaptiveThreshold(src_image, dst_image, max_value,
					  adaptive_method, threshold_type,
					  block_size, C);

	return dst_image;
}

// Binarization by thresholding
/* Input: 
src – Source 8-bit single-channel image.
dst – Destination image of the same size and the same type as src .
maxValue – Non-zero value assigned to the pixels for which the condition is satisfied. 
thresh - Thr eshold value to be checked 
thresholdType – Thresholding type that must be either THRESH_BINARY or THRESH_BINARY_INV. Check ThresholdTypes
*/
cv::Mat CPixel::GS_threshold(cv::Mat src_image, double thresh, int threshold_type)
{
	double max_value = 255.0;

	if (threshold_type == cv::THRESH_BINARY || threshold_type == cv::THRESH_BINARY_INV)
		max_value = thresh;

	return GS_threshold(src_image, thresh, max_value, threshold_type);
}
cv::Mat CPixel::GS_threshold(cv::Mat src_image, double thresh, double max_value, int threshold_type)
{
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type());

	cv::threshold(src_image, dst_image, thresh, max_value, threshold_type);
    
	return dst_image;
}


// Contrast & brightness adjustment by LUT
// Input: 8 bit gray-scale image
cv::Mat CPixel::GS_LUT_basic_contrast_brightness(cv::Mat src_image, double contrast,
						  					        int brightness )
{
	int i, var;

	cv::Mat dst_image;

	// Create a LUT  
	cv::Mat LUT(1, 256, CV_8UC1);

	for(i=0; i<256; i++)
	{
		var = (int)( i*contrast + brightness );
		
		// Clamping for the out-range data 
		if (var > 255) var = 255;
		else if (var < 0 ) var = 0;

		LUT.data[i] = (unsigned char)var;
	}

	LUT.data[0] = 0;

	// Types for src_image, dst_image, and lut should be ALL SAME
	//		  CV_8UC1 : 8bit(integer value), CV_32FC1 : 32bit (real value)
	cv::LUT(src_image, LUT, dst_image);

	return dst_image; 
}

// Histogram Equalization
// Input: 8 bit gray-scale image
cv::Mat CPixel::GS_histeq(cv::Mat src_image)
{
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type());

	cv::equalizeHist(src_image, dst_image);

	return dst_image;
}

void CPixel::GS_view_hist_data(cv::Mat src_image)
{
	std::cout << "[" << std::endl;
	for (int i = 0; i < src_image.rows; i++) {
		for (int j = 0; j < src_image.cols; j++) {
			std::cout << src_image.at<uchar>(i, j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
}

// Calculate Histogram and create Histogram Image
// Input : 8-bit Grayscale Image
cv::Mat CPixel::GS_imhist(cv::Mat img, int max_length)
{	
	int hist_height = 128; int hist_width = 256;


	int hist_bar_height = 15;
	int tmp = 0;

	int bin_size = 256;
	int histSize[] = { bin_size };
	float range[] = { 0, 255 };
	int channels[] = { 0 };
	const float* ranges[] = { range };

	// Calculate Histogram
	cv::Mat hist;
	cv::calcHist(&img, 1, channels, cv::Mat(), hist, 1, histSize, ranges);


	cv::Mat hist_img = cv::Mat(hist_height, hist_width, CV_8UC1, cv::Scalar(0));
	cv::Mat hist_img_bar = cv::Mat(hist_bar_height, hist_width, CV_8UC1, cv::Scalar(0));

	// get min and max value from histogram
	double maxVal, minVal;
	cv::minMaxIdx(hist, &minVal, &maxVal);

	// set Histogram Height
	int tmpHistHeight = round(maxVal);
	if (max_length > 0) tmpHistHeight = max_length;

	cv::Mat tmp_hist_imgA = cv::Mat(cv::Size(hist_width, tmpHistHeight), CV_8UC1, cv::Scalar(0));
	cv::Mat tmp_hist_imgB = cv::Mat(cv::Size(hist_width, hist_height), CV_8UC1);

	// normalize histogram
	cv::normalize(hist, hist, 0, tmp_hist_imgA.rows, cv::NORM_MINMAX, -1, cv::Mat());

	// draw histogram image
	for (int i = 0; i < tmp_hist_imgA.cols; i++)
	{
		tmp = tmpHistHeight - round(hist.at<float>(i));

		if (tmp == 0 || tmp > tmpHistHeight) continue;

		for (int j = tmp - 1; j >= 0; j--) {
			tmp_hist_imgA.at<uchar>(j, i) = 192;
		}
	}

	// resize temp histogram image to [hist_height * hist_width]
	cv::resize(tmp_hist_imgA, tmp_hist_imgB, tmp_hist_imgB.size(), 0.0, 0.0, cv::INTER_CUBIC);
	tmp_hist_imgB.copyTo(hist_img);

	// draw color bar
	float hist_bar_width = (float)hist_img.cols / hist.rows;
	for (int i = 0; i < hist_img_bar.cols; i++)
	{
		for (int j = 0; j < hist_img_bar.rows; j++)
		{
			hist_img_bar.at<uchar>(j,i) = i;
		}
	}

	// attach color bar to histogram image
	hist_img.push_back(hist_img_bar);

	hist.release();
	hist_img_bar.release();

	return hist_img;
}

// Contrast & brightness adjustment (multiplication & addition)
// Input: 8 bit gray-scale image
cv::Mat CPixel::GS_basic_contrast_brightness(cv::Mat src_image, double contrast, int brightness)
{
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type());

	//-------------------------------------
	// STEP 1 : Multiplication for contrast
	//-------------------------------------
	cv::Mat tmp_image = cv::Mat(src_image.size(), src_image.type(), cv::Scalar(1));

	// dst(I)=contrast*src1(I)*src2(I)
	cv::multiply(src_image, tmp_image, dst_image, contrast);

	//-------------------------------
	// STEP 2 : Addition for brightness
	//-------------------------------

	cv::Scalar value; value.all(brightness);
	dst_image.copyTo(tmp_image); // copy
	dst_image.zeros(dst_image.size(), dst_image.type());	// initialization by 0

	cv::add(tmp_image, value, dst_image);						// addition

	if (!tmp_image.empty()) tmp_image.release();

	return dst_image;
}

// Contrast adjustment (multiplication)
// Input: 8 bit gray-scale image
cv::Mat CPixel::GS_multiple_constant(cv::Mat src_image, double scale)
{
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type());

	cv::Mat tmp_image = cv::Mat(src_image.size(), src_image.type(), cv::Scalar(1));

	// dst(I)=scale*src1(I)*src2(I)
	cv::multiply(src_image, tmp_image, dst_image, scale);

	if (!tmp_image.empty()) tmp_image.release();

	return dst_image;
}

// Contrast adjustment (division)
// Input: 8 bit gray-scale image
cv::Mat CPixel::GS_divide_constant(cv::Mat src_image, double scale)
{
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type());

	cv::Mat tmp_image = cv::Mat(src_image.size(), src_image.type(), cv::Scalar(1));

	// dst(I)=(1/scale)*src1(I)*src2(I)
	// (ref) Here, cvDiv() cannot be used because, in cvDiv(),
	//        dst(I)=scale?src1(I)/src2(I), if src1!=NULL
	//        dst(I)=scale/src2(I),         if src1=NULL

	cv::multiply(src_image, tmp_image, dst_image, 1.0 / scale);

	if (!tmp_image.empty()) tmp_image.release();

	return dst_image;
}

// Blending 
// Input: any two images
cv::Mat CPixel::GS_blending_effect(cv::Mat src_image1, cv::Mat src_image2, double alpha)
{
	cv::Mat dst_image;
	double beta = 1.0 - alpha;
	double gamma = 0.0;

	cv::addWeighted(src_image1, alpha, src_image2, beta, gamma, dst_image);

	return dst_image;
}

// addition with two images
// Input: any two images
cv::Mat CPixel::GS_add_image(cv::Mat src_image1, cv::Mat src_image2 )
{
	cv::Mat dst_image;
	cv::add(src_image1, src_image2, dst_image);

	return dst_image;
}

// subtraction with two images
// Input: any two images
cv::Mat CPixel::GS_subtract_image(cv::Mat src_image1, cv::Mat src_image2 )
{
	cv::Mat dst_image;

	cv::subtract(src_image1, src_image2, dst_image);

	return dst_image;
}

// Brightness adjustment (constant addition)
// Input: any image
cv::Mat CPixel::GS_add_constant(cv::Mat src_image, int constant)
{
	cv::Mat dst_image = src_image.clone();
	dst_image.zeros(dst_image.size(), dst_image.type());

	
	cv::Scalar value;
	value.all(fabs((double)constant));

	if (constant >= 0)
		cv::add(src_image, value, dst_image);
	else
		cv::subtract(src_image, value, dst_image);

	return dst_image;
}

// Make band image 
cv::Mat CPixel::GS_makeGrayBand()
{
	int height = 64, width = 256;
	cv::Mat band_image = cv::Mat(cv::Size(width, height), CV_8UC1);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			band_image.at<uchar>(i, j) = j;
		}
	}

	return band_image;
}

// Make contrast image
cv::Mat CPixel::GS_makeContrast()
{
	int height = 100, width = 256;
	cv::Mat contrast_image = cv::Mat(cv::Size(width, height), CV_8UC1);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
		{
			contrast_image.at<uchar>(i, j) = 0;
		}
		for (int j = width / 2; j < width; j++)
		{
			contrast_image.at<uchar>(i, j) = 255;
		}
	}

	return contrast_image;
}
