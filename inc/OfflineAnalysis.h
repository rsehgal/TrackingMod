/*
 * OfflineAnalysis.h
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */

#ifndef INC_OFFLINEANALYSIS_H_
#define INC_OFFLINEANALYSIS_H_

#include "InputFiles.h"
#include "base/Global.h"
#include <vector>
#include <algorithm>
#include <string>
#include "Hodoscope.h"
#include "InputFiles.h"
#include "TDC.h"
#include "Scintillator.h"

//#include "RPC.h"

class TBranch;

typedef std::vector< std::vector< unsigned int> * > ModuleVector;
typedef std::vector< TBranch* > ModuleBranches; //STL vector of TBranch of a TTree
const unsigned ChannelsCount = 128;

namespace Tracking {

class OfflineAnalysis {
private:

  //Making all data members public for the time being
public:

	//Hodoscope h;
	InputFiles inpfiles;
	int run;
	std::string run_config_file;
	//Variable to store data from "daqinfo" file
	std::string variablestring1, temperature, pressure, humidity;
    std::string variablestring2, etaPartition, variablestring3, Trigger_layer;

    unsigned fModulesCount;
    std::vector<ModuleVector> fData;
    std::vector<ModuleBranches> fBranches;
    ModuleVector HitsPerChannel; //Vector< Vector<hits per channel> >

    std::vector<TDC> fTdcVector;
    std::vector<RPC> fRpcVector; //Vector of RPCs
    std::vector<ScintillatorPlane*> fScintPlaneVector;

    double timeWindow;
    int triggerBit;
    int triggerBitEta;
    bool largeAcceptance;
    bool smallAcceptance;
    bool twoTrackingRpc;

    //Reference Chamber Conf
    int refStation;
    int refStation_t2;

    //Variable for Mapping.txt
    int map_station;
    int map_module;
    int map_start;
    int map_finish;
    int map_module1;
    int map_start1;
    int map_finish1;


private:
    //Some verbose variables
    bool fVerbose_trg;


public:

/*
	OfflineAnalysis(std::string filename) {
		inpfiles.SetRootFileName(filename);
		ReadFilesAndFillData();
	}
*/
    int GetTriggerBit(){return triggerBit;}
    int GetTriggerBitEta(){return triggerBitEta;}
    std::string GetEtaPartition(){return etaPartition;}

    double GetTimeWindow(){return timeWindow;}

    void SetTimeWindow(double timewindow){timeWindow=timewindow;}
    void SetTriggerBit(int triggerbit){triggerBit = triggerbit;}

    OfflineAnalysis();
	OfflineAnalysis(std::string filename, unsigned modulesCount=6);
	~OfflineAnalysis();
	void Init();
	void InitializeTree(char *);
	void ReadInifileAndFillModuleInfo();
	void ReadTriggerBit();
	void ReadTriggerBitEta();
	void ReadEtaPartition();
	void ReadLargeAcceptance();
	void ReadSmallAcceptance();
	void ReadTwoRpcFile();

	void ReadFilesAndFillData();
	void ReadInitializationFile();
	void StripProfile();
	void GenerateTracks();

	void PrintTriggerInfo();
	void ReadReferenceChamberInfo();

	void ReadMappingInfo();
	void ReadScintMapFileAndCreatePlane();




}; // end of OfflineAnalysis class


} // end of Tracking namespace

#endif /* INC_OFFLINEANALYSIS_H_ */
