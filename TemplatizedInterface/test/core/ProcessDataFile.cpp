/*
 * ProcessDataFile.cpp
 *
 *  Created on: Feb 27, 2018
 *      Author: rsehgal
 *
 *  This code will take RunNumber as input, and process the
 *  corresponding data file, and will create following
 *
 *  1) Angular distribution histogram.
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "EventProcessor.h"
#include "EventHelper.h"
#include "RunHelper.h"
#include "Tree.h"
#include "CommonFunc.h"
#include "Properties.h"
#include "GlassRpc.h"
#include "DetectorMapping.h"
#include "SetupManager.h"
#include <TH1F.h>
#include <G4SystemOfUnits.hh>
#include "Fit2DLinear.h"

typedef Tomography::Properties Detector;

int main(int arc, char *argv[]){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	int nxbins = 200;
	TH1F *hist = new TH1F("Data", "Data", nxbins, -0.5, 1.5);

	std::string temp_str = std::to_string(atoi(argv[1]));
	temp_str += ".root";
	int clusterSize = 1;
	std::ofstream fs("MuonAngles.txt");

	Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
	std::vector<std::string> detectorNamesVector = detectorMap->GetDetectorNamesVector();
	std::vector<int> startChannelVector = detectorMap->GetStartingChannelVector();
	std::vector<int> moduleVector = detectorMap->GetModuleVector();
	std::vector<double> zcoordinateVector = detectorMap->GetZCoordinateVector();

	for(int i = 0 ; i < detectorNamesVector.size()-1; i++){
	    std::cout << "Name of Detector : " << (i+1) << " : " << detectorNamesVector[i] << std::endl;
	    Detector *rpc = new Tomography::GlassRpc(moduleVector[i], detectorNamesVector[i], zcoordinateVector[i], startChannelVector[i]);
	    rpc->SetClusterSize(1);
	}

	fs << 3000 << " " << "G4_Fe" << " " << 100 << " ";

	Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
	int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();

	/* We need EventProcess, because this guy will
	 * will give us the hit point vector for every
	 * event.
	 */
	Tomography::EventProcessor eventProcessor;
	Tomography::SetupManager *setup = Tomography::SetupManager::instance();
	//numOfEvents = 2;
#define RECONSTRUCT
#ifdef RECONSTRUCT
	Tomography::evolution::Voxelator::Create(50*cm,50*cm,45*cm,10*cm,10*cm,9*cm);
#endif

	Tomography::Fit2DLinear fitter;
	int detectedEventCount = 0;
	int deviatedMuonCounter = 0;
	int undeviatedMuonCounter = 0;

	//numOfEvents = 3;
	//Opening a file for storing fitted hit points
	std::ofstream fitHitFile("FitHitPlot.txt");


	for (int evNo = 0; evNo < numOfEvents; evNo++) {
		//std::cout << "=============== Event No : " << evNo << " =========================== " << std::endl;
		eventProcessor.ProcessEvent(evNo);
		if(setup->EventDetected()){
			std::cout << "==============  Event Detected..... : EvNo : " << evNo << "  ==================" <<  std::endl;
			detectedEventCount++;


		/*Tomography::Track incoming = eventProcessor.GetIncomingTrack();
		//Tomography::Track outgoing = eventProcessor.GetOutgoingTrack();
		Tomography::Track ref(G4ThreeVector(0.,0.,0.),G4ThreeVector(0.,0.,-1.));
		//incoming.Print();
		double angleIncoming = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,ref);
		hist->Fill(angleIncoming);
		fs << angleIncoming << " ";
		*/

		std::vector<Tracking::Vector3D<double>> hitPointVector = eventProcessor.GetHitPointVector();
		std::cout<<"Hit Point Vector Size : " << hitPointVector.size() << std::endl;

		//Copied from inside of some ifdef and kept here outside
		Tomography::Track incoming, outgoing;
		int numOfDetectors = detectorNamesVector.size();

#define USE_FITTED_TRACK
#ifdef USE_FITTED_TRACK


#ifdef SCATTERER_PLACED
		std::vector<Tracking::Vector3D<double>> incomingHitPointVector;
		std::vector<Tracking::Vector3D<double>> outgoingHitPointVector;
		int size = hitPointVector.size();
		for(int i = 0 ; i < size ; i++){
			if(i < size/2){
				incomingHitPointVector.push_back(hitPointVector[i]);
			}else{
				outgoingHitPointVector.push_back(hitPointVector[i]);
			}
		}

		std::vector<Tracking::Vector3D<double>> fittedIncomingHitPointVector = fitter.GetFittedTrack(incomingHitPointVector);
		std::vector<Tracking::Vector3D<double>> fittedOutgoingHitPointVector = fitter.GetFittedTrack(outgoingHitPointVector);
		Tomography::Track incoming(fittedIncomingHitPointVector[0],fittedIncomingHitPointVector[fittedIncomingHitPointVector.size()-1]);
		Tomography::Track outgoing(fittedOutgoingHitPointVector[0],fittedOutgoingHitPointVector[fittedOutgoingHitPointVector.size()-1]);

		for(int i = 0 ; i < fittedIncomingHitPointVector.size() ; i++){
			std::cout << incomingHitPointVector[i].x() << "  " << incomingHitPointVector[i].y() << "  " << incomingHitPointVector[i].z() << "   ::  "
					 << fittedIncomingHitPointVector[i].x() << "  " << fittedIncomingHitPointVector[i].y() << "  " << fittedIncomingHitPointVector[i].z() << std::endl;
		}

		for(int i = 0 ; i < fittedOutgoingHitPointVector.size() ; i++){
			std::cout << outgoingHitPointVector[i].x() << "  " << outgoingHitPointVector[i].y() << "  " << outgoingHitPointVector[i].z() << "   ::  "
					 << fittedOutgoingHitPointVector[i].x() << "  " << fittedOutgoingHitPointVector[i].y() << "  " << fittedOutgoingHitPointVector[i].z() << std::endl;
		}


		incomingHitPointVector.clear();
		outgoingHitPointVector.clear();
#else //Of IF OF SCATTERER_PLACED

		for(int i = 0 ; i < hitPointVector.size() ; i++)
			hitPointVector[i].SetX(hitPointVector[i].x()-30.);
		std::vector<Tracking::Vector3D<double>> fittedHitPointVector = fitter.GetFittedTrack(hitPointVector);
		for(int i = 0 ; i < fittedHitPointVector.size() ; i++){
			std::cout << "HitPoint : " << hitPointVector[i].x() << "  " << hitPointVector[i].y() << "  " << hitPointVector[i].z()
					  << "   :: Fitted :  "
					  << fittedHitPointVector[i].x() << "  " << fittedHitPointVector[i].y() << "  " << fittedHitPointVector[i].z()
					  << std::endl;

		}
		fitHitFile << fittedHitPointVector[1].x() << " " << fittedHitPointVector[1].y() << std::endl;

		//return 0;
		continue;

#endif // of SCATTERER_PLACED

#else //Of IF OF USE_FITTED_TRACK
		//Tomography::Track incoming, outgoing;
		/* NOTE : Keep a watch here, currently we are hardcoding the number of detectors here
		 * , which should actually be calculated or set using DetectorMapping
		 */
//		int numOfDetectors = 6;
		if(numOfDetectors==3){
			incoming.Set(hitPointVector[0],hitPointVector[1]);
			outgoing.Set(hitPointVector[1],hitPointVector[2]);
			//Tomography::EventHelper u(incoming,outgoing);
		}else{
			incoming.Set(hitPointVector[0],hitPointVector[hitPointVector.size()/2-1]);
			outgoing.Set(hitPointVector[hitPointVector.size()/2],hitPointVector[hitPointVector.size()-1]);
		}
#endif // of USE_FITTED_TRACK

		/* NOTE : "incoming" and "outgoing" MUST be set before reaching this point
		 *
		 */
		Tomography::Track ref(Tracking::Vector3D<double>(0.,0.,0.),Tracking::Vector3D<double>(0.,0.,-1.));
		//std::cout<<"INcoming vector : " ; incoming.Print();
		//std::cout<<"OUTgoing vector : " ; outgoing.Print();

		double inAngle = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,ref);
		double outAngle = CommonFunc::Functions::instance()->GetAngleInRadian(outgoing,ref);
		double deviation = CommonFunc::Functions::instance()->GetAngleInRadian(incoming,outgoing);
		std::cout << "Deviation  : " << deviation
				  << " : Incoming : " <<  inAngle
				  << " : Outgoing : " <<  outAngle
				  << " : Diff : " << (inAngle-outAngle) << std::endl;

		if(numOfDetectors == 6){

		// ONLY FOR SIX DETECTOR SETUP, either Simulation or Experiment

		/* Deviation check in air, which should be very very close to zero,
		** here we are allow deviation in air to have a maximum value of
		** 1e-6
		**
		** This is very much useful when we will have 6 detector in experiment
		** and this will help in chosing only those events where muon does not
		** suffer any scattering when travelling in air.
		*/

		//For incoming
		Tomography::Track in,out;
		in.Set(hitPointVector[0],hitPointVector[1]);
		out.Set(hitPointVector[1],hitPointVector[2]);
		double deviationIncoming =  CommonFunc::Functions::instance()->GetAngleInRadian(in,out);

		in.Set(hitPointVector[3],hitPointVector[4]);
		out.Set(hitPointVector[4],hitPointVector[5]);
		double deviationOutgoing =  CommonFunc::Functions::instance()->GetAngleInRadian(in,out);


		//if(deviation > 1e-6)
		bool deviatedMuon = (deviationIncoming > 1e-6 || deviationOutgoing > 1e-7);
		if(deviatedMuon)
			deviatedMuonCounter++;
		else
			undeviatedMuonCounter++;

		//Taking only undeviated events
		//if(deviation < 1e-6)
		if(!deviatedMuon){
			std::cout << "FOUND UNDEVIATED MUON..........." << std::endl;
			//Tomography::EventHelper u(incoming,outgoing);
		 }
		}else{
			//Tomography::EventHelper u(incoming,outgoing);
		}

		//break;
		}else{
			//std::cout << "Event NOT Detected....." << std::endl;
		}

		//if(detectedEventCount == 10)
			//					break;

	} // END OF EVENT LOOP

	//Closing the fitHitPlot.txt file
	fitHitFile.close();

	std::cout << "Total Number of Detected Events : " << detectedEventCount << std::endl;
	std::cout << "No of Unexpected deviation due to Mid point of Pixel are : " << deviatedMuonCounter << std::endl;
	std::cout << "No of Expected Undeviated Muon even after taking Mid point of Pixel are : " << undeviatedMuonCounter << std::endl;
//#undef RECONSTRUCT
//#define RECONSTRUCT
#ifdef RECONSTRUCT
   Tomography::RunHelper *runHelper = new Tomography::RunHelper();
  //Now trying to calculate Radiation for the whole run
  std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
  std::cout<<"\033[1;31m                              SD : " << CommonFunc::Functions::instance()->StandardDeviation(runHelper->GetScatteringAngleVector()) << "  radians for 2 sigma  \033[0m\n" << std::endl;
  //std::cout<<"                              SD : " << CommonFunc::Functions::instance()->StandardDeviation(b1Run->GetScatteringAngleVector()) << std::endl;
  std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
  std::cout<<"\033[1;31m                              RL : " << CommonFunc::Functions::instance()->RadiationLength(runHelper->GetScatteringAngleVector(),10) << "  cms  \033[0m\n" <<  std::endl;
  //std::cout<<"                              RL : " << CommonFunc::Functions::instance()->RadiationLength(b1Run->GetScatteringAngleVector(),10) << std::endl;
  std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
#endif

	hist->Draw();
	fs.close();


	fApp->Run();


}



