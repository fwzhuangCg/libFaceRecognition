/* A class providing easy to use functions for common tasks with FaceVACS matching
*
* NOTE: uses C++ 11 constructs, configure g++ compiler to support this
* by appending "-std = c++0x" or c++11 to compiler flags
*
* @author Michael Grupp
*/

#ifdef WIN32
#define BOOST_ALL_DYN_LINK // Link against the dynamic boost lib. Seems to be necessary because we use /MD, i.e. link to the dynamic CRT.
#define BOOST_ALL_NO_LIB // Don't use the automatic library linking by boost with VS2010 (#pragma ...). Instead, we specify everything in cmake.
#endif

#ifndef _ENROLLMENTENGINE_
#define _ENROLLMENTENGINE_

#include <frsdk/config.h>
#include <frsdk/image.h>  //TODO MG
#include <frsdk/enroll.h>
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//custom libs
#include "libLogging/LoggerFactory.hpp"

using namespace std;
using logging::Logger;
using logging::LoggerFactory;
using logging::LogLevel;
using namespace boost::filesystem;



class EnrollmentEngine {
private:
	std::shared_ptr<FRsdk::Configuration> cfg;
	std::unique_ptr<FRsdk::FIRBuilder> firBuilder;

protected:

public:
	/* the constructor for the class - loads the configuration, which contains settings for enrollment and licensing informations
	 * @param	the path of the FaceVACS config file (frsdk.cfg)
	*/
	EnrollmentEngine(path configFile);


	//destructor
	~EnrollmentEngine() {
		Loggers->getLogger("Enroll").info("closing EnrollmentEngine");
	}

	/* loads one or multiple images into a SampleSet. A SampleSet is a container for enrollment images.
	 * @param	the paths of the images, that are going to be loaded
	 * @return	the SampleSet of the images
	*/
	FRsdk::SampleSet loadSampleSet(vector<path> imagePaths);

	/* this function performs the actual enrollment. Every image of the SampleSet is processed,
	 * and faces are enrolled into a .fir-file, if they're found.
	 * @param	the SampleSet containing the images
	 * @param	the path of the .fir-file that is going to be produced
	*/
	void enrollSampleSet(FRsdk::SampleSet sampleSet, path firPath);


	/* enrolls a folder of images. One .fir-file per image is produced. 
	 * This function was used for the experiment framework of the thesis.
	 * @param	the path of the folder containing the images
	 * @param	the path of the folder, in which the .fir-files are going to be stored
	*/
	void enrollWholeImageFolder(path imageFolder, path outputFolder);


	/* enrolls a single image. One .fir-file is produced.
	 * @param	the path of the image
	 * @param	the path of the folder for the .fir-file, that is going to be produced
	*/
	void enrollSingleImage(path imagePath, path outputFolder);


	/* enrolls a single image from OpenCV
	* @param	the path of the image
	* @return	the path of the folder for the .fir-file, that is going to be produced
	*/
	FRsdk::FIR enrollOpenCVMat(cv::Mat image, path firPath);


	/* enrolls multiple images. One .fir-file is produced for all faces that are found.
	 * @param	the paths of the images
	 * @param	path of the folder, where the .fir-file is going to be stored
	 * @param	the name of the .fir-file, that is going to be produced
	*/
	void enrollMultipleImages(vector<path> imagePaths, path outputFolder, string firName);

};



#endif
