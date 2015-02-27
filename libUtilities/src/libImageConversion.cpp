/* Conversion of CV::Mat to FRsdk::Image and match the FIR files
* original: Patrik's libFaceRecognition in FeatureDetection
* @author Patrik Huber, Michael Grupp
*/

#include <iostream>
#include <frsdk/face.h>
#include <frsdk/image.h>
#include <frsdk/jpeg.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

FRsdk::Image decodeMemory(const char* bytearray, size_t length)
{
	// if a std:string (used for memory management) is created from a byte array
	// and with the additional input of the length of the array the complete
	// array is used as a string and will not end at the first '\0'
	string strfromarray(bytearray, length);
	// create a std::stream from std::string
	std::stringstream memstream(strfromarray);
	// use FRsdk::Jpeg::load( std::istream&, ...) in order to read from
	// the stringstream
	FRsdk::Image img = FRsdk::Jpeg::load(memstream);
	return img;
}

FRsdk::Image CVmatToFRsdkImage(cv::Mat image)
{
	vector<uchar> jpegBuffer;
	vector<int> jpegParams = { cv::IMWRITE_JPEG_QUALITY, 100 }; // default is 95
	cv::imencode(".jpg", image, jpegBuffer, jpegParams);
	FRsdk::Image frsdkImage = decodeMemory(reinterpret_cast<char*>(&jpegBuffer[0]), jpegBuffer.size());
	return frsdkImage;
}

cv::Mat frsdkImageToMat(FRsdk::Image image)
{
	cv::Mat outImage;
	// FRsdk::Image uses uchar
	if (image.isColor()) {
		// FRsdk::Image data is BGRA
		// Danger: We're casting away the const-ness of the FRsdk::Image data here. It might be preferable to copy the data first, but we leave this to the user.
		outImage = cv::Mat(image.height(), image.width(), CV_8UC4, reinterpret_cast<void*>(const_cast<FRsdk::Rgb*>(image.colorRepresentation())));
	}
	else {
		outImage = cv::Mat(image.height(), image.width(), CV_8UC1, reinterpret_cast<void*>(const_cast<FRsdk::Byte*>(image.grayScaleRepresentation())));
	}
	return outImage;
}