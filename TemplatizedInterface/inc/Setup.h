/*
 * Setup.h
 *
 *  Created on: Mar 22, 2017
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_SETUP_H_
#define TEMPLATIZEDINTERFACE_INC_SETUP_H_

#include <vector>
#include "Properties.h"
#include <string>

class XmlDomDocument;

namespace Tomography {

using Detector = Tomography::Properties;

class Setup {
	std::string fXmlFileName;
	XmlDomDocument *fDoc;
	std::vector<Detector*> fTriggeringPlaneVect;
	std::vector<Detector*> fDetectorVect;

	//Other info
	int fClusterSize;
	int fTrigTdc,fTrigChannel;


public:
	Setup(std::string filename);
	virtual ~Setup();
	void FillInfo();
	void CreateTriggeringPlaneVector();
	void CreateDetectorVector();
	std::vector<Detector*> GetTriggeringPlaneVector(){return fTriggeringPlaneVect;}
	std::vector<Detector*> GetDetectorVector(){return fDetectorVect;}
	std::string GetSetupFileName(){return fXmlFileName;}
	int GetClusterSize(){return fClusterSize;}
	int GetTriggerTDC(){return fTrigTdc;}
	int GetTriggerTDCChannel(){return fTrigChannel;}
};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_SETUP_H_ */
