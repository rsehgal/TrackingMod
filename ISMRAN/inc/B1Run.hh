
#ifndef B1Run_h
#define B1Run_h 1

#include "G4Run.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Event;

/// Run class
///

class B1Run : public G4Run
{
	std::vector<double> fScatteringAngleVect;
    std::vector<G4ThreeVector> fPoCAPointVect;

  public:
    B1Run();
    virtual ~B1Run();

    // method from the base class
    virtual void Merge(const G4Run*);
    
    // get methods
    std::vector<double> GetScatteringAngleVector() const{
    //std::cout << "GetScatteringAngleVector Called ........" << std::endl;
    	return fScatteringAngleVect;

    }
    void FillScatteringAngleVector(double scatteringAngle){fScatteringAngleVect.push_back(scatteringAngle);}
    void InsertPocaPoint(G4ThreeVector poca) {fPoCAPointVect.push_back(poca);}
    std::vector<G4ThreeVector> GetPocaPtVector() const {return fPoCAPointVect;}


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

