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
// $Id: B1Run.cc 66536 2012-12-19 14:32:36Z ihrivnac $
//
/// \file B1Run.cc
/// \brief Implementation of the B1Run class

#include "B1Run.hh"
#include <fstream>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1Run::B1Run()
: G4Run(),
  fEdep(0.)
  //,fEdep2(0.)
{
	std::cout<<"@@@@@@ RUN DEFAULT CONSTRUCTOR CALLED @@@@@@@@@@@@" << std::endl;
	fVox.SetVoxelator(50*cm,50*cm,40*cm,10*cm,10*cm,8*cm);
	//Tomography::Voxelator::Create(50*cm,50*cm,10*cm,5*cm,5*cm,5*cm);
	tree = Tracking::Tree::instance();
	  tree->SetTreeDefaultParameters();
	  tree->InitializeTreeForWriting();
	  tree->CreateBranch<Track>("InComingTracking", incoming);
	  tree->CreateBranch<Track>("OutGoingTracking", outgoing);
	  tree->CreateBranch<double>("ScatteringAngle", scattering);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1Run::~B1Run()
{ tree->WriteToFile();
std::cout<<"Length of RunVoxelVector : "<< fVectorOfVoxelsForWholeRun.size() << std::endl;

//Logic to Remove outlier base on voxel
std::ofstream filter("filtered.txt");
for(int i= 0 ; i < fVoxelVector.size() ; i++){
	if(!fVoxelVector[i]->IsOutlier()){
		std::vector<Tracking::Vector3D<double>> ptVect = fVoxelVector[i]->GetPocaPointsVector();//GetPointsVector();
		for(int j = 0 ; j < ptVect.size() ; j++){
			filter << ptVect[j].x() << " " << ptVect[j].y() << " " <<  ptVect[j].z() << " " << ptVect[j].GetColor() << std::endl;
		}

	}
}
filter.close();

}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1Run::Merge(const G4Run* run)
{
  const B1Run* localRun = static_cast<const B1Run*>(run);
  fEdep  += localRun->fEdep;
  fEdep2 += localRun->fEdep2;

  G4Run::Merge(run); 
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1Run::AddEdep (G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}

void B1Run::FillPhysicalTrackVector(std::vector<Vector3D<double>> hitVect){
    phyTrackVect.push_back(hitVect);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


