/*
 * Calibration.h
 *
 *  Created on: 21-Nov-2020
 *      Author: rsehgal
 */

#ifndef CRY_MANUAL_INTERFACE_MUONREADER_H_
#define CRY_MANUAL_INTERFACE_MUONREADER_H_

#include <TF1.h>
#include <TH1F.h>
#include <TTree.h>
#include <TFile.h>
#include <TApplication.h>
#include <TVector3.h>
#include <string>
#include <cstring>
#include "cry_manual_interface.h"

namespace lite_interface{

class MuonReader {

	Muon *fMuon; 

	/*
	 * To be used only by the code
	 */
	std::string fFileName;
	TFile *fMuonFile;
	TTree *fMuonTree;
	static Long64_t fEntryCount;
	static MuonReader *s_instance;
public:
	MuonReader();
	static MuonReader *instance();
	static MuonReader *instance(std::string filename);
	MuonReader(std::string fileName);
	virtual ~MuonReader();

	/*
	 * Function to return the Muon
	 */
	Muon* GetMuon();
};

} /* End of lite_interface */
#endif /* CRY_MANUAL_INTERFACE_MUONREADER_H_ */
