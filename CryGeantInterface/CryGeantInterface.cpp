/*
 * CryGeantInterface.cpp
 *
 *  Created on: Jan 1, 2018
 *      Author: rsehgal
 */

#include "CryGeantInterface.h"
#include "base/Vector3D.h"
#include "CommonFunc.h"
#include "Files.h"
#include "DetectorMapping.h"
#include "base/Global.h"
CryGeantInterface::CryGeantInterface() {
	// TODO Auto-generated constructor stub

}

CryGeantInterface::~CryGeantInterface() {
	// TODO Auto-generated destructor stub
}


void CryGeantInterface::GeneratePrimariesForCry(G4Event *anEvent){
      
    if(Tomography::EventBreak::instance()->BreakSimulation())
        return;

	  if (InputState != 0) {
	    G4String* str = new G4String("CRY library was not successfully initialized");
	    //G4Exception(*str);
	    G4Exception("PrimaryGeneratorAction", "1",
	                RunMustBeAborted, *str);
	  }
	  std::string particleName;
	  vect->clear();
	  gen->genEvent(vect);

	  //....debug output
	  G4cout << "\nEvent=" << anEvent->GetEventID() << " "
	         << "CRY generated nparticles=" << vect->size()
	         << G4endl;

	  for ( unsigned j=0; j<vect->size(); j++) {
	    particleName=CRYUtils::partName((*vect)[j]->id());

	    //....debug output
	    std::cout << "  "          << particleName << " "
	         << "charge="      << (*vect)[j]->charge() << " "
	         << std::setprecision(4)
	         << "energy (MeV)=" << (*vect)[j]->ke()*MeV << " "
	         << "pos (m)"
	         << G4ThreeVector((*vect)[j]->x(), (*vect)[j]->y(), (*vect)[j]->z())
	         << " " << "direction cosines "
	         << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
	         << " " << std::endl;

	    particleGun->SetParticleDefinition(particleTable->FindParticle((*vect)[j]->PDGid()));
	    particleGun->SetParticleEnergy((*vect)[j]->ke()*MeV);
	    double gunZ = Tomography::DetectorMapping::instance()->GetGunZ();
	    particleGun->SetParticlePosition(G4ThreeVector((*vect)[j]->x()*m, (*vect)[j]->y()*m, gunZ)); //150*cm));
	    particleGun->SetParticleMomentumDirection(G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w()));
	    particleGun->SetParticleTime((*vect)[j]->t());

        double solidAngleIncoming = CommonFunc::Functions::instance()->
                      GetAngleInRadian(Tracking::Vector3D<double>((*vect)[j]->u(),
                                                                  (*vect)[j]->v(),
                                                                  (*vect)[j]->w()).Unit(),
                                                          Tracking::Vector3D<double>(0.,0.,-1.));
        double energy = (*vect)[j]->ke()*MeV;
        if(particleName == "muon")
        	Tomography::Files::instance()->Write("StatsFromGenerator.txt",3,1., solidAngleIncoming,energy);

        //_____________________________________________________________________________________
		  	  //Logic use to store real incoming muon angles in a vector of angle in Run
/*
		  	  B1Run* run
		  	      = static_cast<B1Run*>(
		  	          G4RunManager::GetRunManager()->GetNonConstCurrentRun());
		  	  double angleIncoming = CommonFunc::Functions::instance()->
		  			  GetAngleInRadian(Tracking::Vector3D<double>((*vect)[j]->u(),
		  					  	  	  	  	  	  	  	  	  	  (*vect)[j]->v(),
																  (*vect)[j]->w()),
		  					  	  	  	  	  	  	  	  Tracking::Vector3D<double>(0.,0.,-1.));
		  	  if(!strcmp(particleName,"muon"))
		  		  run->FillRealIncomingAngleVector(angleIncoming);
		  	//run->FillRealIncomingAngleVector((*vect)[j]->ke()*MeV);
*/
		 //_____________________________________________________________________________________



	    particleGun->GeneratePrimaryVertex(anEvent);
	    delete (*vect)[j];
	  }

}

void CryGeantInterface::ForCry(const char *inputfile){
	  // define a particle gun
	  particleGun = new G4ParticleGun();

	  // Read the cry input file
	  std::ifstream inputFile;
	  inputFile.open(inputfile,std::ios::in);
	  char buffer[1000];

	  if (inputFile.fail()) {
	    if( *inputfile !=0)  //....only complain if a filename was given
	      G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
	    InputState=-1;
	  }else{
	    std::string setupString("");
	    while ( !inputFile.getline(buffer,1000).eof()) {
	      setupString.append(buffer);
	      setupString.append(" ");
	    }

	    CRYSetup *setup=new CRYSetup(setupString,"../data");

	    gen = new CRYGenerator(setup);

	    // set random number generator
	    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
	    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
	    InputState=0;
	  }
	  // create a vector to store the CRY particle properties
	  vect=new std::vector<CRYParticle*>;

	  // Create the table containing all particle names
	  particleTable = G4ParticleTable::GetParticleTable();

	  gunMessenger = new PrimaryGeneratorMessenger(this);
}


//----------------------------------------------------------------------------//
void CryGeantInterface::InputCRY()
{
  InputState=1;
}

//----------------------------------------------------------------------------//
void CryGeantInterface::UpdateCRY(std::string* MessInput)
{
  CRYSetup *setup=new CRYSetup(*MessInput,"../data");

  gen = new CRYGenerator(setup);

  // set random number generator
  RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
  setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
  InputState=0;

}

//----------------------------------------------------------------------------//
void CryGeantInterface::CRYFromFile(G4String newValue)
{
  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(newValue,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    G4cout << "Failed to open input file " << newValue << G4endl;
    G4cout << "Make sure to define the cry library on the command line" << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup=new CRYSetup(setupString,"../data");

    gen = new CRYGenerator(setup);

  // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
  }
}



