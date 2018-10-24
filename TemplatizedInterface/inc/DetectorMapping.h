
#ifndef TEMPLATIZEDINTERFACE_INC_DETECTORMAPPING_H_
#define TEMPLATIZEDINTERFACE_INC_DETECTORMAPPING_H_

#include <vector>
#include <string>
#include <iostream>
#include "base/Vector3D.h"

namespace Tomography{

namespace Mapping{
struct Detector{
	std::string sDetectorName;
	std::string sDetectorType;
	double sZCoordinate;
	int sStartingChannel;
	int sModule;
	double sDetectorLength;
	double sDetectorBreadth;

	Detector(std::string detName,  double zCoord, 
			int  startChannel, int module, std::string detType = "GLASS"):
			sDetectorLength(1000.),sDetectorBreadth(1000.),
			sDetectorName(detName),sDetectorType(detType),sZCoordinate(zCoord)
			,sStartingChannel(startChannel), sModule(module){}

};

struct Scatterer{
	std::string sName;
	std::string sMaterial;
	Tracking::Vector3D<double> sDim;
	Tracking::Vector3D<double> sLocation;

	Scatterer(std::string name, std::string material, Tracking::Vector3D<double> dim, Tracking::Vector3D<double> location) :
		sName(name), sMaterial(material), sDim(dim), sLocation(location){}
};
}

class DetectorMapping{
	
	private:
		std::string fNumOfStripsInEachPlaneString;
		int fNumOfStripsInEachPlane;
		std::string fStartStripNumOfEachDetectorString;
		int fStartStripNumOfEachDetector;
	    std::string fWorldSizeString;
	    double fWorldHalfLength;
		std::string fGunString;
		double fGunZ;
		int fMaxNumOfEvents;

		//For Scatterer
		std::string fNumOfScatterersString;
		int fNumOfScatterers;
		std::vector<Mapping::Scatterer*> fScattererVector;
		std::vector<Tracking::Vector3D<double>> scattererMin;
		std::vector<Tracking::Vector3D<double>> scattererMax;


		std::string fileName;
		int fNumOfDetectors;
	    static DetectorMapping *s_instance;
	    std::vector<std::string> fDetectorNameVector;
	    std::vector<int> fModuleVector;
	    std::vector<int> fStartingChannelVector;
	    std::vector<double> fZCoordinateVector;

	    //New Structure
	    std::vector<Mapping::Detector*> fDetectorVector;


	    //Location to store all the plots
	    std::string fPlotsLocation ;

	    //private constructors for singleton class
	    DetectorMapping();
	    DetectorMapping(std::string filename);


	    //Data members for Efficiency values
	    std::vector<double> fEfficienyTopVector;
	    std::vector<double> fEfficienyBottomVector;
	    std::vector<double> fEfficienyBothVector;
	    std::vector<double> fVoltageVector;

	    //Data member for Temp, Pressure and Humidity
	    float fTemp = 0.;
	    float fPressure = 0.;
	    float fHumidity = 0.;


	public:

		~DetectorMapping();
		static DetectorMapping *instance();
		static DetectorMapping *create(std::string filename);

		void SetFileName(std::string filename) {fileName = filename;}
		int GetNumOfDetectors() const {return fNumOfDetectors;}
		void ReadMapping();
		std::vector<std::string> GetDetectorNamesVector() const {return fDetectorNameVector;}
		std::vector<int> GetModuleVector() const {return fModuleVector;}
		std::vector<int> GetStartingChannelVector() const {return fStartingChannelVector;}
		std::vector<double> GetZCoordinateVector() const {return fZCoordinateVector;}

		//This will return the full DetectorMap
		std::vector<Mapping::Detector*>  GetDetectorVector() const { return fDetectorVector; }

		//Get Scatterer Vectgor
		std::vector<Mapping::Scatterer*>  GetScattererVector() const { return fScattererVector; }

		//Calculate Extent of Scatterer
		void CalculateScattererExtent();

		std::vector<Tracking::Vector3D<double>> GetScattererMinExtent() const {return scattererMin;}
		std::vector<Tracking::Vector3D<double>> GetScattererMaxExtent() const {return scattererMax;}

		std::string GetPlotsLocation() const {return fPlotsLocation;}

		//Function to read information from runnumber.daq file
		void ReadDaqInfo(std::string daqfile);

		//Some Printing function basically for debugging
		void PrintEfficiencyVector() const;

		//GetZ location of Gun
		double GetGunZ() const {return fGunZ;}

		//Get WorldHalfLength
		double GetWorldHalfLength() const {return fWorldHalfLength;}

		//Get NumberOfStripsInEachPlane
		int GetNumberOfStripsInEachPlane() const {return fNumOfStripsInEachPlane;}

		//Get StartStripNumOfEachDetector
		int GetStartStripNumOfEachDetector() const {return fStartStripNumOfEachDetector;}

		int GetNumOfDetectors(std::string type);
		std::vector<std::string> GetDetectorNamesVector(std::string type);



};

} /* end of Tomography namespace*/

#endif /* DETECTORMAPPING_H_ */

