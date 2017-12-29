//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1Run.hh 66536 2012-12-19 14:32:36Z ihrivnac $
//
/// \file B1Run.hh
/// \brief Definition of the B1Run class

#ifndef B1Run_h
#define B1Run_h 1

#include "G4Run.hh"
#include "globals.hh"

class G4Event;

/// Run class
///

class B1Run : public G4Run
{
	std::vector<double> fScatteringAngleVect;
	std::vector<double> fMuonInitialAngles;
	std::vector<double> fRealIncomingMuonAngles;

  public:
    B1Run();
    virtual ~B1Run();

    // method from the base class
    virtual void Merge(const G4Run*);
    
    void AddEdep (G4double edep); 

    // get methods
    G4double GetEdep()  const { return fEdep; }
    G4double GetEdep2() const { return fEdep2; }
    std::vector<double> GetScatteringAngleVector() const{
    	//std::cout << "GetScatteringAngleVector Called ........" << std::endl;
    	return fScatteringAngleVect;

    }

    /* This function is introduced to record the angle of incoming muon before it reaches the detector.
    ** ideally this should be same as angle calculated using first two detector that muon encounters.
    ** This will be a good validation step, and will be very much useful to validate the angular
    ** histogram of cosmic muons
    **
    ** NOTE : But this information is available only at PrimaryGenerator level, so this vector needs
    ** to be filled in PrimaryGenerator
    */
    void FillRealIncomingAngleVector(double incomingAngle){fRealIncomingMuonAngles.push_back(incomingAngle);}
    std::vector<double> GetRealIncomingAngleVector() const {return fRealIncomingMuonAngles;}

    void FillScatteringAngleVector(double scatteringAngle){fScatteringAngleVect.push_back(scatteringAngle);}
    void FillInComingAngleVector(double incomingAngle){fMuonInitialAngles.push_back(incomingAngle);}
    std::vector<double> GetIncomingAngleVector() const {return fMuonInitialAngles;}

  private:
    G4double  fEdep;
    G4double  fEdep2;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

