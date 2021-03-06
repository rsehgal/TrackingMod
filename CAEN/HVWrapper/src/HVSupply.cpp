/*
 * HVSupply.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: rsehgal
 */

#include "HVSupply.h"

namespace caen {

HVSupply::HVSupply(std::string name, std::string ipaddress, int link, std::string username, std::string passwd ) {
	// TODO Auto-generated constructor stub
	fName = name;
	fIPAddress = ipaddress;
	fLink = LINKTYPE_TCPIP;
	fUsername = username;
	fPasswd = passwd;

	param = new float; //malloc(sizeof(float));
	param2 = new float; //malloc(sizeof(uint));
	id_channel = new ushort; //malloc(sizeof(ushort));

	fVMon = 0.;
	fIMon = 0.;
	fPower = 0;


}

HVSupply::~HVSupply() {
	// TODO Auto-generated destructor stub

	//This function is not yet there in HVSuppyEmulator
	//fRet_init = CAENHVDeinitSystem(fName);
}

void HVSupply::Login(){

	//TODO : if already logged in then don't login again,
	//Take care of semaphores

	fRet_init =  CAENHVInitSystem(fName.c_str(), fLink, fIPAddress.c_str(), fUsername.c_str(), fPasswd.c_str());
	if(fRet_init == CAENHV_OK)
		std::cout<<"Login Successfull..." << std::endl;
	else
		std::cerr << "Login Unsuccessfull. Please check login parameter...." << std::endl;
}

float HVSupply::GetVoltage(int slot, ushort channel){

	fRet = CAENHVGetChParam(fName.c_str(), slot, "VMon", 1, channel, param);
	if (fRet==CAENHV_OK) {
	   fVMon = param[0];
	}

	return fVMon;
}

float HVSupply::GetCurrent(int slot, ushort channel){

	fRet = CAENHVGetChParam(fName.c_str(), slot, "IMon", 1, channel, param);
	if (fRet==CAENHV_OK) {
	   fIMon = param[0];
	}

	return fIMon;
}

int HVSupply::GetPowerStatus(int slot, ushort channel){

	fRet = CAENHVGetChParam(fName.c_str(), slot, "Pw", 1, channel, param2);
	if (fRet==CAENHV_OK) {
	   fPower = param2[0];
	}

	return fPower;
}

int HVSupply::GetSystemStatus(int slot, ushort channel){

	fRet = CAENHVGetChParam(fName.c_str(), slot, "Status", 1, channel, param2);
	if (fRet==CAENHV_OK) {
	   fSystemStatus = param2[0];
	}

	return fSystemStatus;
}


} /* namespace caen */
