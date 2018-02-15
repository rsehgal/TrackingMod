
#ifndef TEMPLATIZEDINTERFACE_INC_DETECTORMAPPING_H_
#define TEMPLATIZEDINTERFACE_INC_DETECTORMAPPING_H_

#include <vector>
#include <string>
#include <iostream>

namespace Tomography{


class DetectorMapping{
	
	private:
		std::string fileName;
		int fNumOfDetectors;
	    static DetectorMapping *s_instance;
	    std::vector<std::string> fDetectorNameVector;
	    std::vector<int> fModuleVector;
	    std::vector<int> fStartingChannelVector;
	    std::vector<double> fZCoordinateVector;

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

		std::string GetPlotsLocation() const {return fPlotsLocation;}

		//Function to read information from runnumber.daq file
		void ReadDaqInfo(std::string daqfile);


		//Some Printing function basically for debugging
		void PrintEfficiencyVector() const;


};

} /* end of Tomography namespace*/

#endif /* DETECTORMAPPING_H_ */

