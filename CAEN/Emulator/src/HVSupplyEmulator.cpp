/*
 * HVSupply.cpp
 *
 *  Created on: Feb 23, 2017
 *      Author: rsehgal
 */

#include "HVSupplyEmulator.h"
#include <string.h>

namespace caen {

    HVSupplyEmulator::HVSupplyEmulator(){
        /*fName = "CAEN";
        fLink = 1;
        fIPAddress = "31.10.3.243";
        fUsername = "admin";
        fPasswd = "admin";*/

        fData["VMon"] = 10000.;
        fData["IMon"] = 4.5;

    }

    HVSupplyEmulator::~HVSupplyEmulator(){


    }

CAENHVRESULT HVSupplyEmulator::CAENHVInitSystem( char *name, int link, char *ipaddress, char *username, char *passwd){

    if(!strcmp(fSupply.GetName(),name) 
        && (fSupply.GetLink()==link) 
        && (!strcmp(fSupply.GetIPAddress(),ipaddress) )
        && (!strcmp(fSupply.GetUserName(),username))
        && (!strcmp(fSupply.GetPasswd(),passwd)) )
        return 1;
}

CAENHVRESULT HVSupplyEmulator::CAENHVGetChParam( char *Name, int slot, char *paramname, int ch, ushort channel, float *param){


}

}// end of caen namespace