#include "DetectorMapping.h"
#include <fstream>
#include "GlassRpc.h"
#include "Properties.h"

typedef Tomography::Properties Detector;

namespace Tomography{

DetectorMapping *DetectorMapping::s_instance = 0;
DetectorMapping* DetectorMapping::instance() {
        if (!s_instance){
          std::cout << "DetectorMapping Object does not exist, so creating a new one with default filename 'mapping.txt' .... " << std::endl;
          s_instance = new DetectorMapping;
        }
        return s_instance;
    }


DetectorMapping* DetectorMapping::create(std::string filename) {

	if (!s_instance){
    	s_instance = new DetectorMapping(filename);

    }else{
    	s_instance->SetFileName(filename);
    	s_instance->fDetectorVector.clear();
    	// s_instance->fModuleVector.clear();
    	// s_instance->fDetectorNameVector.clear();
    	// s_instance->fStartingChannelVector.clear();
    	// s_instance->fZCoordinateVector.clear();

    	//clearing efficiency vectors
    	s_instance->fEfficienyTopVector.clear();
    	s_instance->fEfficienyBottomVector.clear();
    	s_instance->fEfficienyBothVector.clear();
    	s_instance->fVoltageVector.clear();

    }
    return s_instance;
}


//Daq code will use this, coz the name of mapping file there
//will be mapping.txt
DetectorMapping::DetectorMapping(){
	fileName = "mapping.txt";
	ReadMapping();

}

//This will be used by TestProgram or Other program, if the
//name of mapping file is different
DetectorMapping::DetectorMapping(std::string filename){
	fileName = filename;
	ReadMapping();
}

void DetectorMapping::CalculateScattererExtent(){
	for(int i = 0 ; i < fScattererVector.size() ; i++){
		scattererMin.push_back(Tracking::Vector3D<double>(fScattererVector[i]->sLocation.x()-fScattererVector[i]->sDim.x(),
				                                          fScattererVector[i]->sLocation.y()-fScattererVector[i]->sDim.y(),
														  fScattererVector[i]->sLocation.z()-fScattererVector[i]->sDim.z()));

		scattererMax.push_back(Tracking::Vector3D<double>(fScattererVector[i]->sLocation.x()+fScattererVector[i]->sDim.x(),
                										  fScattererVector[i]->sLocation.y()+fScattererVector[i]->sDim.y(),
														  fScattererVector[i]->sLocation.z()+fScattererVector[i]->sDim.z()));
	}
}

void DetectorMapping::ReadMapping(){
	//std::cout<<"FileName from ReadMapping : " << fileName << std::endl;
	std::ifstream in(fileName);
	int count = -7 ;
	int detCounter = 0;
	while(1){

		if(!in.good()) break;
		std::string detectorName = "";
		std::string detectorType = "";
		int module = 0;
		int channelStart = 31;
		double zCoordinate = 0.;

		//reading Scatterer info
		if(count == -7){
			count++;
			in >> fNumOfScatterersString >> fNumOfScatterers;
			std::cout << "fNumOfScatterersString : " << fNumOfScatterersString << " : fNumOfScatterers : " << fNumOfScatterers << std::endl;
			for(int i = 0 ; i < fNumOfScatterers ; i++){
				std::string scattererName, scattererMaterial;
				double dimX,dimY,dimZ,locX,locY,locZ;
				in >> scattererName >> scattererMaterial >> dimX >> dimY >> dimZ >> locX >> locY >> locZ ;
				std::cout << scattererName << " : " << scattererMaterial << " : " << dimX << ", " << dimY << ", " << dimZ <<" :: " << locX << ", " <<locY << ", " << locZ <<std::endl;
				fScattererVector.push_back(new Mapping::Scatterer(scattererName,scattererMaterial,Tracking::Vector3D<double>(dimX,dimY,dimZ),Tracking::Vector3D<double>(locX,locY,locZ)));
			}
		}

		//reading NumOfStripsInEachPlane;
		if(count == -6){
			count++;
			in >> fStartStripNumOfEachDetectorString >> fStartStripNumOfEachDetector;
			continue;
		}


		//reading NumOfStripsInEachPlane;
		if(count == -5){
			count++;
			in >> fNumOfStripsInEachPlaneString >> fNumOfStripsInEachPlane;
			continue;
		}

		//reading WorldHalfLength;
		if(count == -4){
			count++;
			in >> fWorldSizeString >> fWorldHalfLength;
			continue;
		}

		//reading GunZ
		if(count == -3){
			count++;
			in >> fGunString >> fGunZ; 
			continue;
		}

		//reading numOfEvents
		if(count == -2){
			std::string numOfEvt;
			count++;
			in >> numOfEvt >> fMaxNumOfEvents;
			Tomography::EventBreak::instance()->SetMaxNumOfEvents(fMaxNumOfEvents);
			continue;
		}

		//std::string plotsLocation;
		if(count == -1){
			count++;
			in >> fPlotsLocation >> fPlotsLocation;
			continue;
		}
		if(count == 0){
			count++;
			std::string lmodule, lchannelstart, lzcoordinate;
			in >> detectorName >> lmodule >> lchannelstart >> lzcoordinate >> detectorType;
			continue;
		}
		
		if(count==1){
			detCounter++;
			in >> detectorName >> module >> channelStart >> zCoordinate >> detectorType;
			// fDetectorNameVector.push_back(detectorName);
			// fModuleVector.push_back(module);
			// fStartingChannelVector.push_back(channelStart);
			// fZCoordinateVector.push_back(zCoordinate);

			//Filling the Structure
			fDetectorVector.push_back(new Mapping::Detector(detectorName,zCoordinate,channelStart,
														    module,detectorType));
		}
	}
	std::cout << "Stack Length from ReadMapping : " << detCounter << std::endl;
	//fNumOfDetectors = fDetectorNameVector.size();
	fNumOfDetectors = fDetectorVector.size();
	std::cout << "Num Of Detector from ReadMapping : " << fNumOfDetectors << std::endl;

	//Calculating the Extent vector
	CalculateScattererExtent();
}

void DetectorMapping::ReadDaqInfo(std::string daqfile){
	std::string rootfile = daqfile+".root";
	std::string run_config_file = daqfile+".info";
	std::ifstream rff(run_config_file);
	bool verbose_openfile = false;
	if (rff.is_open()) {
		if (verbose_openfile)
			std::cout << "Run config file: " << run_config_file << " is opened."
					<< std::endl;

		// This is the header of the daqinfo file
		std::string variablestring1;
		double temperature = 0., pressure = 0., humidity = 0.;
		rff >> variablestring1 >> temperature >> humidity >> pressure;
		fTemp = temperature;
		fPressure = pressure;
		fHumidity = humidity;

		if (verbose_openfile)
			std::cout << "Temp: " << temperature << " humidity: " << humidity << " pressure: " << pressure << std::endl;

		std::string variablestring2, variablestring3, Trigger_layer;
		char etapartition;
		rff >> variablestring2 >> etapartition >> variablestring3
				>> Trigger_layer;
		if (verbose_openfile)
			std::cout << "Eta partition: " << etapartition << " trigger: " << Trigger_layer << std::endl;

		std::string rff_conf_line;
		getline(rff, rff_conf_line);
		if (verbose_openfile)
			std::cout << "reading this line: " << rff_conf_line << std::endl;

		getline(rff, rff_conf_line);
		if (verbose_openfile)
			std::cout << "reading this line: " << rff_conf_line << std::endl;

		std::string chamber_tn, chamber_tw, chamber_b;
		float curr1 = 0., vset1 = 0., vmon1 = 0., curr2 = 0., vset2 = 0., vmon2 = 0., curr3 = 0., vset3 = 0., vmon3 = 0.;
		int station = 0, feb[13];
		int i=0;
		//while (rff.good()) {
		for(int i = 0 ; i < fNumOfDetectors-1 ; i++ ){
			getline(rff, rff_conf_line);
			//getline(rff, rff_conf_line);
			rff >> chamber_tn >> curr1 >> vset1 >> vmon1 >> station;
			rff >> chamber_tw >> curr2 >> vset2 >> vmon2 >> station;
			rff >> chamber_b >> curr3 >> vset3 >> vmon3 >> station;
			fVoltageVector.push_back(vset1);

			std::cout << chamber_tn << " : " << curr1 << std::endl;

			//Logic to calculate efficieny using new Analysis code
			//std::cout<<"Size of DetectorNameVector : " << (fDetectorNameVector.size()-1) << std::endl;
  		    //for(int i = 0 ; i < fDetectorNameVector.size()-1; i++){
			if (verbose_openfile)
				std::cout << "Name of Detector : " << (i+1) << " : " << fDetectorNameVector[i] << " : " << fModuleVector[i] << " : " << fStartingChannelVector[i] << std::endl;
			  	Detector *rpc = new GlassRpc(fModuleVector[i], fDetectorNameVector[i], fZCoordinateVector[i], fStartingChannelVector[i]);
			  	rpc->SetClusterSize(1);
			  	Tracking::Tree::instance()->ReadTree(rootfile.c_str(), "BSC_DATA_TREE", 0);
			  	rpc->SetEfficiency();
			  	rpc->GetPlane(0)->SetEfficiency();
			  	rpc->GetPlane(1)->SetEfficiency();
			  	//std::cout << "EffPlaneX : " << rpc->GetPlane(0)->GetEfficiency() << std::endl;
			  	//std::cout << "EffPlaneY : " << rpc->GetPlane(1)->GetEfficiency() << std::endl;
			  	//std::cout << "EffPlaneBoth : " << rpc->GetEfficiency() << std::endl;
			  	fEfficienyTopVector.push_back(rpc->GetPlane(0)->GetEfficiency());
			  	fEfficienyBottomVector.push_back(rpc->GetPlane(1)->GetEfficiency());
			  	fEfficienyBothVector.push_back(rpc->GetEfficiency());
			  	delete rpc;
			//}

			rff >> variablestring1 >> feb[1] >> feb[2] >> feb[3] >> feb[4]
					>> feb[5] >> feb[6] >> feb[7] >> feb[8] >> feb[9] >> feb[10]
					>> feb[11] >> feb[12];

			if (verbose_openfile)
				std::cout << " " << std::endl << " " << std::endl;
			if (verbose_openfile)
				std::cout << "Gap: " << chamber_tn << " current: " << curr1
						<< " vset: " << vset1 << " vmon1: " << vmon1
						<< " station=" << station << std::endl;
			if (verbose_openfile)
				std::cout << "Gap: " << chamber_tw << " current: " << curr2
						<< " vset: " << vset2 << " vmon2: " << vmon2
						<< " station=" << station << std::endl;
			if (verbose_openfile)
				std::cout << "Gap: " << chamber_b << " current: " << curr3
						<< " vset: " << vset3 << " vmon3: " << vmon3
						<< " station=" << station << std::endl;
			if (verbose_openfile)
				std::cout << "FEB values: " << feb[1] << " " << feb[2] << " "
						<< feb[3] << " " << feb[4] << " " << feb[5] << " "
						<< feb[6] << " " << feb[7] << " " << feb[8] << " "
						<< feb[9] << " " << feb[10] << " " << feb[11] << " "
						<< feb[12] << std::endl;

			//i++;
			//if(i==2)
				//break;
		}
	}

}

void DetectorMapping::PrintEfficiencyVector() const {

	bool verbose = false;

	for(int i = 0 ; i < fEfficienyBothVector.size() ; i++){
	std::ofstream outfile(fDetectorNameVector[i]+"-Eff.txt",std::ios::app);

	//for(int i = 0 ; i < fEfficienyBothVector.size() ; i++){
		outfile << fDetectorNameVector[i] << " " <<fVoltageVector[i] << " " << fTemp << " " << fPressure << " " << fHumidity
				<< " "<< fEfficienyTopVector[i] << " " << fEfficienyBottomVector[i] << " " << fEfficienyBothVector[i] << std::endl;
		if(verbose)
			std::cout << "Eff-Top : " << fEfficienyTopVector[i] <<" : Eff-Bottom : " << fEfficienyBottomVector[i] << " : Eff-Both : " << fEfficienyBothVector[i] << std::endl;

	//}
	outfile.close();
	}
}


int DetectorMapping::GetNumOfDetectors(std::string type){
	int numOfDetectors = 0;
	for(int i = 0 ; i < fDetectorVector.size() ; i++){
		if(fDetectorVector[i]->sDetectorType == type){
			numOfDetectors++;
		}
	}
	return numOfDetectors;
}

std::vector<std::string> DetectorMapping::GetDetectorNamesVector(std::string type){
	std::vector<std::string> detNamesVector;
	for(int i = 0 ; i < fDetectorVector.size() ; i++){
			if(fDetectorVector[i]->sDetectorType == type){
				detNamesVector.push_back(fDetectorVector[i]->sDetectorName);
			}
	}

	return detNamesVector;
}

}

