/*
 * Setup.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: rsehgal
 */

#include "Setup.h"
#include "XmlDomDocument.h"
#include <iostream>
#include "Properties.h"
#include "Scintillator.h"
#include "ScintillatorPlane.h"

using RealDetector = Tomography::Properties;

namespace Tomography {

Setup::Setup(std::string filename) {
	// TODO Auto-generated constructor stub
	fXmlFileName=filename; // Hardcoding for the time being
	fDoc = new XmlDomDocument(fXmlFileName.c_str());
	FillInfo();
	CreateTriggeringPlaneVector();
	CreateDetectorVector();

}

Setup::~Setup() {
	// TODO Auto-generated destructor stub
}

void Setup::CreateTriggeringPlaneVector(){
	//Read XML and create Triggering plane by creating object of DetectorFactory and passing true as last arguement

}

void Setup::CreateDetectorVector(){
	//Read XML and create detectors


}



void Setup::FillInfo(){

	bool verbose=false;
	//Used to fill Triggering, cluster size and other information
	std::string value = fDoc->getChildValue("setup", 0, "clustersize", 0);
	fClusterSize = std::stoi(value);
	value = fDoc->getChildAttribute("setup",0,"trigger",0,"tdcid");
	fTrigTdc = std::stoi(value);
	//assert(fTrigTdc==1);
	value = fDoc->getChildAttribute("setup",0,"trigger",0,"tdcchannel");
	fTrigChannel = std::stoi(value);
	//assert(fTrigChannel==31);

	if(verbose){
		std::cout<<"--- Entered FillInfo ---- " << std::endl;
		std::cout<<"ClusterSize : "<< fClusterSize << std::endl;
		std::cout<<"TDC for Trigger : " << fTrigTdc << std::endl;
		std::cout<<"TDC Channel for Trigger : " << fTrigChannel << std::endl;
	}

	//Now trying to create Detector and Triggering plane and fill in corresponding vectors

	//Just filling Triggering plane and Detector vector,
	for (int i = 0; i < fDoc->getChildCount("setup", 0, "triggeringplane"); i++) {
		fTriggeringPlaneVect.push_back(new RealDetector);
	}

	for (int i = 0; i < fDoc->getChildCount("setup", 0, "rpc"); i++) {
		fDetectorVect.push_back(new RealDetector);
	}

	//Modifying default TriggeringPlanes
	for (int i = 0; i < fDoc->getChildCount("setup", 0, "triggeringplane"); i++) {
		value= fDoc->getChildAttribute("setup",0,"triggeringplane",i,"tdcchannelStart");
		int startId = std::stoi(value);
		value= fDoc->getChildAttribute("setup",0,"triggeringplane",i,"zpos");
		int zPos=std::stoi(value);
		value=fDoc->getChildAttribute("setup",0,"triggeringplane",i,"length");
		double length=std::stod(value);
		value=fDoc->getChildAttribute("setup",0,"triggeringplane",i,"breadth");
		double breadth = std::stod(value);
		value=fDoc->getChildAttribute("setup",0,"triggeringplane",i,"height");
		double height = std::stod(value);
		value=fDoc->getChildAttribute("setup",0,"triggeringplane",i,"tdcid");
		int moduleId = std::stoi(value);
		value=fDoc->getChildAttribute("setup",0,"triggeringplane",i,"nofstrips");
		int numOfStrips = std::stoi(value);
		value=fDoc->getChildAttribute("setup",0,"triggeringplane",i,"name");
		std::string planeName=value;
		value= fDoc->getChildAttribute("setup",0,"rpc",i,"dettype");
		std::string detType = value;

		//Now modifying detector properties
		fTriggeringPlaneVect[i]->SetDetectorType(detType);
		Scintillator::SetStartingId(startId);
		Scintillator::SetStartingStripNum();
		fTriggeringPlaneVect[i]->SetNumOfPlanes(1);
		fTriggeringPlaneVect[i]->SetName(planeName);
		fTriggeringPlaneVect[i]->SetZPos(zPos);
		fTriggeringPlaneVect[i]->SetLBH(length,breadth,height);
		fTriggeringPlaneVect[i]->Initialize();
		fTriggeringPlaneVect[i]->InsertPlane(new ScintillatorPlane(moduleId,numOfStrips,zPos,fTriggeringPlaneVect[i]->GetLength(),fTriggeringPlaneVect[i]->GetBreadth(),fTriggeringPlaneVect[i]->GetHeight(),0.,false));
		fTriggeringPlaneVect[i]->SetClusterSize(fClusterSize);

	}


	//Modifying default Detector
	for (int i = 0; i < fDoc->getChildCount("setup", 0, "rpc"); i++) {

		int moduleId = 0;
		int numOfStrips = 0;
		int startId = 0;
		std::string name;
		value = fDoc->getChildAttribute("setup", 0, "rpc", i, "name");
		std::string detName = value;
		std::cout<<"Creating detector : " <<  detName << std::endl;
		value = fDoc->getChildAttribute("setup", 0, "rpc", i, "zpos");
		int zPos = std::stoi(value);
		value = fDoc->getChildAttribute("setup", 0, "rpc", i, "length");
		double length = std::stod(value);
		value = fDoc->getChildAttribute("setup", 0, "rpc", i, "breadth");
		double breadth = std::stod(value);
		value = fDoc->getChildAttribute("setup", 0, "rpc", i, "height");
		double height = std::stod(value);

		value = fDoc->getChildAttribute("setup", 0, "rpc", i, "dettype");
		std::string detType = value;
		//Now modifying detector properties
		fDetectorVect[i]->SetDetectorType(detType);
		Scintillator::SetStartingId(startId);
		fDetectorVect[i]->SetNumOfPlanes(2);
		fDetectorVect[i]->SetName(detName);
		fDetectorVect[i]->SetZPos(zPos);
		fDetectorVect[i]->SetLBH(length, breadth, height);
		fDetectorVect[i]->Initialize();
		for (int j = 0; j < fDoc->getChildCount("rpc", i, "plane"); j++) {
			value = fDoc->getChildAttribute("rpc", i, "plane", j, "name");
			name = value;
			value = fDoc->getChildAttribute("rpc", i, "plane", j, "tdcid");
			moduleId = std::stoi(value);
			value = fDoc->getChildAttribute("rpc", i, "plane", j, "nofstrips");
			numOfStrips = std::stoi(value);
			value = fDoc->getChildAttribute("rpc", i, "plane", j,
					"tdcchannelStart");
			startId = std::stoi(value);
			//std::cout<<"Inserting " << (j+1) <<"st Plane" << std::endl;
			if (j == 0) {

				Scintillator::SetStartingId(startId);
				Scintillator::SetStartingStripNum();
				//std::cout<<"Starting Channel num of plane : " << Scintillator::GetStartingId() << std::endl;
				fDetectorVect[i]->InsertPlane(
						new ScintillatorPlane(moduleId, numOfStrips, zPos,
								fDetectorVect[i]->GetLength(),
								fDetectorVect[i]->GetBreadth(),
								fDetectorVect[i]->GetHeight(), 0., true,name));
			} else {
				//std::cout<<"Starting Channel num of plane : " << Scintillator::GetStartingId() << std::endl;
				fDetectorVect[i]->InsertPlane(
						new ScintillatorPlane(moduleId, numOfStrips, zPos,
								fDetectorVect[i]->GetLength(),
								fDetectorVect[i]->GetBreadth(),
								fDetectorVect[i]->GetHeight(), 0., false,name));
			}
		}
		fDetectorVect[i]->SetClusterSize(fClusterSize);

	}



}

} /* namespace Tomography */
