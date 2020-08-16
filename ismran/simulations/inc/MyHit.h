/*
 * MyHit.h
 *
 *  Created on: Dec 8, 2017
 *      Author: rsehgal
 */

#ifndef INC_MYHIT_H_
#define INC_MYHIT_H_

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

using Vec_t = G4ThreeVector;

class MyHit : public G4VHit {

private:
	Vec_t fPosition;
	G4String fName;
	unsigned int fCopyNo;
	double fEDep;

public:
	MyHit();
	virtual ~MyHit();
	MyHit(const MyHit &right);
	const MyHit& operator=(const MyHit &right);
	int operator==(const MyHit &right) const;
    inline void * operator new(size_t);
    inline void operator delete(void *aHit);
    void Draw() const{}
    void Print() const{
    	std::cout << "Volume Name : " << fName << " : Copy No : " << fCopyNo 
    			  << " : Energy Deposited : " << fEDep << std::endl;
    }

    void SetCopyNum(unsigned int cpNo){
    	fCopyNo = cpNo;
    }

    unsigned int GetCopyNum()const{
    	return fCopyNo;
    }

    void SetEnergyDeposited(double edep){
    	fEDep=edep;
    }

    double GetEnergyDeposited() const{
    	return fEDep;
    }

    void SetName(G4String name){
    	fName = name;
    }

    G4String GetName(){
    	return fName;
    }

	void SetPosition(Vec_t position){
		fPosition = position;
	}

	Vec_t GetPosition(){
		return fPosition;
	}
};

typedef G4THitsCollection<MyHit> MyHitsCollection;

//extern G4Allocator<MyHit> MyHitAllocator;
extern G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator;

inline void* MyHit::operator new(size_t)
{
	  if(!MyHitAllocator)
	      MyHitAllocator = new G4Allocator<MyHit>;
	  return (void *) MyHitAllocator->MallocSingle();

}

inline void MyHit::operator delete(void *aHit)
{
	MyHitAllocator->FreeSingle((MyHit*) aHit);
}





#endif /* INC_MYHIT_H_ */
