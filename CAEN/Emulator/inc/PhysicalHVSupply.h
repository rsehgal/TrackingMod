/*
 * HVSupply.h
 *
 *  Created on: Feb 23, 2017
 *      Author: rsehgal
 */

#ifndef CAEN_EMULATOR_INC_PHYSICALHVSUPPLY_H_
#define CAEN_EMULATOR_INC_PHYSICALHVSUPPLY_H_

#include <string>
#include <vector>

namespace caen{

class Channel{
    char *fName;
    float fVSet;
    float fVMon;
    float fIMon;
    float fISet;
    static int id;

public:
    Channel(): fName("TestChannel"),
               fVSet(0.),fVMon(0.),
               fIMon(0.),fISet(0.){

                id++;
                std::string channelName = "TestChannel-"+id;
                fName = (char*)channelName.c_str();

               }

    ~Channel(){}
    char* GetName(){return fName;}
    float GetVMon(){return fVMon;}
    float GetIMon(){return fIMon;}
    void SetV0(float vset){fVMon = vset; fVSet = vset;}
    void SetI0(float iset){fIMon = iset; fISet = iset;}

};


class Slot{
    char  *fName;
    int fNumOfChannels;
    std::vector<Channel*> fChannelVect;
    static int id;

public:
    Slot():fName("TestSlot"),
           fNumOfChannels(6){
            id++;
            std::string slotName = "TestSlot-"+id;
            fName = (char*)slotName.c_str();
            fChannelVect.resize(fNumOfChannels);
            for(int i = 0 ; i < fNumOfChannels ; i++){

                fChannelVect.push_back(new Channel());
            }
           }
    ~Slot(){}
    char* GetName(){return fName;}
    int GetNumOfChannels(){return fNumOfChannels;}
    std::vector<Channel*> GetChannelVector(){return fChannelVect;}



};


class PhysicalSupply{
    char *fName;
    int fLink;
    char *fIPAddress;
    char *fUsername;
    char *fPasswd;
    int fNumOfSlots;
    std::vector<Slot*> fSlotVect;
public:

    PhysicalSupply(){
        fName = "CAEN";
        fLink = 1;
        fIPAddress = "31.10.3.243";
        fUsername = "admin";
        fPasswd = "admin";
        fNumOfSlots = 6;
        fSlotVect.resize(6);
        for(int i = 0 ;  i < fNumOfSlots ; i++)
            fSlotVect.push_back(new Slot());
    }

    ~PhysicalSupply(){}

    char* GetName(){return fName;}
    int GetLink(){return fLink;}
    char* GetIPAddress(){return fIPAddress;}
    char* GetUserName(){return fUsername;}
    char* GetPasswd(){return fPasswd;}
    int GetNumOfSlots(){return fNumOfSlots;}
    std::vector<Slot*> GetSlotVector(){return fSlotVect;}
};

}// end of caen namespace

#endif