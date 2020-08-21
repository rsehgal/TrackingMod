/*
 * Analyzer.cpp
 *
 *  Created on: 21-Aug-2020
 *      Author: rsehgal
 */

#include "Analyzer.h"
#include "TTree.h"
#include "includes.hh"
#include "PsBar.h"
#include "TreeEntry.h"



Analyzer::Analyzer() {
	// TODO Auto-generated constructor stub

}

Analyzer::~Analyzer() {
	// TODO Auto-generated destructor stub
}

Analyzer::Analyzer(std::string calibFileName, std::string dataFileName) {
	fCalib = new Calibration(calibFileName);
	fDataFileName = dataFileName;
	TFile *fp = new TFile(fDataFileName.c_str(), "r");
	UShort_t brch; //! board #  and channel number ( its packed in as follows )	//! board*16 + chno.
	UInt_t qlong;   //! integrated charge in long gate 88 nsec
	ULong64_t tstamp;  //! time stamp in pico sec.
	UInt_t time;    //! real computer time in sec
	Int_t dt;      //! delt between near and far channels

	TTree *tr = (TTree*) fp->Get("ftree");
	tr->SetBranchAddress("fBrCh", &brch);
	tr->SetBranchAddress("fQlong", &qlong);
	tr->SetBranchAddress("fTstamp", &tstamp);
	tr->SetBranchAddress("fTime", &time);
	tr->SetBranchAddress("fDelt", &dt);

	UInt_t CurrTime = 0;
	UInt_t StartTime = 0;
	UInt_t StopTime = 0;

	float minCh = 0;
	float maxCh = 32000;
	int nbins = (int) (maxCh - minCh) / 1;

	Long64_t nb = 0;


	TTimeStamp *times = new TTimeStamp();
	Long64_t nEntries = tr->GetEntries();
	for (Long64_t iev = 0; iev < nEntries; iev++) {
		nb += tr->GetEntry(iev);
		if (0)
			std::cout << brch << " , " << qlong << " , " << tstamp << " , " << time << ", " << dt << std::endl;
		fVecOfTreeEntry.push_back(TreeEntry(brch, qlong, tstamp, time, dt));
		if (iev % 500000 == 0) {
			times->Set(time, kTRUE, offset, kFALSE);
			std::cout << " Processing event : " << iev << "\t" << times->GetTimeSpec() << std::endl;
		}

		if (iev == 0) {
			StartTime = time;
			CurrTime = time;
		} else
			StopTime = time;

		UShort_t qch_n = (UShort_t)(((UInt_t) qlong) >> 16);
		UShort_t qch_f = (UShort_t)((UInt_t) qlong);
		float gmean = sqrt(qch_n * qch_f);

		float delt = 1.0 * dt / 1000.;
	}      //! event loop

	TreeEntryVector pairedEntryVec = CheckPairs(fVecOfTreeEntry);
	std::vector<ScintillatorBar*> scintBarVec = DetectMuonHits(pairedEntryVec);
	//PrintScintillatorVector(scintBarVec);

}
