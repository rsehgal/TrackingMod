/*
 * ReadCalibrationDataFile.h
 *
 *  Created on: 09-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_READCALIBRATIONDATAFILE_H_
#define ISMRAN_ANALYSIS_INC_READCALIBRATIONDATAFILE_H_

#include <vector>
#include <string>
class TFile;

struct FileNameStruct{


	std::string sSourceType;
	std::string sBarName;
	std::string sMode;
	int sNearVoltage;
	int sNearTh;
	int sFarVoltage;
	int sFarTh;
	int sSourcePos;

	FileNameStruct(std::string sourceType, std::string barName, std::string mode
					, int vNear, int thNear, int vFar, int thFar, int sourcePos){

		sSourceType = sourceType;
		sBarName = barName;
		sMode = mode;
		sNearVoltage = vNear;
		sNearTh = thNear;
		sFarVoltage = vFar;
		sFarTh = thFar;
		sSourcePos = sourcePos;
	}

	FileNameStruct(std::string sourceType){
		sSourceType = sourceType;
	}

	FileNameStruct(){

	}

};

class ReadCalibrationDataFile {
//Keeping public for the time being
public:
	std::vector<std::string> fDatafileNamesVec;
	TFile *fout;
public:
	ReadCalibrationDataFile();
	ReadCalibrationDataFile(std::string dummy);
	virtual ~ReadCalibrationDataFile();
	double CalculateBarDelTOffsetOf(unsigned int barIndex);
	double CalculateBarDelTOffsetOf(std::string datafileName,unsigned int barIndex, int sourcePos);
	void CalculateParameterizationOf(unsigned int barIndex);
	void DoParameterization();
	void ReadDataFile();
	void ReadDataFile_V2();

	//V2 of required functions
	void DoParameterization_V2(std::string dirname);
	void CalculateParameterizationOnVector(std::vector<std::string> filenameVector);
	std::vector<double> fDelTMeanVector;
	std::vector<double> fPosVector;
	double offsett;
	std::string dirName;
	std::string currentBarName;
	void ProcessFile(std::string filename);
	FileNameStruct* GetFileNameStruct(std::string filename);

};

#endif /* ISMRAN_ANALYSIS_INC_READCALIBRATIONDATAFILE_H_ */
