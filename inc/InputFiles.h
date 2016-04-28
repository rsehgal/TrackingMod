/*
 * InputFiles.h
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */

#ifndef INC_INPUTFILES_H_
#define INC_INPUTFILES_H_

#include <string>
#include "base/Global.h"


namespace Tracking {

class InputFiles{

private:
	//Data corresponds to daqinfo file
	//std::string variablestring1, temperature, pressure, humidity;
	//std::string variablestring2, etaPartition, variablestring3, triggerLayer;

private:
	//File names
	std::string mappingFile;
	std::string daqInfoFile;
	std::string runFile;
	std::string offlineCfgFile;
	std::string iniFile;
	std::string rootFile;
	std::string etaPartitionFile;
	std::string largeAcceptanceFile;
	std::string smallAcceptanceFile;
	std::string twoRpcFile;

public:
    InputFiles();
    InputFiles(const std::string);
    InputFiles(const std::string,const std::string,const std::string,const std::string, const std::string,
        const std::string, const std::string, const std::string, const std::string, const std::string);
	TRACKING_INLINE
	void SetMappingFileName(std::string filename){mappingFile = filename;}
	TRACKING_INLINE
	void SetDaqInfoFileName(std::string filename){daqInfoFile = filename;}
	TRACKING_INLINE
	void SetRunFileName(std::string filename){runFile = filename;}
	TRACKING_INLINE
	void SetOfflineCfgFileName(std::string filename){offlineCfgFile = filename;}
	TRACKING_INLINE
	void SetInitializationFileName(std::string filename){iniFile = filename;}
	TRACKING_INLINE
	void SetRootFileName(std::string filename){rootFile = filename;}
	TRACKING_INLINE
	void SetEtaPartitionFile(std::string filename){etaPartitionFile = filename;}


	TRACKING_INLINE
	std::string GetMappingFileName(){return mappingFile;}
	TRACKING_INLINE
	std::string GetDaqInfoFileName(){return daqInfoFile;}
	TRACKING_INLINE
	std::string GetRunFileName(){return runFile;}
	TRACKING_INLINE
	std::string GetOfflineCfgFileName(){return offlineCfgFile;}
	TRACKING_INLINE
	std::string GetInitializationFileName(){return iniFile;}
	TRACKING_INLINE
	std::string GetRootFileName(){return rootFile;};
	TRACKING_INLINE
	std::string GetEtaPartitionFile(){return etaPartitionFile;}

	void SetLargeAcceptanceFile(std::string filename){largeAcceptanceFile = filename;}
	std::string GetLargeAcceptanceFile(){return largeAcceptanceFile;}

	void SetSmallAcceptanceFile(std::string filename){smallAcceptanceFile = filename;}
	std::string GetSmallAcceptanceFile(){return smallAcceptanceFile;}

	void SetTwoRpcFileName(std::string tworpc){twoRpcFile = tworpc;}
	std::string GetTwoRpcFileName(){return twoRpcFile;}


};//end of class

}

#endif /* INC_INPUTFILES_H_ */
