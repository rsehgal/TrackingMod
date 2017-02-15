/*
 * HVSupply.h
 *
 *  Created on: Feb 15, 2017
 *      Author: rsehgal
 */

#ifndef CAEN_INC_HVSUPPLY_H_
#define CAEN_INC_HVSUPPLY_H_
#include <cstring>
namespace caen {

class HVSupply {
	std::string fName;
	int fNumOfSlots;
	int fNumOfChannelPerSlot;
	CAENHVRESULT  fRet_init;
	CAENHVRESULT fRet;
	int fLink;
	std::string fIPAddress;
	std::string fUsername;
	std::string fPasswd;
	float fVMon;
	float fIMon;

	//Parameter for reading
	float *param = NULL; // used for reading VMON and IMON
	uint *param2 = NULL; //used for reading power status
	ushort *id_channel = NULL;
	std::string fParName;
	int fPower;
	int fSystemStatus;

public:
	HVSupply();
	virtual ~HVSupply();
	void Login();
	bool IsLoginOk(){return (fRet_init == CAEN_OK);}

	//Getters functions
	std::string GetIPAddress() {return fIPAddress;}
	std::string GetName() {return fName;}
	int GetNumOfSlots() {return fNumOfSlots;}
	int GetNumOfChannelPerSlot() {return fNumOfChannelPerSlot;}
	float GetVoltage();//{return fVMon;}
    float GetCurrent();//{return fIMon;}
	int GetPowerStatus();//{return fPower;}
	int GetSystemStatus();//{return fSystemStatus;}

	//Setters functions
	void SetV0();
	void SetPowerOn();
	void SetPowerOff();
	void SetI0();
	void SetTrip();
	void HVPrint(){}
	void HVScan();
	void SetV0Single();
	void HVPrintSingle(){}
	void SetTripSingle();
	void SetI0Single();
	void HVMon_Full();
	void HVScanGap();
	void SetI0Gap();
	void SetTripGap();
	void SetV0Gap();
	void HVMonFullGap();
	void ChannelTest();
	void HVMonFullStabGap();


};

} /* namespace caen */

#endif /* CAEN_INC_HVSUPPLY_H_ */
