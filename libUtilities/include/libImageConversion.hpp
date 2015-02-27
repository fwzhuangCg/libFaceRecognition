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

FRsdk::Image decodeMemory(const char* bytearray, size_t length);

FRsdk::Image CVmatToFRsdkImage(cv::Mat image);

cv::Mat frsdkImageToMat(FRsdk::Image image);