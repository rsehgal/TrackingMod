#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TFile.h>
#include <TColor.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TProfile.h>
#include <TLegend.h>
#include <TSpectrum.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TVirtualPad.h>
#include <TLatex.h>
#include <TRandom3.h>
#include <TVirtualFitter.h>
#include <Math/MinimizerOptions.h>
#include <TTimeStamp.h>
#include <TStopwatch.h>
#include <TApplication.h>

#include "utilities.h"

const int offset=0;
const double kDelTBar = 32.0; //! ns
TStopwatch timer;

std::map<std::string,TH1D*> barsEnergyMap;
std::vector<TH1D*> vecOfEnergyHist;

struct ScintillatorBar{
	std::string  scintName;    //! board #  and channel number ( its packed in as follows ) //! board*16 + chno.

	//Charge info may not be required, but keeping it for the time being.
	UInt_t    qlongNear;   //! integrated charge in long gate 88 nsec
	UInt_t    qlongFar;   //! integrated charge in long gate 88 nsec
	UInt_t    qlongMean;   //! integrated charge in long gate 88 nsec


	ULong64_t tstampNear;  //! time stamp in pico sec.
	ULong64_t tstampFar;  //! time stamp in pico sec.
	ULong64_t tsmallTimeStamp;  //! time stamp in pico sec.
	Long64_t deltaTstamp;  //! time stamp in pico sec.
	UInt_t    time;    //! real computer time in sec

	unsigned short barIndex;


	ScintillatorBar(unsigned short l_channelNear, unsigned short l_channelFar,
					ULong64_t l_tstampNear, ULong64_t l_tstampFar,
					UInt_t l_qlongNear,	UInt_t l_qlongFar, UInt_t l_time){

		barIndex = l_channelNear/2;
		scintName="PsBar"+std::to_string(barIndex)+"-"+std::to_string(l_channelNear)+"-"+std::to_string(l_channelFar);
		qlongNear=l_qlongNear;
		qlongFar=l_qlongFar;
		qlongMean=sqrt(qlongNear*qlongFar);
		tstampNear=l_tstampNear;
		tstampFar=l_tstampFar;
		tsmallTimeStamp = (tstampNear < tstampFar) ? tstampNear : tstampFar;
		deltaTstamp=tstampNear-tstampFar;
		time=l_time;
	}

	ScintillatorBar(){}

	void Print(){

		std::cout << scintName << " , " << qlongNear << " , " << qlongFar << " , " << qlongMean << " , "
				  << tstampNear  << " , " << tstampFar << " , " << tsmallTimeStamp << " , " << deltaTstamp << " , " << time << std::endl;

	}


};

bool CompareTimestampScintillator(ScintillatorBar *i1, ScintillatorBar *i2)
{
	//return (i1->tstampNear < i2->tstampNear);
	return (i1->tsmallTimeStamp < i2->tsmallTimeStamp);
}

void PrintScintillatorVector(std::vector<ScintillatorBar*> scintBarVector){
	//unsigned int n = scintBarVector.size();
	unsigned int n=50;
	std::cout << "======= Printing " << n <<" Scintillators entries===========" << std::endl;
	std::cout << "BarName" << " , " << "qlongNear" << " , " << "qlongFar" << " , " << "qlongMean" << " , "
					  << "tstampNear"  << " , " << "tstampFar" << " , " << "tsmallTimeStamp" << " , " << "deltaTstamp" << " , " << "ClockTime" << std::endl;
	for(unsigned int i=0 ; i < n ; i++){
		scintBarVector[i]->Print();
	}
}



void InitializeBarsEnergyMap(){
	unsigned short int numOfChannels=60;
	for(unsigned short int i = 0 ; i < numOfChannels ; i++){
		std::string barId=std::to_string(i)+std::to_string(i+1);
		
		int qstart=0;
		int qend=25000;
		int nbins=1000;
		const char *histName=("Q-GeomMean"+barId).c_str();
		barsEnergyMap[barId]=new TH1D(histName,histName,nbins,qstart,qend);
	}
}

void InitializeVectOfEnergyHist(){
	unsigned short int numOfChannels=60;
	int qstart=0;
	int qend=25000;
	int nbins=1000;

	for(unsigned short int i = 0 ; i < numOfChannels ; ){
		unsigned int barIndex=i/2;
		std::string barId="Bar-"+std::to_string(barIndex);
		const char *histName=barId.c_str();
		vecOfEnergyHist.push_back(new TH1D(histName,histName,nbins,qstart,qend));
		i=i+2;
	}
}

struct TreeEntry
{
  UShort_t  brch;    //! board #  and channel number ( its packed in as follows )
  //! board*16 + chno.
  UInt_t    qlong;   //! integrated charge in long gate 88 nsec 
  ULong64_t tstamp;  //! time stamp in pico sec.
  UInt_t    time;    //! real computer time in sec
  Int_t     dt;      //! delt between near and far channels

  TreeEntry(UShort_t br, UInt_t ql, ULong64_t ts,UInt_t t, Int_t delT){
    brch=br;
    qlong=ql;
    tstamp=ts;
    time=t;
    dt=delT;

  }
  
  void Print(){
  	std::cout << brch <<" , " << qlong << " , " << tstamp <<" , "<< time << ", " << dt << std::endl;
  }
  
};


using TreeEntryVector = std::vector<TreeEntry>;

bool CompareTimestamp(TreeEntry i1, TreeEntry i2) 
{ 
	return (i1.tstamp < i2.tstamp); 
} 

void CheckPairs(TreeEntryVector treeEntVec);

void PrintEntryVector(TreeEntryVector treeEntVec){
	//int lenToPrint=treeEntVec.size();
	int lenToPrint=110;
	for(unsigned int i = 0 ; i < lenToPrint ; i++){
		treeEntVec[i].Print();
	}
	
	std::cout << "Printing timediff : " << treeEntVec[7].tstamp-treeEntVec[0].tstamp << std::endl;
}

void PlotHistOfTSDiff(TreeEntryVector treeEntVec){
	gStyle->SetOptStat();//1001);
	unsigned int nbins=10000;
	int tstart=-2000;
	int tend=2000;
	new TCanvas();
	TH1D *hist=new TH1D("TimeDiffHist","TimeDiffHist",nbins,tstart,tend);
	for(unsigned int i = 0 ; i < treeEntVec.size() ; i++){
	//for(unsigned int i = 0 ; i < 100 ; i++){
		Long64_t tdiff=(treeEntVec[i].tstamp-treeEntVec[i+1].tstamp);
		//std::cout <<"I : " << treeEntVec[i].tstamp << " : I+1 : " << treeEntVec[i+1].tstamp << " : Diff : " << tdiff << std::endl;
		tdiff/=1000.;
		hist->Fill(tdiff);
	}
	hist->Draw();
	return;
}

void PlotHistOfTS(TreeEntryVector treeEntVec){
	gStyle->SetOptStat();//1001);

	int tstart=treeEntVec[0].tstamp;
	int tend=treeEntVec[treeEntVec.size()-1].tstamp;
	Long64_t nbins=(tend-tstart)/100000;

	new TCanvas();
	TH1D *hist=new TH1D("TimeStamp","TimeStamp",nbins,tstart,tend);
	for(unsigned int i = 0 ; i < treeEntVec.size() ; i++){
	//for(unsigned int i = 0 ; i < 100 ; i++){
		//Long64_t tdiff=(treeEntVec[i].tstamp-treeEntVec[i+1].tstamp);
		//std::cout <<"I : " << treeEntVec[i].tstamp << " : I+1 : " << treeEntVec[i+1].tstamp << " : Diff : " << tdiff << std::endl;
		//tdiff/=1000.;
		//hist->Fill(tdiff);
		
		hist->Fill(treeEntVec[i].tstamp);
	}
	hist->Draw();
	return;
}

void PlotHistOfQ(TreeEntryVector treeEntVec){
	gStyle->SetOptStat();//1001);

	int qstart=0;
	int qend=25000;
	int nbins=1000;

	new TCanvas();
	TH1D *hist=new TH1D("Q-GeomMean","Q-GeomMean",nbins,qstart,qend);
	for(unsigned int i = 0 ; i < treeEntVec.size() ; ){
		std::string barId="";
		unsigned int barIndex=0;
		if(treeEntVec[i].brch < treeEntVec[i+1].brch){
			barId=std::to_string(treeEntVec[i].brch)+std::to_string(treeEntVec[i+1].brch);
			barIndex=treeEntVec[i].brch/2;
		}
		else{
			barId=std::to_string(treeEntVec[i+1].brch)+std::to_string(treeEntVec[i].brch);
			barIndex=treeEntVec[i+1].brch/2;
		}
		
		//UShort_t qch_n = (UShort_t)(((UInt_t) treeEntVec[i].qlong)>>16);
    	//UShort_t qch_f = (UShort_t)((UInt_t)treeEntVec[i].qlong);
	    /*
	    if(barId=="89"){
			float gmean   = sqrt(treeEntVec[i].qlong*treeEntVec[i+1].qlong);		
			hist->Fill(gmean);
		}
		*/
		float gmean   = sqrt(treeEntVec[i].qlong*treeEntVec[i+1].qlong);		
		//barsEnergyMap[barId]->Fill(gmean);
		vecOfEnergyHist[barIndex]->Fill(gmean);
		
		i=i+2;
	}
	//hist->Draw();
	return;
}

/*
 * Function to detect the Muon Hit based on the energy info
 */
std::vector<ScintillatorBar*> DetectMuonHits(TreeEntryVector treeEntVec){
	std::cout <<"Trying to detect Muon Hits.........." << std::endl;
	/*gStyle->SetOptStat();//1001);

	int qstart=0;
	int qend=25000;
	int nbins=1000;

	new TCanvas();
	TH1D *hist=new TH1D("Q-GeomMean","Q-GeomMean",nbins,qstart,qend);*/
	std::vector<ScintillatorBar*> scintBarVec;

	int qstart = 4000;
	int qend = 18000;
	for(unsigned int i = 0 ; i < treeEntVec.size() ; ){
		std::string barId="";
		unsigned int barIndex=0;
		float qmean   = sqrt(treeEntVec[i].qlong*treeEntVec[i+1].qlong);
		if(qmean > qstart && qmean < qend){
			if(treeEntVec[i].brch < treeEntVec[i+1].brch){
				scintBarVec.push_back(new ScintillatorBar(treeEntVec[i].brch,treeEntVec[i+1].brch,
														  treeEntVec[i].tstamp,treeEntVec[i+1].tstamp,
														  treeEntVec[i].qlong,treeEntVec[i+1].qlong,treeEntVec[i].time));
				barIndex=treeEntVec[i].brch/2;
			}
			else{
				scintBarVec.push_back(new ScintillatorBar(treeEntVec[i+1].brch,treeEntVec[i].brch,
														  treeEntVec[i+1].tstamp,treeEntVec[i].tstamp,
														  treeEntVec[i+1].qlong,treeEntVec[i].qlong,treeEntVec[i].time));
				barIndex=treeEntVec[i+1].brch/2;
			}

			float gmean=sqrt(treeEntVec[i].qlong*treeEntVec[i+1].qlong);
			vecOfEnergyHist[barIndex]->Fill(gmean);
		}


		i=i+2;

	}

	std::cout << "Length of Detected Muon Hits : " << scintBarVec.size() << std::endl;
	return scintBarVec;
}

/*
 * Function to detect muon hits belonging to the muon track
 *
 * -----
 * ALGO:
 * -----
 * Check consecutive deltaTFar or deltaTNear, or better may be delta(TFar < TNear ? TFar : TNear)
 * If deltaT is less than 20 ns window (window size needs to be calculated based on physics), then
 * then all the muon hits within 5ns windown belongs to same muon track(and ideally from the
 * current sample data the length of this vector should be 3) otherwise they belongs to different
 * muon tracks.
 *
 * @input  : vector of muon hits i.e. std::vector<ScintillatorBar*> sorted by smallest of TNear and TFar ie. tsmallTimeStamp
 * @output : return vector of muon tracks i.e. std::vector< std::vector<ScintillatorBar*> >
 */
std::vector< std::vector<ScintillatorBar*> >  DetectMuonTracks(std::vector<ScintillatorBar*> muonHitVector){

	unsigned int hitLength = muonHitVector.size();
	std::vector< std::vector<ScintillatorBar*> >  muonTrackVec;
	std::vector<ScintillatorBar*> singleMuonTrack;
	singleMuonTrack.push_back(muonHitVector[0]);
	for(unsigned int i = 1 ; i < hitLength ; i++){

//		ULong64_t timeI = (muonHitVector[i]->tstampNear < muonHitVector[i]->tstampFar) ? muonHitVector[i]->tstampNear : muonHitVector[i]->tstampFar;
//		ULong64_t timeIPlusOne = (muonHitVector[i+1]->tstampNear < muonHitVector[i+1]->tstampFar) ? muonHitVector[i+1]->tstampNear : muonHitVector[i+1]->tstampFar;


		if((muonHitVector[i]->tsmallTimeStamp - muonHitVector[i-1]->tsmallTimeStamp) < 20000){
			//Within 20ns window
			singleMuonTrack.push_back(muonHitVector[i]);
		}else{
			//Outside 20ns window
			muonTrackVec.push_back(singleMuonTrack);
			singleMuonTrack.clear();
			singleMuonTrack.push_back(muonHitVector[i]);
		}
	}

	return muonTrackVec;
}

void PrintMuonTrackVector(std::vector< std::vector<ScintillatorBar*> > muonTrackVec){
	//unsigned int muonTrackVecLength = muonTrackVec.size();
	unsigned int muonTrackVecLength = 20;
	for(unsigned int i = 0 ; i < muonTrackVecLength ; i++){
		std::cout << "====== Print Muon Track : " << i << " ======" << std::endl;
		unsigned int muonHitLength = muonTrackVec[i].size();
		for(unsigned int j = 0 ; j < muonHitLength ; j++){
			muonTrackVec[i][j]->Print();
		}
	}
}

//void DigitizerDiff();
/*void DigitizerAll();
void Digitizer()
{
  //DigitizerDiff();
  DigitizerAll();
}*/
void DigitizerAll()
{
  timer.Start();
  
  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();
  LoadStyle();

  const int nset=3;

  TFile *fin=0;
  std::string infile="testismran.root";
  //std::string infile="Waveforms_Ch0Ch1_SF891_all32ns.root";
  std::string outfile="test.root";
  TFile *fout = new TFile(outfile.c_str(),"RECREATE");

  
  UInt_t CurrTime=0;
  UInt_t StartTime=0;
  UInt_t StopTime=0;

  float minCh=0;
  float maxCh=32000;
  int nbins=(int)(maxCh-minCh)/1;

  Long64_t nb=0;    

  fin = new TFile(infile.c_str(),"r");  
  //! Variables in the input file obtained from DAQ.
  UShort_t  brch;    //! board #  and channel number ( its packed in as follows )
  //! board*16 + chno.
  UInt_t    qlong;   //! integrated charge in long gate 88 nsec 
  ULong64_t tstamp;  //! time stamp in pico sec.
  UInt_t    time;    //! real computer time in sec
  Int_t     dt;      //! delt between near and far channels
  
  TTree *tr= (TTree*)fin->Get("ftree");
  tr->SetBranchAddress("fBrCh"  ,&brch);
  tr->SetBranchAddress("fQlong" ,&qlong);
  tr->SetBranchAddress("fTstamp",&tstamp);
  tr->SetBranchAddress("fTime"  ,&time);
  tr->SetBranchAddress("fDelt"  ,&dt);
  
  Long64_t nEntries = tr->GetEntries();
  
  TTimeStamp *times = new TTimeStamp();
  
  //! From known date to sec conversion
  //!             Y    M   D  H  M  S NS  isUTC   secoffset
  //! times->Set(2017, 11, 9, 2, 1, 0, 0, kTRUE,  0     )
  
  //! From known sec to DD/MM/YY HH:MM:SS  conversion  
  //!            fTime, isUTC, secoffset  dosDate
  //! times->Set(time , kTRUE, 0        , kFALSE);
  
  nb += tr->GetEntry(0);
  times->Set(time,kTRUE,offset,kFALSE);
  cout << " The aquisition started on : "<< times->GetDate() 
       << " at : "  << times->GetTime()  << endl;
  StartTime=time;
  nb += tr->GetEntry(tr->GetEntries()-1);
  times->Set(time,kTRUE,offset,kFALSE);
  cout << " The aquisition stopped on : "<< times->GetDate() 
       << " at : "  << times->GetTime()  << endl;
  cout << endl;
  StopTime=time;
 
  std::cout << " Total #  of events : " << nEntries << std::endl;
  
  StartTime=StopTime=0;
  
  TreeEntryVector treeEntryVec;

  for(Long64_t iev=0; iev<nEntries; iev++){
    nb += tr->GetEntry(iev);
    if(0)
      std::cout << brch <<" , " << qlong << " , " << tstamp <<" , "<< time << ", " << dt << std::endl;
    treeEntryVec.push_back(TreeEntry(brch,qlong,tstamp,time,dt)) ;
    if( iev%500000 == 0 ){
      times->Set(time,kTRUE,offset,kFALSE);
      cout << " Processing event : " << iev << "\t"<< times->GetTimeSpec() << endl;
    }
    
    if( iev == 0 ){
      StartTime=time;
      CurrTime =time;
    }else StopTime=time;
    
    UShort_t qch_n = (UShort_t)(((UInt_t)qlong)>>16);
    UShort_t qch_f = (UShort_t)((UInt_t)qlong);
    float gmean   = sqrt(qch_n*qch_f);
    
    float delt    = 1.0*dt/1000.;
  }//! event loop

  float AcqTime = 1.0*(StopTime - StartTime);
  //hTimeRun[i]->SetBinContent(1,AcqTime);
  cout << " Global AcqTime : " << AcqTime << endl;

  std::cout << "Length of TreeEntryVector : " << treeEntryVec.size() << std::endl;
  
  std::cout << "=============== Going to Detect Unpaired Entries =============" << std::endl;
  CheckPairs(treeEntryVec);

  fout->cd();
  fout->Write();

  timer.Stop();
  float  mbytes = 0.000001*nb;
  float rtime  = timer.RealTime();
  float ctime  = timer.CpuTime();
  
  std::cout<<"\t"<<std::endl;
  std::cout<<Form("RealTime=%f seconds, CpuTime=%f seconds",rtime,ctime)<<std::endl;
  std::cout<<Form("You read %f Mbytes/RealTime seconds",mbytes/rtime)<<std::endl;
  std::cout<<Form("You read %f Mbytes/CpuTime  seconds",mbytes/ctime)<<std::endl;
  std::cout<<"\t"<<std::endl;
  std::cout<<"Good bye : " <<"\t"<<std::endl;
}

void CheckPairs(TreeEntryVector treeEntVec){
	TreeEntryVector smallTSEntVec;
	TreeEntryVector pairedEntVec;
	TreeEntryVector unpairedEntVec;
	int counter=110;
	for(unsigned int i = 0 ; i < treeEntVec.size() ; ){
	
		//treeEntVec[i].Print();
		
		if(abs(treeEntVec[i].brch - treeEntVec[i+1].brch) > 1){
			unpairedEntVec.push_back(treeEntVec[i]);
			
			
			if(0){
				//std::cout << "Unpaired entry found at index : " << i << std::endl;
				counter--;
				treeEntVec[i].Print();
				if(!counter)
					break;
				
			}
			
			i++;
			
		}else{
			pairedEntVec.push_back(treeEntVec[i]);
			pairedEntVec.push_back(treeEntVec[i+1]);
			if(treeEntVec[i].tstamp > treeEntVec[i+1].tstamp)
				smallTSEntVec.push_back(treeEntVec[i+1]);
			else
				smallTSEntVec.push_back(treeEntVec[i]);	
	
			i = i+2;
		}
		
	}
	std::cout << "Length of Unpaired Entries Vector : " << unpairedEntVec.size() << std::endl;
	std::cout << "Length of Paired Entries Vector : " << pairedEntVec.size() << std::endl;
	std::cout << "Length of SmallTS Entries Vector : " << smallTSEntVec.size() << std::endl;
	std::cout << "Percentage of Unpaired Entries : " << (unpairedEntVec.size()*1.0/treeEntVec.size())*100 <<" %" << std::endl;
	
	std::cout <<"================== Printing few entries of Paired Entry Vec==================" << std::endl;	
	PrintEntryVector(pairedEntVec);
	
	std::cout <<"========== Trying to Sort SmallTS Entry Vector ==============" << std::endl;
		std::sort(smallTSEntVec.begin(),smallTSEntVec.end(),CompareTimestamp);
	std::cout <<"===================== Sorting Done........ ==================" << std::endl;
	
	std::cout <<"================== Printing few entries of SmallTS Entry Vec==================" << std::endl;
	PrintEntryVector(smallTSEntVec);
	
	//std::cout <<"================== Printing few entries ==================" << std::endl;
	//PrintEntryVector(smallTSEntVec);

	PlotHistOfTSDiff(pairedEntVec);
	//Working
	//PlotHistOfQ(pairedEntVec);

	std::vector<ScintillatorBar*> scintBarVec = DetectMuonHits(pairedEntVec);
	PrintScintillatorVector(scintBarVec);

	std::cout <<"========== Trying to Sort ScintillatorBar Vector ==============" << std::endl;
	std::sort(scintBarVec.begin(),scintBarVec.end(),CompareTimestampScintillator);
	std::cout <<"===================== Sorting Done........ ==================" << std::endl;
	PrintScintillatorVector(scintBarVec);

	std::cout << std::endl;
	std::cout <<"==========================================================================================" << std::endl;
	std::cout <<"@@@@@@@@@@@@@@@@@@@@@@@@@@@ Trying to Get Vector of Muon tracks @@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
	std::cout <<"==========================================================================================" << std::endl;
	std::vector<std::vector<ScintillatorBar*>> muonTrackVec = DetectMuonTracks(scintBarVec);
	PrintMuonTrackVector(muonTrackVec);

	//std::sort(pairedEntVec.begin(),pairedEntVec.end(),CompareTimestamp);
	//PrintEntryVector(pairedEntVec);
	
	
	//PlotHistOfTS(smallTSEntVec);
	//return;
}

int main(){

  //InitializeBarsEnergyMap();
  //std::cout << "========== Energy Map Initialized. =========="<<std::endl;
  std::cout << "==========Vector of  Energy Histogram Initialized. =========="<<std::endl;
  InitializeVectOfEnergyHist();
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  DigitizerAll();
  //barsEnergyMap["67"]->Draw();
  
  //Drawing histograms for all the bars
  for(unsigned int i = 0 ; i < vecOfEnergyHist.size(); i ++){
  	new TCanvas();
  	vecOfEnergyHist[i]->Draw();
  }
  
  fApp->Run();
}
