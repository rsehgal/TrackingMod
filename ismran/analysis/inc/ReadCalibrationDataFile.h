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

class ReadCalibrationDataFile {
//Keeping public for the time being
public:
	std::vector<std::string> fDatafileNamesVec;
	TFile *fout;
public:
	ReadCalibrationDataFile();
	virtual ~ReadCalibrationDataFile();
	double CalculateBarDelTOffsetOf(unsigned int barIndex);
	double CalculateBarDelTOffsetOf(std::string datafileName,unsigned int barIndex, int sourcePos);
	void CalculateParameterizationOf(unsigned int barIndex);
	void DoParameterization();
	void ReadDataFile();
};

#endif /* ISMRAN_ANALYSIS_INC_READCALIBRATIONDATAFILE_H_ */
