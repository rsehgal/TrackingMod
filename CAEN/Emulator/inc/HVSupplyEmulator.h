/*
 * HVSupply.h
 *
 *  Created on: Feb 23, 2017
 *      Author: rsehgal
 */

#ifndef CAEN_EMULATOR_INC_HVSUPPLYEMULATOR_H_
#define CAEN_EMULATOR_INC_HVSUPPLYEMULATOR_H_

#include <string.h>
#include <cstring>
#include <map>
#include "PhysicalHVSupply.h"
#include "EmulatorConstants.h"
namespace caen{

CAENHVRESULT CAENHVInitSystem( const char *name, int link, const char *ipaddress,const char *username,const char *passwd);
CAENHVRESULT CAENHVGetChParam( const char *Name, int slot, const char *paramname, int ch, ushort channel, float *param);

#if(0)
class HVSupplyEmulator{

   /* std::string fName;
    int fNumOfSlots;
    int fNumOfChannelPerSlot;*/
    CAENHVRESULT  fRet_init;
    CAENHVRESULT fRet;
    /*int fLink;
    std::string fIPAddress;
    std::string fUsername;
    std::string fPasswd;*/
    /*float fVMon;
    float fIMon;*/
    std::map<char*,float> fData;
    PhysicalSupply fSupply;

public:
    HVSupplyEmulator();
    ~HVSupplyEmulator();
    CAENHVRESULT CAENHVInitSystem( char *name, int link, char *ipaddress, char *username, char *passwd);
    CAENHVRESULT CAENHVGetChParam( char *Name, int slot, char *paramname, int ch, ushort channel, float *param);
    PhysicalSupply GetPhysicalSupply(){return fSupply;}



};
#endif
} // end of caen namespace
#endif /* CAEN_EMULATOR_INC_HVSUPPLYEMULATOR_H_ */
