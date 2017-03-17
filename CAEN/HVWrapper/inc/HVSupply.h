/*
 * HVSupply.h
 *
 *  Created on: Feb 15, 2017
 *      Author: rsehgal
 */

#ifndef CAEN_INC_HVSUPPLY_H_
#define CAEN_INC_HVSUPPLY_H_
#include <cstring>

#ifdef USE_EMULATOR
#include "EmulatorConstants.h"
#include "HVSupplyEmulator.h"

#else
//Include header file of CAEN libraries
#endif

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

	//TODO: DATA TYPES of These parameter must be carefully checked

	//Parameter for reading
	float *param;// = NULL; // used for reading VMON and IMON
	float *param2;// = NULL; //used for reading power status
	ushort *id_channel;// = NULL;
	std::string fParName;
	int fPower;
	int fSystemStatus;

public:
	HVSupply(std::string name, std::string ipaddress, int link, std::string username="admin", std::string passwd="admin");
	virtual ~HVSupply();
	void Login();
	bool IsLoginOk(){return (fRet_init == CAENHV_OK);}

	//Getters functions
	std::string GetIPAddress() {return fIPAddress;}
	std::string GetName() {return fName;}
	int GetNumOfSlots() {return fNumOfSlots;}
	int GetNumOfChannelPerSlot() {return fNumOfChannelPerSlot;}
	float GetVoltage(int slot, ushort channel);//{return fVMon;}
    float GetCurrent(int slot, ushort channel);//{return fIMon;}
	int GetPowerStatus(int slot, ushort channel);//{return fPower;}
	int GetSystemStatus(int slot, ushort channel);//{return fSystemStatus;}

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
