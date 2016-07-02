/*
 * TestMultihit.cpp
 *
 *  Created on: Jun 27, 2016
 *      Author: rsehgal
 */

#include <iostream>
#include <TriggeringPlane.h>
#include <Properties.h>
#include <SetupManager.h>
#include <cstring>
#include "base/Global.h"
typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]){
	std::string temp_str = std::to_string(atoi(argv[1]));
	temp_str += ".root";
	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
	SetupManager *setup = SetupManager::instance();
	Detector *MT1=new GlassRpc(2,"MT1",-75,31);
	Detector *MT2=new GlassRpc(4,"MT2",45,31);
	Detector *TopScint=new TriggeringPlane(2,"TopScint",105,-1);
	Detector *BottomScint=new TriggeringPlane(2,"BottomScint",-105,7);
	int k;
	int topcount=0, bottomcount=0;
	int numerator=0, denominator=0;
	bool shower,rpcFired;
	int NumberOfStripsFired;
	setup->Register(MT1);
	setup->Register(MT2);
	setup->Register(TopScint);
	setup->Register(BottomScint);
	std::vector<Detector*> detVect=setup->GetDetectorVector("GLASS");
	int showercount=0;
	int ClusterSize[4];
	std::vector<Detector*> trgVect=setup->GetDetectorVector("TRG");
	int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
	for(int evNo=0;evNo<numOfEvents;++evNo) {
//		std::cout<<"======================= Event no. : "<<evNo<<"===================="<<std::endl;
		setup->SetEventDetected("TRG",evNo);
		shower=false;

		//Printing events with Multihit
		for(int i=0;i<trgVect.size();++i) {
			for(int j=0;j<trgVect[i]->GetNumOfPlanes();++j) {

                          for (int k = 0; k < trgVect[i]->GetPlane(j)->GetNumOfScintillators(); k++) {
                            std::string bName = trgVect[i]->GetPlane(j)->GetScintVector()[k]->GetName();
                            Channel *ch = Tracking::Tree::instance()->GetEntry(bName, evNo);
                            if(ch->size()>1){
                            std::cout <<"Event No : "<< evNo << " : Branch Name : " << bName << " : Multihit VectorSize : " << ch->size()
                                      << " Values : "; //<<std::endl;
                            for (int l = 0; l < ch->size(); l++) {
                              std::cout << ch->at(l) << ", ";
                            }
                            std::cout << std::endl;
                            }
                          }
                        }

		}
	}
		std::cout<<"================================ Mulitihit Printing Done ==========================" << std::endl;
		int count = 0 ;
		for(int evNo=0;evNo<numOfEvents;++evNo) {
			setup->SetEventDetected("TRG",evNo);
		for(int i=0;i<trgVect.size();++i) {

			for(int j=0;j<trgVect[i]->GetNumOfPlanes();++j) {
				bool fired = false;
				//NumberOfStripsFired=trgVect[i]->GetPlane(j)->GetFiredStripsVector().size();
				fired = trgVect[i]->GetPlane(j)->GetFiredStripsVector().size();

				//Printing if some event are there for which either of Scintillators in ScintillatorPlane NOT fired
				if(!fired){
					count++;
					std::cout<<"---- For Event : " << evNo << " : TrgPlane No : "<< i << " : NOT fired " << std::endl;
					for(int k = 0 ; k<trgVect[i]->GetPlane(j)->GetNumOfScintillators() ; k++){
						std::string bName = trgVect[i]->GetPlane(j)->GetScintVector()[k]->GetName();
						Channel *ch = Tracking::Tree::instance()->GetEntry(bName,evNo);
						std::cout<<"Branch Name : "<< bName <<" : Multihit VectorSize : "<<ch->size()<<" Values : ";//<<std::endl;
						for(int l = 0 ; l < ch->size() ; l++){
							std::cout<<ch->at(l)<<", ";
						}
						std::cout<<std::endl;

					}

				}
			}
		}
	}
		if(count)
			std::cout<<std::endl<<"***************** Something Unexpected Happening ***********************" << std::endl<<std::endl;
		else
			std::cout<<std::endl<<"---------- GR8888 , Program running as Expected --------------" << std::endl << std::endl;

}
