/*
 * MySD.h
 *
 *  Created on: Dec 8, 2017
 *      Author: rsehgal
 */

#ifndef INC_MYSD_H_
#define INC_MYSD_H_

#include "G4VSensitiveDetector.hh"
#include "MyHit.h"
#include "vector"
#include "includes.hh"
#include "PsBar.h"
#include "SingleMuonTrack.h"
#include "ScintillatorBar_V2.h"


class G4Step;
class G4HCofThisEvent;
class TTree;


class MySD : public G4VSensitiveDetector {

public:
//	MySD();
	MySD(const G4String& name,
	                const G4String& hitsCollectionName);
	virtual ~MySD();

	// methods from base class
	virtual void   Initialize(G4HCofThisEvent* hitCollection);
	virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
	virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

	static int numOfParticlesReached;
	bool reachedSensitiveRegion;

	/*
	 * Extra methods to store info in ScintillatorBar
	 */
	void InitializeVectorOfPsBars();

	//static 
	std::vector<lite_interface::ScintillatorBar_V2*> psBarVec;
	void PrintVectorOfPsBars();

	static std::vector< std::vector<lite_interface::ScintillatorBar_V2*> > eventsVec;
	// static std::vector<ScintillatorBar*> eventsVec2;

	//As per the new functions
	static std::vector< lite_interface::SingleMuonTrack* > muonTrackVec;

	static std::vector<G4ThreeVector> exactHitVector;

	static void Print();
	/*{
		std::cout << "888888 Printing eventsVec2 888888888" << std::endl;
		for(unsigned int i = 0 ; i < eventsVec2.size() ; i++){
			eventsVec2[i]->Print();
		}
		if(0){
		std::cout << "9999999999 Printing eventsVec 999999999" << std::endl;
		for(unsigned int i = 0 ; i < eventsVec.size() ; i++){
			std::cout << "@@@@@@@ Printing event num : " << i <<" @@@@@@@" << std::endl;
			for(unsigned int j = 0 ; j < eventsVec[i].size() ; j++){
				eventsVec[i][j]->Print();	
			}
			
		}
		}
	}*/

private:
	    MyHitsCollection* fHitsCollection;
	    static int stepNum;
	    static unsigned int evNo;

public:
	    static unsigned int numOfStoppedParticles;
	    static unsigned long int muonNum;
	    


};

#endif /* INC_MYSD_H_ */
