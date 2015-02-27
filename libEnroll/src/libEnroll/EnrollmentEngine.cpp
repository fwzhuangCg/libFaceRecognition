#ifdef WIN32
#define BOOST_ALL_DYN_LINK // Link against the dynamic boost lib. Seems to be necessary because we use /MD, i.e. link to the dynamic CRT.	
#define BOOST_ALL_NO_LIB // Don't use the automatic library linking by boost with VS2010 (#pragma ...). Instead, we specify everything in cmake.
#endif

#include <frsdk/config.h>
#include <frsdk/image.h>   //TODO MG
#include <frsdk/enroll.h>
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//custom libs
#include "libEnroll/edialog.h" //only include this lib in .cpp files, it contains definitions, not just declarations -> crazy linker error 
#include "libLogging/LoggerFactory.hpp"
#include "libEnroll/EnrollmentEngine.hpp"
#include "libUtilities/include/libImageConversion.hpp"

using namespace std;
using logging::Logger;
using logging::LoggerFactory;
using logging::LogLevel;
using namespace boost::filesystem;



//constructor
EnrollmentEngine::EnrollmentEngine(path configFile) {
	cfg = std::unique_ptr<FRsdk::Configuration>(new FRsdk::Configuration(configFile.string()));
	//init logging
	LogLevel logLevel = LogLevel::Info;
	Loggers->getLogger("Enroll").addAppender(make_shared<logging::ConsoleAppender>(logLevel));
	Logger sessionLogger = Loggers->getLogger("Enroll");
}

FRsdk::SampleSet EnrollmentEngine::loadSampleSet(vector<path> imagePaths) {
	Logger sessionLogger = Loggers->getLogger("Enroll");
	FRsdk::SampleSet sampleSet;
	for (auto& imagePath : imagePaths) {
		try {
			FRsdk::Image img(FRsdk::ImageIO::load(imagePath.string()));
			sampleSet.push_back(FRsdk::Sample(img));
			return sampleSet;
		}
		catch (exception& e) {
			sessionLogger.info("Error in loadSampleSet - is file really an image?");
			sessionLogger.info(e.what());
		}
	}
}

void EnrollmentEngine::enrollSampleSet(FRsdk::SampleSet sampleSet, path firPath) {
	Logger sessionLogger = Loggers->getLogger("Enroll");
	FRsdk::Enrollment::Processor processor(*cfg.get());
	try {
		// create the needed interaction instances
		FRsdk::Enrollment::Feedback feedback(new EnrolCoutFeedback(firPath.string()));
		// do the enrollment    
		processor.process(sampleSet.begin(), sampleSet.end(), feedback);
	}
	catch (exception& e) {
		sessionLogger.info("Error in enrollSampleSet");
		sessionLogger.info(e.what());
	}
}

void EnrollmentEngine::enrollWholeImageFolder(path imageFolder, path outputFolder) {
	string firName;
	path firPath;
	FRsdk::SampleSet sampleSet;
	//gather all filenames from folder
	vector <path> imagePaths;
	copy(directory_iterator(imageFolder), directory_iterator(), back_inserter(imagePaths));
	for (auto& imagePath : imagePaths) {
		sampleSet = loadSampleSet(vector<path>(1, imagePath));
		firName = imagePath.filename().replace_extension("fir").string();
		firPath = outputFolder / firName;
		enrollSampleSet(sampleSet, firPath);
	}
}

void EnrollmentEngine::enrollSingleImage(path imagePath, path outputFolder) {
	FRsdk::SampleSet sampleSet = loadSampleSet(vector<path>(1, imagePath));
	string firName = imagePath.filename().replace_extension("fir").string();
	path firPath = outputFolder / firName;
	enrollSampleSet(sampleSet, firPath);
}

FRsdk::FIR EnrollmentEngine::enrollOpenCVMat(cv::Mat cvImage, path firPath) {
	//convert image to FaceVACS format
	FRsdk::Image fvImage = CVmatToFRsdkImage(cvImage);
	//enroll
	try {
		FRsdk::SampleSet sampleSet;
		sampleSet.push_back(FRsdk::Sample(fvImage));
		enrollSampleSet(sampleSet, firPath);
	}
	catch (exception& e) {
		Logger sessionLogger = Loggers->getLogger("Enroll");
		sessionLogger.info("Error in enrollment - is there a face?");
		sessionLogger.info(e.what());
	}
}

void EnrollmentEngine::enrollMultipleImages(vector <path> imagePaths, path outputFolder, string firName) {
	FRsdk::SampleSet sampleSet = loadSampleSet(imagePaths);
	path firPath = outputFolder / firName;
	enrollSampleSet(sampleSet, firPath);
}

