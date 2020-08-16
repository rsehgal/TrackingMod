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



class G4Step;
class G4HCofThisEvent;

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

	/*
	 * Extra methods to store info in ScintillatorBar
	 */
	void InitializeVectorOfPsBars();

	std::vector<ScintillatorBar*> psBarVec;
	void PrintVectorOfPsBars();

	static std::vector< std::vector<ScintillatorBar*> > eventsVec;
	// static std::vector<ScintillatorBar*> eventsVec2;

	static void Print(){
		/*std::cout << "888888 Printing eventsVec2 888888888" << std::endl;
		for(unsigned int i = 0 ; i < eventsVec2.size() ; i++){
			eventsVec2[i]->Print();
		}*/		
		std::cout << "9999999999 Printing eventsVec 999999999" << std::endl;
		for(unsigned int i = 0 ; i < eventsVec.size() ; i++){
			std::cout << "@@@@@@@ Printing event num : " << i <<" @@@@@@@" << std::endl;
			for(unsigned int j = 0 ; j < eventsVec[i].size() ; j++){
				eventsVec[i][j]->Print();	
			}
			
		}
	}

private:
	    MyHitsCollection* fHitsCollection;
	    static int stepNum;
	    static unsigned int evNo;
	    

};

#endif /* INC_MYSD_H_ */
