/*
 * OfflineAnalysis.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */
#include "OfflineAnalysis.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <TTree.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "IniFile.h"


//Temporarily doing this
using namespace std;

namespace Tracking{

OfflineAnalysis::OfflineAnalysis(){}
OfflineAnalysis::OfflineAnalysis(std::string filename, unsigned modulesCount) : fModulesCount(modulesCount),
          largeAcceptance(0), smallAcceptance(0){

      fVerbose_trg = true;
      inpfiles.SetRootFileName(filename);
      char *fName = const_cast<char*>(filename.c_str());
      ReadFilesAndFillData();
      InitializeTree(fName);
      ReadTriggerBit();
      ReadTriggerBitEta();
      ReadEtaPartition();
      ReadLargeAcceptance();
      ReadSmallAcceptance();
      bool verbose=true;
      if(verbose){
      std::cout<<"TriggerBit : "<<triggerBit<<std::endl;
      std::cout<<"EtaPartition : "<<etaPartition<<" : Large : "<<largeAcceptance<<" : Small : "<<smallAcceptance<<std::endl;
      }
      ReadInifileAndFillModuleInfo();


      //Just printing triggering info
      PrintTriggerInfo();

      ReadMappingInfo();
      ReadScintMapFileAndCreatePlane();

        }
OfflineAnalysis::~OfflineAnalysis(){}

void OfflineAnalysis::ReadInitializationFile(){}
void OfflineAnalysis::StripProfile(){}
void OfflineAnalysis::GenerateTracks(){}


void OfflineAnalysis::Init(){
  fData.resize(fModulesCount);
  fBranches.resize(fModulesCount);
  HitsPerChannel.resize(fModulesCount);
  //NUMBER_OF_CHANNELS.resize(fModulesCount);
  for (unsigned i = 0; i < fModulesCount; ++i) {
    fData[i].resize(ChannelsCount);
    fBranches[i].resize(ChannelsCount);
  }
}

/*
 * This funtion will create the scintillator planes
 */
void OfflineAnalysis::ReadScintMapFileAndCreatePlane(){
  //ifstream map_file(inpfiles.GetMappingFileName());
  ifstream map_file("scintMap.txt");
  int scintId=0, moduleId=0, channelId=0;
  int numOfPlane=0, numOfScintInEachPlane=0;
  std::cout<<"ScintMapFile-Exist : "<<map_file.is_open()<<std::endl;
    if (map_file.is_open()) {
      map_file >> numOfPlane >> numOfScintInEachPlane;
      numOfPlane=2;
      numOfScintInEachPlane=4;
      std::cout<<numOfPlane<<" , "<<numOfScintInEachPlane<<std::endl;
      for(int i=0 ; i<numOfPlane ; i++){
        fScintPlaneVector.push_back(new ScintillatorPlane(numOfScintInEachPlane,"ScintillatorPlane-"+std::to_string(i)));
      }
      /*while (map_file.good()) {
        map_file >> scintId >> moduleId >> channelId ;

      }*/
    }
    map_file.close();
}

void OfflineAnalysis::ReadMappingInfo(){

  // ifstream map_file("/home/user/workspace/rpcupgrade/Offline/mapping.txt");
  ifstream map_file(inpfiles.GetMappingFileName());
  if (map_file.is_open()) {
    while (map_file.good()) {
      map_file >> map_station >> map_module >> map_start >> map_finish >> map_module1 >> map_start1 >> map_finish1;
      cout << "loop : " << map_station << endl;

      //Below info may not be required for the time being
      //Hence commenting if for the time being
      // reference_station=map_station; //This line was already commented
      /*
      if (map_station == refStation) {
        reference_module = map_module;
        reference_map_start = map_start;
        reference_map_finish = map_finish;
        reference_map_module1 = map_module1;
        reference_map_start1 = map_start1;
        reference_map_finish1 = map_finish1;
      }

      if (map_station == refStation_t2) {
        reference_module_t2 = map_module;
        reference_map_start_t2 = map_start;
        reference_map_finish_t2 = map_finish;
        reference_map_module1_t2 = map_module1;
        reference_map_start1_t2 = map_start1;
        reference_map_finish1_t2 = map_finish1;
      }
      */

    }
  }
  map_file.close();
}

void OfflineAnalysis::ReadReferenceChamberInfo(){

  // Reading the reference stations
    //ifstream refch("/srv/www/htdocs/refchamber.txt");
    ifstream refch("refchamber.txt");
    if (refch.is_open()) {
      while (refch.good()) {
        refch >> refStation;
      }
      refch.close();
      cout << "Ref 1 : " << refStation << endl;
    } else {
      refStation= -1;
    }

    cout << "Strange" << endl;
    //ifstream refch2("/srv/www/htdocs/refchamber_t2.txt");
    ifstream refch2("refchamber_t2.txt");
    if (refch2.is_open()) {
      while (refch2.good()) {
        refch2 >> refStation_t2;
      }
      refch2.close();
      cout << "Ref 2 : " << refStation_t2 << endl;
    } else {
      // reference_station_t2=-1;
      // Added
      cout << "Strange 2" << endl;
      refStation_t2 = 0;
    }
}

void OfflineAnalysis::PrintTriggerInfo(){
  if(fVerbose_trg){
    cout << " " << endl << " " << endl << "-----------------------------------------------------" << endl;
    cout << "Master trigger bit :  " << triggerBit << endl;
    cout << "-----------------------------------------------------" << endl << " " << endl << " " << endl;
    cout << " " << endl;

    if (largeAcceptance == 1) {
      cout << " " << endl;
      cout << "---------------------------------------- " << endl;
      cout << " Using large acceptance trigger, eta=ALL " << endl;
      cout << "---------------------------------------- " << endl;
      cout << " " << endl;
      if (etaPartition != "ALL") {
        cout << "Warning eta partition was not set to 'ALL'... so I am setting it." << endl << endl;
        etaPartition = "ALL";
      }
    }
/*
    if (twoTrackingRpc == 1) {
      cout << " " << endl;
      cout << "---------------------------------------- " << endl;
      cout << " Using two tracking RPCs in the trigger " << endl;
      cout << "---------------------------------------- " << endl;
      //cout << "Refence station : " << reference_station_t2 << endl;
      if ((reference_station <= 0) || (reference_station_t2 <= 0)) {
        cout << "Raman Warning REFERENCE STATIONS NOT PROPERLY configured, going out..." << endl << endl;
        return;
      }

      // First reference
      {
        cout << " First reference detectors is on station " << reference_station << " ( module = " << reference_module
             << ", " << reference_map_start << "<channel<" << reference_map_finish;

        if (reference_map_module1 > 0) {
          cout << " and module = " << reference_map_module1 << ", " << reference_map_start1 << "<channel<"
               << reference_map_finish1 << " )" << endl;
        } else {
          cout << " )" << endl;
        }
      }

      // Second reference
      {
        cout << " Second reference detectors is on station " << reference_station_t2
             << " ( module = " << reference_module_t2 << ", " << reference_map_start_t2 << "<channel<"
             << reference_map_finish_t2;

        if (reference_map_module1_t2 > 0) {
          cout << " and module = " << reference_map_module1_t2 << ", " << reference_map_start1_t2 << "<channel<"
               << reference_map_finish1_t2 << " )" << endl;
        } else {
          cout << " )" << endl;
        }
        cout << "--------------------------" << endl;
        cout << " " << endl;
      }

      if (etapartition != "ALL") {
        etapartition = "ALL";
        cout << "Warning eta partition was not set to 'ALL'... so I am setting it to 'ALL'." << endl << endl;
      }
    }
    */
  }
}

void OfflineAnalysis::ReadTwoRpcFile(){

  ifstream conf_file(inpfiles.GetTwoRpcFileName());
  if (conf_file.is_open()) {
    while (conf_file.good()) {
      conf_file >> twoTrackingRpc;
    }
  }
  conf_file.close();
}

void OfflineAnalysis::ReadLargeAcceptance(){

  ifstream conf_file(inpfiles.GetLargeAcceptanceFile());
  if (conf_file.is_open()) {
    while (conf_file.good()) {
      conf_file >> largeAcceptance;
    }
  }
  conf_file.close();
}

void OfflineAnalysis::ReadSmallAcceptance(){
  ifstream conf_file(inpfiles.GetSmallAcceptanceFile());
  bool verbose(false);
  if(verbose)
    std::cout<<"Small.cfg Exist : "<<conf_file.is_open()<<std::endl;
  if (conf_file.is_open()) {
    while (conf_file.good()) {
      conf_file >> smallAcceptance;
    }
  }
  conf_file.close();
}

void OfflineAnalysis::ReadEtaPartition(){

    ifstream conf_file(inpfiles.GetEtaPartitionFile());
    bool verbose(false);
    if(verbose)
      std::cout<<"trg_eta Exist : "<<conf_file.is_open()<<std::endl;
  if (conf_file.is_open()) {
    while (conf_file.good()) {
      conf_file >> etaPartition;
    }
  }
  conf_file.close();
}

void OfflineAnalysis::ReadTriggerBitEta(){

/*  ifstream conf_file2("offline_eta.cfg");
  if (conf_file2.is_open()) {
    while (conf_file2.good()) {
      getline(conf_file2, conf_line);
      mat_line = "trigger : ";
      if (((int)conf_line.find(mat_line)) != -1) {
        conf_line.erase(0, mat_line.length());
        trigger_biteta = atoi(conf_line.c_str());
      }
      conf_file2.close();
    }
  }
  conf_file2.close();*/

  ReadTriggerBit();
  triggerBitEta = triggerBit;

}

void OfflineAnalysis::ReadTriggerBit(){

  ifstream conf_file(inpfiles.GetOfflineCfgFileName());
  std::string conf_line;
  if (conf_file.is_open()) {
    while (conf_file.good()) {
      getline(conf_file, conf_line);
      std::string mat_line = "trigger : ";
      if (((int)conf_line.find(mat_line)) != -1) {
        conf_line.erase(0, mat_line.length());
        triggerBit = atoi(conf_line.c_str());
      }
      conf_file.close();
    }
  }
  conf_file.close();
}

void OfflineAnalysis::ReadInifileAndFillModuleInfo(){

  IniFile * ini = new IniFile( inpfiles.GetInitializationFileName() );
  //ini->Read();
  ini->ReadAndFillTDC();
  fTdcVector = ini->GetTdcVector();
  for(int i=0;i<fTdcVector.size();i++){
    //fTdcVector[i].Print();
  }
  cout << "Trigger Windows Width: " << fTdcVector[0].GetTriggerWindowWidth() << endl;
  timeWindow = fTdcVector[0].GetTriggerWindowWidth();
  timeWindow=timeWindow*25/1000000000;      // expressing time windows in sec:
}

void OfflineAnalysis::InitializeTree(char *fName){
  TTree *tree = 0;
  std::cout << "- Initializing " << fModulesCount << " modules." << std::endl;
  std::cout << "Going to initialize the tree...\n";
  if (tree == 0) {
    TFile *f = (TFile *)gROOT->GetListOfFiles()->FindObject(fName);
    if (!f) {
      f = new TFile(fName);
    }
    tree = (TTree *)gDirectory->Get("BSC_DATA_TREE");
  }
}

void OfflineAnalysis::ReadFilesAndFillData(){

	bool verbose_openfile = 1;
	std::stringstream stst;
	std::ifstream in(inpfiles.GetRootFileName());
	if (verbose_openfile) std::cout << " " << std::endl << " " << std::endl << "-----------------------------------------------------" << std::endl;
	if (verbose_openfile) std::cout << "Opening file: " << inpfiles.GetRootFileName() << std::endl;

	std::ifstream runname(inpfiles.GetRunFileName());
	if (runname.is_open()) {
	        while(runname.good()) {
	            runname >> run;
	        }
	    }

	run_config_file = inpfiles.GetDaqInfoFileName();
	if (verbose_openfile) std::cout << "Run config file: " << run_config_file  << std::endl;
	std::ifstream rff(run_config_file);
    if (rff.is_open()) {
      if (verbose_openfile)
        std::cout << "Run config file: " << run_config_file << " is opened." << std::endl;

      // This is the header of the daqinfo file
      rff >> variablestring1 >> temperature >> humidity >> pressure;
      if (verbose_openfile)
        cout << "Temp: " << temperature << " humidity: " << humidity << " pressure: " << pressure << std::endl;

      rff >> variablestring2 >> etaPartition >> variablestring3 >> Trigger_layer;
      if (verbose_openfile)
        cout << "Eta partition: " << etaPartition << " trigger: " << Trigger_layer << endl;

      //temporary variable to read blank lines
      std::string rff_conf_line;
      getline(rff, rff_conf_line);
      if (verbose_openfile)
        cout << "reading this line: " << rff_conf_line << endl;

      getline(rff, rff_conf_line);
      if (verbose_openfile)
        cout << "reading this line: " << rff_conf_line << endl;



      ifstream map_file(inpfiles.GetMappingFileName());
      if (map_file.is_open()) {

    int i=-1;

    //while (rff.good()) {
    while (map_file.good()) {
                map_file >> map_station >> map_module >> map_start >> map_finish >> map_module1 >> map_start1 >> map_finish1;
                cout << "loop : " << map_station << endl;
                RPC tempRpc;
                tempRpc.map_station=map_station;
                tempRpc.map_module=map_module;
                tempRpc.map_start=map_start;
                tempRpc.map_finish=map_finish;
                tempRpc.map_module1=map_module1;
                tempRpc.map_start1=map_start1;
                tempRpc.map_finish1=map_finish1;
    	i++;
     	std::string chamber_tn,chamber_tw, chamber_b;
     	Precision curr1=0.,curr2=0.,curr3=0.;
     	Precision vset1=0.,vset2=0.,vset3=0.;
     	Precision vmon1=0.,vmon2=0.,vmon3=0.;
     	int station=0;
     	Precision feb[13]; // this should be double.

         getline(rff, rff_conf_line);
         //h.AddChambers();
         rff >> chamber_tn >> curr1 >> vset1 >> vmon1 >> station;
         rff >> chamber_tw >> curr2 >> vset2 >> vmon2 >> station;
         rff >> chamber_b >> curr3 >> vset3 >> vmon3 >> station;
         rff >> variablestring1 >> feb[1] >> feb[2] >> feb[3] >> feb[4] >> feb[5] >> feb[6] >> feb[7] >> feb[8] >>
                     feb[9] >> feb[10] >> feb[11] >> feb[12];

         //Trying to construct chamber name from GapName
         size_t pos;
         pos = chamber_tn.find("_tn");
         std::string chamberName = chamber_tn.substr(0,pos);

         //tempRpc.SetName(chamberName);
         tempRpc.Set(chamberName,etaPartition,Trigger_layer);
         tempRpc.SetGap(0,chamber_tn, curr1, vset1, vmon1, station);
         tempRpc.SetGap(1,chamber_tw, curr2, vset2, vmon2, station);
         tempRpc.SetGap(2,chamber_b, curr3, vset3, vmon3, station);
         tempRpc.SetFeb(0,feb[1],feb[2],feb[3],feb[4]);
         tempRpc.SetFeb(1,feb[5],feb[6],feb[7],feb[8]);
         tempRpc.SetFeb(2,feb[9],feb[10],feb[11],feb[12]);

         //r[i].Print();

         for(int j=0 ; j<tempRpc.GetGapVectorSize() ; j++)
         {
         Gap temp = tempRpc.GetGap(j);

         if (verbose_openfile)
            cout << "Gap: " << temp.GetName() << " current: " << temp.GetCurrent() << " vset: " << temp.GetVSet() << " vmon2: " <<
            temp.GetVMon() << " station=" << temp.GetStation() << endl;
         }
         cout << "FEB values: " ;
         for(int j=0; j<3;j++){
           int numOfChips = tempRpc.GetFeb(j).GetNumOfChips();
           for(int k=0; k<numOfChips ; k++){
             cout<<tempRpc.GetFeb(j).GetChip(k).GetThreshold()<<" ";
           }
         }
         std::cout<<std::endl<<std::endl;

         tempRpc.SetStatictics();
         fRpcVector.push_back(tempRpc);

       }
    }
    }
























    //RPC r = h.GetChamber(0);
    //r.Print();
    //std::cout<<r.GetGap(0)<<std::endl;
    //r.GetFeb(0).Print();


//    RPC* rpc = h.GetChamber(0);
//    rpc->Print();


} //end of ReadFilesAndFillData function

}//end of Tracking namespace


