#ifdef WIN32
	#define BOOST_ALL_DYN_LINK // Link against the dynamic boost lib. Seems to be necessary because we use /MD, i.e. link to the dynamic CRT.
	#define BOOST_ALL_NO_LIB // Don't use the automatic library linking by boost with VS2010 (#pragma ...). Instead, we specify everything in cmake.
#endif

#include <iostream>
#include <frsdk/config.h>
#include <frsdk/match.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//custom libs
#include "libLogging/LoggerFactory.hpp"
#include "libMatching/MatchingEngine.hpp"

using namespace std;
using namespace logging;
using logging::Logger;
using logging::LoggerFactory;
using logging::LogLevel;
using namespace boost::filesystem;


MatchingEngine::MatchingEngine(path configFile, path logfile) {
	//init logging
	LogLevel logLevel = LogLevel::Info;
	path logfilePath = logfile;
	Loggers->getLogger("matching").addAppender(make_shared<logging::FileAppender>(logLevel, logfilePath.string()));
	Logger matchingLogger = Loggers->getLogger("matching");
	
	//init FaceVACS
	cfg = std::unique_ptr<FRsdk::Configuration>(new FRsdk::Configuration(configFile.string()));
	firBuilder = std::unique_ptr<FRsdk::FIRBuilder>(new FRsdk::FIRBuilder(*cfg.get()));
	me = std::unique_ptr<FRsdk::FacialMatchingEngine>(new FRsdk::FacialMatchingEngine(*cfg.get()));
	matchingLogger.info("initialized MatchingEngine");
}


FRsdk::FIR MatchingEngine::loadFIR(path fir){
	if (boost::filesystem::is_directory(fir.parent_path()) || !boost::filesystem::is_empty(fir.parent_path())) {
		//build fir
		ifstream firIn(fir.string(), ios::in | ios::binary);
		return firBuilder->build(firIn);
	}
	else {
		Logger matchingLogger = Loggers->getLogger("matching");
		matchingLogger.info("Error at loadFIR: specified folder is empty or doesn't exist");
		throw exception();
	}
}


FRsdk::Population MatchingEngine::createPopulation(path firFolder){
	//get the paths of the .fir files in the folder an create the poulation out of them
	vector<path> firPaths;
	if (boost::filesystem::is_directory(firFolder) || !boost::filesystem::is_empty(firFolder)) {
		copy(directory_iterator(firFolder), directory_iterator(), back_inserter(firPaths));
		FRsdk::Population population(*cfg.get());
		string galleryName;
		for (auto& path : firPaths) {
			galleryName = path.filename().string();
			//build fir population
			ifstream fir(path.string(), ios::in | ios::binary);
			population.append(firBuilder->build(fir), galleryName);
		}
		return population;
	}
	else {
		Logger matchingLogger = Loggers->getLogger("matching");
		matchingLogger.info("Error at createPopulation: specified folder is empty or doesn't exist");
		throw exception();
	}
}


float MatchingEngine::matchPair(path probeFIRpath, path galleryFIRpath) {
	FRsdk::FIR probeFIR = loadFIR(probeFIRpath);
	FRsdk::FIR galleryFIR = loadFIR(galleryFIRpath);
	return me->compare(probeFIR, galleryFIR);
}


vector<float>  MatchingEngine::matchToPopulation(FRsdk::FIR probe, const FRsdk::Population& galleryPopulation){
	FRsdk::CountedPtr<FRsdk::Scores> scores;
	scores = me->compare(probe, galleryPopulation);
	//convert CountedPtr to std::vector<float> because FRsdk::CountedPtr is no STL container 
	vector<float> scoreVector;
	for (FRsdk::Scores::const_iterator scoreIter = scores->begin(); scoreIter != scores->end(); scoreIter++){
		scoreVector.push_back((float)*scoreIter);
	}
	return scoreVector;
}


float matchPairCV(cv::Mat, path galleryFIRpath) {

	//TODO

}


void MatchingEngine::matchExperiment(path probeFolder, path galleryFolder, path outputFolder) {
	Logger matchingLogger = Loggers->getLogger("matching");
	
	if (boost::filesystem::create_directory(outputFolder))
		matchingLogger.info("Output directory was created successfully");
	else
		matchingLogger.info("Output directory could not be created");
	
	
	FRsdk::CountedPtr<FRsdk::Scores> scores;
	string galleryName;
	path scorefilePath;
	ofstream scorefile;
	//ifstream firIn;
	
	//gather FIR paths from the folders
	vector<path> probePaths, galleryPaths;
	vector<string> galleryNames;
	copy(directory_iterator(probeFolder), directory_iterator(), back_inserter(probePaths));
	copy(directory_iterator(galleryFolder), directory_iterator(), back_inserter(galleryPaths));
	
	//all probes will be matched to this population...
	FRsdk::Population galleryPopulation(*cfg.get());
	for (auto& path : galleryPaths) {
		galleryName = path.filename().string();
		//population and names are in same order
        ifstream fir (path.string(), ios::in | ios::binary);
        galleryPopulation.append(firBuilder->build(fir), galleryName);
		galleryNames.push_back(galleryName);
	}

	matchingLogger.info("Starting to match two folders for scorefiles");
	matchingLogger.info("# of Probes:"+std::to_string(probePaths.size()));
	matchingLogger.info("# of Galleries:" + std::to_string(galleryPaths.size()));
	
	//match and make scorefile for each probe
	for (auto& path : probePaths) {
		//open probe fir
		ifstream firIn(path.string(), ios::in | ios::binary);
		FRsdk::FIR fir = firBuilder->build(firIn);
		//match to gallery population
		scores = me->compare(fir, galleryPopulation);
		string scoreFileName = path.filename().replace_extension("txt").string();//scorefile is txt
		scorefilePath = outputFolder / scoreFileName;
		FRsdk::Scores::const_iterator scoreIter = scores->begin();
		//write scorefile (names and scores must be in same order)
		scorefile.open(scorefilePath.string(), ios::out | ios::trunc);
		for (int nameIter=0; scoreIter != scores->end(); nameIter++) {
			//row of scorefile: galleryName score
			scorefile << galleryNames[nameIter] << " " << (float)*scoreIter << endl;
			scoreIter++;
		}
		scorefile.close();
		firIn.close();
	}
	matchingLogger.info("Finished");
}
