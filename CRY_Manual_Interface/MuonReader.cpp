/*
 * Calibration.cpp
 *
 *  Created on: 21-Nov-2020
 *      Author: rsehgal
 */

#include "MuonReader.h"
#include "TFile.h"
#include "PsBar.h"
#include "includes.hh"
#include "HardwareNomenclature.h"
#include <TH1D.h>
#include "cry_manual_interface.h"

namespace lite_interface{

MuonReader *MuonReader::s_instance = 0;
Long64_t MuonReader::fEntryCount = 0;

MuonReader::MuonReader() {
	// TODO Auto-generated constructor stub

}

MuonReader::~MuonReader() {
	// TODO Auto-generated destructor stub
}

MuonReader* MuonReader::instance(std::string filename) {
        if (!s_instance)
          s_instance = new MuonReader(filename);
        return s_instance;
}

MuonReader* MuonReader::instance() {
        if (s_instance)
          return s_instance;
        else{
        	std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
        	std::cout<<"@@ Instance of Calibration class does not exist in Memory, please create it first....@@" << std::endl;
        	std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
        }
}

MuonReader::MuonReader(std::string fileName) {
	fMuon = new Muon();
	fFileName = fileName;
	fMuonFile = new TFile(fileName.c_str(),"READ");
	fMuonTree = (TTree*)fMuonFile->Get("MuonTree");

	fMuonTree->SetBranchAddress("u",&(fMuon->angleX));
	fMuonTree->SetBranchAddress("v",&(fMuon->angleY));
	fMuonTree->SetBranchAddress("w",&(fMuon->angleZ));
	fMuonTree->SetBranchAddress("e",&(fMuon->energy));

}

Muon* MuonReader::GetMuon(){
	//std::cout << "fEntryCount : " << fEntryCount << std::endl;
	fMuonTree->GetEntry(fEntryCount);
	//fMuon->Print();
	fEntryCount++;
	return fMuon;
}

} /* end of lite_interface */
