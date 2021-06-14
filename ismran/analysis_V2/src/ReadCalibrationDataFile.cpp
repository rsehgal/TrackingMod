/*
 * ReadCalibrationDataFile.cpp
 *
 *  Created on: 09-Sep-2020
 *      Author: rsehgal
 */

#include "ReadCalibrationDataFile.h"
#include <iostream>
#include "includes.hh"
#include <TFitResult.h>
#include "HardwareNomenclature.h"
#include <TSystemDirectory.h>
#include <sstream>
#include "HelperFunctions.h"
int StrToInt(std::string s){
	std::stringstream ss(s);
	int val = 0;
	ss >> val;
	return val;
}

Double_t Gaus(Double_t *x,Double_t *par) {
      Double_t arg = 0;
      if (par[2]!=0) arg = (x[0] - par[1])/par[2];
      Double_t fitval = par[0]*TMath::Exp(-0.5*arg*arg);
      return fitval;
}

/*Double_t Pol3(Double_t *x,Double_t *par) {

      Double_t fitval = par[0] + par[1]*x[0] + par[2]*pow(x[0],2) + par[3]*pow(x[0],3);
      return fitval;
}*/

ReadCalibrationDataFile::ReadCalibrationDataFile(std::string dummy) {
	fout = new TFile("calib.root","RECREATE");
}

ReadCalibrationDataFile::ReadCalibrationDataFile() {
	// TODO Auto-generated constructor stub
	//fDatafileNamesVec = vecOfDatafileNames;
	fout = new TFile("calib.root","RECREATE");
	DoParameterization();

}


//Trying to use NOMENCLATURE to generate file name
void ReadCalibrationDataFile::ReadDataFile(){

	//TFile fout("calib.root","RECREATE");

	for(unsigned int i = 0 ; i < vecOfOnlyBarsNames.size() ; i++){
		/*
		 * Generating file name when source is kept at 0.
		 *
		 * Used for offset calculation.
		 */
		char *datafileName =  Form("%s_%s_Couples_N%dV_Th%d_F%dV_Th%d_+0cm.root",
									source.c_str(), vecOfOnlyBarsNames[i].c_str(),
									vecOfNearVoltage[i], vecOfNearThreshold[i],
									vecOfFarVoltage[i], vecOfFarThreshold[i]);

		std::string filename = "/home/rsehgal/BackBoneSoftwares/ismranData/CalibData-71-81/"+std::string(datafileName);
		std::cout << "Working on file : " << filename << std::endl;
		TFile *fp = new TFile(filename.c_str(), "r");
		UShort_t brch; //! board #  and channel number ( its packed in as follows )	//! board*16 + chno.
		UInt_t qlong;   //! integrated charge in long gate 88 nsec
		ULong64_t tstamp;  //! time stamp in pico sec.
		UInt_t time;    //! real computer time in sec
		int delt;
		std::vector<int> vecE;

		TTree *tr = (TTree*) fp->Get("ftree");
		tr->SetBranchAddress("fBrCh", &brch);
		tr->SetBranchAddress("fQlong", &qlong);
		tr->SetBranchAddress("fTstamp", &tstamp);
		tr->SetBranchAddress("fTime", &time);
		tr->SetBranchAddress("fDelt", &delt);
		//tr->SetBranchAddress("vWaveform", &vecE);


		TH1D *hist = new TH1D(fDatafileNamesVec[i].c_str(),fDatafileNamesVec[i].c_str(),100,-30000,30000);

		TTimeStamp *times = new TTimeStamp();
		Long64_t nEntries = tr->GetEntries();
		std::cout << "Total number of Entries : " << nEntries << std::endl;

		Long64_t nb = 0;
		const int offset=0;
		for (Long64_t iev = 0; iev < nEntries; iev++) {
			nb += tr->GetEntry(iev);
			//if(iev==0 || iev==(nEntries-1))
			if(0)
				std::cout << iev << " , " << brch << " , " << tstamp << " , " << time << " , "
						<< qlong << " , " << delt << std::endl;

			if (iev % 500000 == 0) {
				times->Set(time, kTRUE, offset, kFALSE);
				std::cout << " Processing event : " << iev << "\t"
						<< times->GetTimeSpec() << std::endl;
			}

			hist->Fill(delt);

		}      //! event loop

		int binmax = hist->GetMaximumBin();
		unsigned long int maxCount = hist->GetBinContent(binmax);
		double mean = hist->GetMean();
		double stdDev = hist->GetStdDev();


		std::cout << maxCount << " : " << mean << " : " << stdDev << std::endl;

		TF1 *formula = new TF1(Form("fdelt_shift_Cs137_%s_0cm",vecOfOnlyBarsNames[i].c_str()),Gaus,-30000,30000,3);
		formula->SetParameters(maxCount,mean,stdDev);
		hist->Fit(formula,"r");
		std::cout << "Fitted Paramerter : " << formula->GetParameter(0) <<" : " << formula->GetParameter(1) << " : " << formula->GetParameter(2) << std::endl;

		fout->cd();
		formula->Write();
		delete hist;
		delete fp;
		delete formula;



	}
}

void ReadCalibrationDataFile::ReadDataFile_V2(){

	//TFile fout("calib.root","RECREATE");
	std::string dir="/media/rsehgal/CMSSW/sharedCosmicData/MuonTomography/";

	for(unsigned int i = 0 ; i < vecOfOnlyBarsNames.size() ; i++){
		/*
		 * Generating file name when source is kept at 0.
		 *
		 * Used for offset calculation.
		 */
		char *datafileName =  Form("%s_%s_Couples_N%dV_Th%d_F%dV_Th%d_+0cm.root",
									source.c_str(), vecOfOnlyBarsNames[i].c_str(),
									vecOfNearVoltage[i], vecOfNearThreshold[i],
									vecOfFarVoltage[i], vecOfFarThreshold[i]);

		std::string filename = "/home/rsehgal/BackBoneSoftwares/ismranData/CalibData-71-81/"+std::string(datafileName);
		std::cout << "Working on file : " << filename << std::endl;
		TFile *fp = new TFile(filename.c_str(), "r");
		UShort_t brch; //! board #  and channel number ( its packed in as follows )	//! board*16 + chno.
		UInt_t qlong;   //! integrated charge in long gate 88 nsec
		ULong64_t tstamp;  //! time stamp in pico sec.
		UInt_t time;    //! real computer time in sec
		int delt;
		std::vector<int> vecE;

		TTree *tr = (TTree*) fp->Get("ftree");
		tr->SetBranchAddress("fBrCh", &brch);
		tr->SetBranchAddress("fQlong", &qlong);
		tr->SetBranchAddress("fTstamp", &tstamp);
		tr->SetBranchAddress("fTime", &time);
		tr->SetBranchAddress("fDelt", &delt);
		//tr->SetBranchAddress("vWaveform", &vecE);


		TH1D *hist = new TH1D(fDatafileNamesVec[i].c_str(),fDatafileNamesVec[i].c_str(),100,-30000,30000);

		TTimeStamp *times = new TTimeStamp();
		Long64_t nEntries = tr->GetEntries();
		std::cout << "Total number of Entries : " << nEntries << std::endl;

		Long64_t nb = 0;
		const int offset=0;
		for (Long64_t iev = 0; iev < nEntries; iev++) {
			nb += tr->GetEntry(iev);
			//if(iev==0 || iev==(nEntries-1))
			if(0)
				std::cout << iev << " , " << brch << " , " << tstamp << " , " << time << " , "
						<< qlong << " , " << delt << std::endl;

			if (iev % 500000 == 0) {
				times->Set(time, kTRUE, offset, kFALSE);
				std::cout << " Processing event : " << iev << "\t"
						<< times->GetTimeSpec() << std::endl;
			}

			hist->Fill(delt);

		}      //! event loop

		int binmax = hist->GetMaximumBin();
		unsigned long int maxCount = hist->GetBinContent(binmax);
		double mean = hist->GetMean();
		double stdDev = hist->GetStdDev();


		std::cout << maxCount << " : " << mean << " : " << stdDev << std::endl;

		TF1 *formula = new TF1(Form("fdelt_shift_Cs137_%s_0cm",vecOfOnlyBarsNames[i].c_str()),Gaus,-30000,30000,3);
		formula->SetParameters(maxCount,mean,stdDev);
		hist->Fit(formula,"r");
		std::cout << "Fitted Paramerter : " << formula->GetParameter(0) <<" : " << formula->GetParameter(1) << " : " << formula->GetParameter(2) << std::endl;

		fout->cd();
		formula->Write();
		delete hist;
		delete fp;
		delete formula;



	}
}


double ReadCalibrationDataFile::CalculateBarDelTOffsetOf(unsigned int barIndex){



		//	for(unsigned int i = 0 ; i < vecOfOnlyBarsNames.size() ; i++){
		/*
		 * Generating file name when source is kept at 0.
		 *
		 * Used for offset calculation.
		 */
		char *datafileName =  Form("%s_%s_Couples_N%dV_Th%d_F%dV_Th%d_+0cm.root",
									source.c_str(), vecOfOnlyBarsNames[barIndex].c_str(),
									vecOfNearVoltage[barIndex], vecOfNearThreshold[barIndex],
									vecOfFarVoltage[barIndex], vecOfFarThreshold[barIndex]);

		std::string filename = "/home/rsehgal/BackBoneSoftwares/ismranData/CalibData-71-81/"+std::string(datafileName);
		std::cout << "Working on file : " << filename << std::endl;
		TFile *fp = new TFile(filename.c_str(), "r");
		UShort_t brch; //! board #  and channel number ( its packed in as follows )	//! board*16 + chno.
		UInt_t qlong;   //! integrated charge in long gate 88 nsec
		ULong64_t tstamp;  //! time stamp in pico sec.
		UInt_t time;    //! real computer time in sec
		int delt;
		std::vector<int> vecE;

		TTree *tr = (TTree*) fp->Get("ftree");
		tr->SetBranchAddress("fBrCh", &brch);
		tr->SetBranchAddress("fQlong", &qlong);
		tr->SetBranchAddress("fTstamp", &tstamp);
		tr->SetBranchAddress("fTime", &time);
		tr->SetBranchAddress("fDelt", &delt);
		//tr->SetBranchAddress("vWaveform", &vecE);


		TH1D *hist = new TH1D(vecOfOnlyBarsNames[barIndex].c_str(),vecOfOnlyBarsNames[barIndex].c_str(),100,-50,50);

		TTimeStamp *times = new TTimeStamp();
		Long64_t nEntries = tr->GetEntries();
		std::cout << "Total number of Entries : " << nEntries << std::endl;

		Long64_t nb = 0;
		const int offset=0;
		for (Long64_t iev = 0; iev < nEntries; iev++) {
			nb += tr->GetEntry(iev);
			//if(iev==0 || iev==(nEntries-1))
			if(0)
				std::cout << iev << " , " << brch << " , " << tstamp << " , " << time << " , "
						<< qlong << " , " << delt << std::endl;

			if (iev % 500000 == 0) {
				times->Set(time, kTRUE, offset, kFALSE);
				std::cout << " Processing event : " << iev << "\t"
						<< times->GetTimeSpec() << std::endl;
			}

			hist->Fill(delt/1000.);

		}      //! event loop


		int binmax = hist->GetMaximumBin();
		unsigned long int maxCount = hist->GetBinContent(binmax);
		double mean = hist->GetMean();
		double stdDev = hist->GetStdDev();


		std::cout << maxCount << " : " << mean << " : " << stdDev << std::endl;

		TF1 *formula = new TF1(Form("fdelt_shift_Cs137_%s_0cm",vecOfOnlyBarsNames[barIndex].c_str()),Gaus,-50,50,3);
		formula->SetParameters(maxCount,mean,stdDev);
		hist->Fit(formula,"r");
		double meanVal = formula->GetParameter(1);
		std::cout << "Fitted Paramerter : " << formula->GetParameter(0) <<" : " << meanVal << " : " << formula->GetParameter(2) << std::endl;

		fout->cd();
		formula->Write();
		delete hist;
		delete fp;
		delete formula;
		return meanVal ;//formula->GetParameter(1);



//	}
}

double ReadCalibrationDataFile::CalculateBarDelTOffsetOf(std::string datafileName, unsigned int barIndex, int sourcePos){

	std::string filename = "/home/rsehgal/BackBoneSoftwares/ismranData/CalibData-71-81/"
			+ vecOfOnlyBarsNames[barIndex] + "/" + datafileName;
	std::cout << "Working on file : " << filename << std::endl;
	TFile *fp = new TFile(filename.c_str(), "r");
	UShort_t brch; //! board #  and channel number ( its packed in as follows )	//! board*16 + chno.
	UInt_t qlong;   //! integrated charge in long gate 88 nsec
	ULong64_t tstamp;  //! time stamp in pico sec.
	UInt_t time;    //! real computer time in sec
	int delt;
	std::vector<int> vecE;

	TTree *tr = (TTree*) fp->Get("ftree");
	tr->SetBranchAddress("fBrCh", &brch);
	tr->SetBranchAddress("fQlong", &qlong);
	tr->SetBranchAddress("fTstamp", &tstamp);
	tr->SetBranchAddress("fTime", &time);
	tr->SetBranchAddress("fDelt", &delt);
	//tr->SetBranchAddress("vWaveform", &vecE);

	//TH1D *hist = new TH1D(datafileName.c_str(),datafileName.c_str(),100,-30000,30000);
	TH1D *hist = new TH1D(vecOfOnlyBarsNames[barIndex].c_str(),
			vecOfOnlyBarsNames[barIndex].c_str(), 100, -50, 50);

	TTimeStamp *times = new TTimeStamp();
	Long64_t nEntries = tr->GetEntries();
	std::cout << "Total number of Entries : " << nEntries << std::endl;

	Long64_t nb = 0;
	const int offset = 0;
	for (Long64_t iev = 0; iev < nEntries; iev++) {
		nb += tr->GetEntry(iev);
		//if(iev==0 || iev==(nEntries-1))
		if (0)
			std::cout << iev << " , " << brch << " , " << tstamp << " , "
					<< time << " , " << qlong << " , " << delt << std::endl;

		if (iev % 500000 == 0) {
			times->Set(time, kTRUE, offset, kFALSE);
			std::cout << " Processing event : " << iev << "\t"
					<< times->GetTimeSpec() << std::endl;
		}

		hist->Fill(delt/1000.);

	}      //! event loop

	int binmax = hist->GetMaximumBin();
	unsigned long int maxCount = hist->GetBinContent(binmax);
	double mean = hist->GetMean();
	double stdDev = hist->GetStdDev();

	std::cout << maxCount << " : " << mean << " : " << stdDev << std::endl;

	TF1 *formula = new TF1(	Form("fdelt_shift_Cs137_%s_%dcm",vecOfOnlyBarsNames[barIndex].c_str(), sourcePos), Gaus,-50, 50, 3);
	formula->SetParameters(maxCount, mean, stdDev);
	hist->Fit(formula, "r");
	std::cout << "Fitted Paramerter : " << formula->GetParameter(0) << " : "
			<< formula->GetParameter(1) << " : " << formula->GetParameter(2)
			<< std::endl;

	/*if want to write formula for hist at each position then
	 * uncomment the below two lines
	 */
	fout->cd();
	formula->Write();
	delete hist;
	delete fp;

	double meanVal = formula->GetParameter(1);

	delete formula;

	return meanVal;
}

void ReadCalibrationDataFile::CalculateParameterizationOf(unsigned int barIndex){
	double delTOffset = CalculateBarDelTOffsetOf(barIndex);
	std::vector<double> delTMeanVec;
	std::vector<double> corrDelTMeanVec;
	std::vector<double> sourceZPosVec;


	for (unsigned int i = 0 ; i < vecOfSourcePositions.size() ; i++){

		char *datafileName = "";

		if(vecOfSourcePositions[i] < 0){
			datafileName =  Form("%s_%s_Couples_N%dV_Th%d_F%dV_Th%d_%dcm.root",
															source.c_str(), vecOfOnlyBarsNames[barIndex].c_str(),
															vecOfNearVoltage[barIndex], vecOfNearThreshold[barIndex],
															vecOfFarVoltage[barIndex], vecOfFarThreshold[barIndex],vecOfSourcePositions[i]);
		}else{
			datafileName =  Form("%s_%s_Couples_N%dV_Th%d_F%dV_Th%d_+%dcm.root",
												source.c_str(), vecOfOnlyBarsNames[barIndex].c_str(),
												vecOfNearVoltage[barIndex], vecOfNearThreshold[barIndex],
												vecOfFarVoltage[barIndex], vecOfFarThreshold[barIndex],vecOfSourcePositions[i]);
		}

		std::cout << "Reading file name : " << datafileName << std::endl;
		double delTMean = CalculateBarDelTOffsetOf(datafileName,barIndex,vecOfSourcePositions[i]);
		delTMeanVec.push_back(delTMean);
		corrDelTMeanVec.push_back((delTMean - delTOffset));
		sourceZPosVec.push_back(vecOfSourcePositions[i]);

	}
	for(unsigned int i = 0 ; i < corrDelTMeanVec.size() ; i++){
		std::cout << "Actual Mean : " << delTMeanVec[i] << " : Corrected Mean : " << corrDelTMeanVec[i] << " : Source Position : " << sourceZPosVec[i] << std::endl;
	}


	std::cout << "+++++++ Creating Canvas for BarIndex : " << barIndex << std::endl;
	//(new TCanvas(("Bar-"+std::to_string(barIndex)).c_str(),("Bar-"+std::to_string(barIndex)).c_str(),800,600))->cd();
	new TCanvas();
	TGraph *grParam = new TGraph(corrDelTMeanVec.size(),&corrDelTMeanVec[0],&sourceZPosVec[0]);
	grParam->SetName(("grDelTCorrected-"+vecOfOnlyBarsNames[barIndex]).c_str());
	grParam->SetMarkerStyle(8);
	grParam->SetMarkerSize(1);
	grParam->Draw("ap");

	fout->cd();
	grParam->Write();

	TF1 *formula = new TF1(Form("fzparam_%s",vecOfOnlyBarsNames[barIndex].c_str()),Pol3,-10,10,4);
	//formula->SetParameters(maxCount,mean,stdDev);
	grParam->Fit(formula,"r");
	fout->cd();
	formula->Write();

	std::cout << "Size of corrDelTMeanVec : " << corrDelTMeanVec.size() << " : Size of sourceZPosVec : " << sourceZPosVec.size() << std::endl;
	//sleep(20);

}

void ReadCalibrationDataFile::DoParameterization(){
	for(unsigned int barIndex = 0 ; barIndex < vecOfOnlyBarsNames.size() ; barIndex++){
		std::cout <<"@@@@@@@@@@@@ Calling CalculateParameterizationOf for BarIndex : " << barIndex << "  @@@@@@@@@@@@" << std::endl;
		CalculateParameterizationOf(barIndex);
	}
}



ReadCalibrationDataFile::~ReadCalibrationDataFile() {
	// TODO Auto-generated destructor stub
	fout->Close();
	delete fout;
	std::cout << "File written and pointer deleted......" << std::endl;
}


void ReadCalibrationDataFile::DoParameterization_V2(std::string dirname){
	/*
	 * Logic to get the list of all the root file
	 */
	currentBarName="";
	dirName = dirname;
	fDelTMeanVector.clear();
	fPosVector.clear();
	offsett = 0.;
	std::vector<std::string> filenameVector;
	TSystemDirectory dir(dirname.c_str(), dirname.c_str());
	TList *files = dir.GetListOfFiles();
	if (files) {
		TSystemFile *file;
		TString fname;
		TIter next(files);
		while ((file = (TSystemFile*) next())) {
			fname = file->GetName();

			//if (!file->IsDirectory() && fname.EndsWith(ext)) {
			if (!file->IsDirectory() && fname.EndsWith(".root")) {

				std::cout << fname.Data() << std::endl;
				//std::string sFName(fname.Data());
				filenameVector.push_back(std::string(fname.Data()));
			}
		}
	}

	CalculateParameterizationOnVector(filenameVector);

}

void ReadCalibrationDataFile::CalculateParameterizationOnVector(std::vector<std::string> filenameVector){

	for(unsigned int i = 0 ; i < filenameVector.size() ; i++){
		ProcessFile(filenameVector[i]);
	}

	std::cout << "====== Printing Parameterization graph points and offset ===========" << std::endl;
	std::cout << "Offset : " << offsett << std::endl;
	for(unsigned int i = 0 ; i <fDelTMeanVector.size() ; i++){
		std::cout << "SourcePos : " << fPosVector[i] << " : Mean Value : " << fDelTMeanVector[i] << std::endl;
	}

	for(unsigned int i = 0 ; i < fDelTMeanVector.size() ; i++){
		fDelTMeanVector[i] -= offsett ;
	}

	fout->cd();
	TGraph *gr = new TGraph(fDelTMeanVector.size(),&fDelTMeanVector[0],&fPosVector[0]);
	gr->SetMarkerStyle(20);
	gr->SetMarkerSize(0.6);
	gr->Draw("ap");
	gr->Write();
	TF1 *formula = new TF1(Form("fzparam_%s",currentBarName.c_str()),Pol3,-10,10,4);
	gr->Fit(formula,"rq");
	formula->Write();



	/*new TCanvas();
	gr->SetMarkerStyle(20);
	gr->Draw("ap");*/

}

void ReadCalibrationDataFile::ProcessFile(std::string filename){
	FileNameStruct *fileNameStruct = GetFileNameStruct(filename);

	if(fileNameStruct->sSourceType=="Cs137" || fileNameStruct->sSourceType=="Cs")
	{
		currentBarName = fileNameStruct->sBarName;
		std::cout << "Working on file : " << filename << std::endl;
		TFile *fp = new TFile((dirName+filename).c_str(), "r");
		UShort_t brch; //! board #  and channel number ( its packed in as follows )	//! board*16 + chno.
		UInt_t qlong;   //! integrated charge in long gate 88 nsec
		ULong64_t tstamp;  //! time stamp in pico sec.
		UInt_t time;    //! real computer time in sec
		int delt;
		std::vector<int> vecE;

		TTree *tr = (TTree*) fp->Get("ftree");
		tr->SetBranchAddress("fBrCh", &brch);
		tr->SetBranchAddress("fQlong", &qlong);
		tr->SetBranchAddress("fTstamp", &tstamp);
		tr->SetBranchAddress("fTime", &time);
		tr->SetBranchAddress("fDelt", &delt);
		//tr->SetBranchAddress("vWaveform", &vecE);

		//TH1D *hist = new TH1D(datafileName.c_str(),datafileName.c_str(),100,-30000,30000);
		TH1D *hist = new TH1D((fileNameStruct->sBarName).c_str(),
				(fileNameStruct->sBarName).c_str(), 100, -50, 50);

		TTimeStamp *times = new TTimeStamp();
		Long64_t nEntries = tr->GetEntries();
		std::cout << "Total number of Entries : " << nEntries << std::endl;

		Long64_t nb = 0;
		const int offset = 0;
		for (Long64_t iev = 0; iev < nEntries; iev++) {
			nb += tr->GetEntry(iev);
			//if(iev==0 || iev==(nEntries-1))
			if (0)
				std::cout << iev << " , " << brch << " , " << tstamp << " , "
						<< time << " , " << qlong << " , " << delt << std::endl;

			if (iev % 500000 == 0) {
				times->Set(time, kTRUE, offset, kFALSE);
				std::cout << " Processing event : " << iev << "\t"
						<< times->GetTimeSpec() << std::endl;
			}

			hist->Fill(delt/1000.);

		}      //! event loop

		int binmax = hist->GetMaximumBin();
		unsigned long int maxCount = hist->GetBinContent(binmax);
		double mean = hist->GetMean();
		double stdDev = hist->GetStdDev();

		std::cout << maxCount << " : " << mean << " : " << stdDev << std::endl;

		std::string tempBarName=fileNameStruct->sBarName;
		if(fileNameStruct->sBarName=="PS1_S2AB1017"){
			tempBarName="PS01_S2AB1017";
			currentBarName=tempBarName;
		}
		if(fileNameStruct->sBarName=="PS2_S2AB1005"){
                        tempBarName="PS02_S2AB1005";
			currentBarName=tempBarName;
		}
		 if(fileNameStruct->sBarName=="PS3_S3AA1783"){
                        tempBarName="PS03_S3AA1783";
			currentBarName=tempBarName;
		 }
		 if(fileNameStruct->sBarName=="PS4_S2AB1007"){
                        tempBarName="PS04_S2AB1007";
			currentBarName=tempBarName;
		 }
		  if(fileNameStruct->sBarName=="PS5_S2AB1006"){
                        tempBarName="PS05_S2AB1006";
			currentBarName=tempBarName;
		  }
		   if(fileNameStruct->sBarName=="PS6_S2AB1013"){
                        tempBarName="PS06_S2AB1013";
			currentBarName=tempBarName;
		   }
		    if(fileNameStruct->sBarName=="PS7_S3AA1786"){
                        tempBarName="PS07_S3AA1786";
			currentBarName=tempBarName;
		    }
		     if(fileNameStruct->sBarName=="PS8_S3AA1784"){
                        tempBarName="PS08_S3AA1784";
			currentBarName=tempBarName;
		     }
		      if(fileNameStruct->sBarName=="PS9_S2AB1016"){
                        tempBarName="PS09_S2AB1016";
		      	currentBarName=tempBarName;
		      }


		TF1 *formula = new TF1(	Form("fdelt_shift_Cs137_%s_%dcm",(tempBarName).c_str(), fileNameStruct->sSourcePos), Gaus,-50, 50, 3);
		formula->SetParameters(maxCount, mean, stdDev);
		hist->Fit(formula, "r");
		std::cout << "Fitted Paramerter : " << formula->GetParameter(0) << " : "
				<< formula->GetParameter(1) << " : " << formula->GetParameter(2)
				<< std::endl;

		/*if want to write formula for hist at each position then
		  uncomment the below two lines*/

		fout->cd();
		formula->Write();
		delete hist;
		delete fp;

		double meanVal = formula->GetParameter(1);
		fDelTMeanVector.push_back(meanVal);
		fPosVector.push_back((double)fileNameStruct->sSourcePos);
		if(fileNameStruct->sSourcePos == 0){
			offsett = meanVal;
		}


		delete formula;
	}
	else{
		std::cout<< "Wrong Source Type...." << std::endl;
	}

}

//FileName Format
//Source_BarName_Mode_NVoltage_NTh_FVoltage_FTh_SourcePos.root

FileNameStruct* ReadCalibrationDataFile::GetFileNameStruct(std::string filename){
	std::string delimiter = "_";
	//std::string delimiter = ">=";
	std::string s = filename;
	size_t pos = 0;
	std::string token;
	std::vector<std::string> tokenVector;
	while ((pos = s.find(delimiter)) != std::string::npos) {
	    token = s.substr(0, pos);
	    std::cout << token << std::endl;
	    s.erase(0, pos + delimiter.length());
	    tokenVector.push_back(token);

	}
	tokenVector.push_back(s);

	std::cout << s << std::endl;

	std::string sourceType = tokenVector[0];
	if(sourceType != "Cs137" && sourceType != "Cs")
		return (new FileNameStruct(sourceType));

	std::string barName = (tokenVector[1]+"_"+tokenVector[2]);
	std::string mode = tokenVector[3];
	int vNear = StrToInt(tokenVector[4].substr(1,tokenVector[4].size()-2));
	unsigned int thNear = StrToInt(tokenVector[5].substr(2));
	int vFar = StrToInt(tokenVector[6].substr(1,tokenVector[6].size()-2));
	unsigned int thFar = StrToInt(tokenVector[7].substr(2));
	//int sourcePos = StrToInt(tokenVector[7].substr(1,tokenVector[7].size()-3));
	int sourcePos = StrToInt(tokenVector[8].substr(0,tokenVector[8].size()-7));
	if(sourcePos < 0)
		std::cout << "Placed at Negative locations........" << std::endl;
	else
		std::cout << "Placed at Positive locations........" << std::endl;

	std::cout << vNear <<" : " << thNear <<" :  " << vFar << " : " << thFar << " : " << sourcePos << std::endl;

	return (new FileNameStruct(sourceType,barName,mode,vNear,thNear,vFar,thFar,sourcePos));

	//new FileNameStruct(tokenVector[0],(tokenVector[1]+tokenVector[2]),tokenVector[3],);
}
