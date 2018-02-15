
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

	    DetectorMapping();
	    DetectorMapping(std::string filename);

	public:

		~DetectorMapping();
		static DetectorMapping *instance();
		static DetectorMapping *create(std::string filename);

		int GetNumOfDetectors() const {return fNumOfDetectors;}
		void ReadMapping();
		std::vector<std::string> GetDetectorNamesVector() const {return fDetectorNameVector;}



};

} /* end of Tomography namespace*/

#endif /* DETECTORMAPPING_H_ */

