#include <iostream>
#include <TriggeringPlane.h>
#include <Properties.h>
#include <SetupManager.h>
#include <cstring>

typedef Tomography::Properties Detector;
using namespace Tomography;

int main(int argc, char *argv[]){
  std::string temp_str = std::to_string(atoi(argv[1]));
  temp_str += ".root";
  Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);
  std::cout << "===========================================================================" << std::endl;
  Detector *topPlane = new TriggeringPlane(2,"TopPlane",105,-1);
  Detector *bottomPlane = new TriggeringPlane(2,"bottomPlane",105,7);
  
Tomography::SetupManager::instance()->Register(topPlane);
Tomography::SetupManager::instance()->Register(bottomPlane);
SetupManager *setup = Tomography::SetupManager::instance();

std::cout<<"Size : " << setup->GetDetectorVector("TRG").size() << std::endl;
std::vector<Detector*> trgPlaneVect = setup->GetDetectorVector("TRG");

for(int i = 0 ; i < trgPlaneVect.size() ; i++){
    std::cout<<"Name : " << trgPlaneVect[i]->GetName() << std::endl;
    std::vector<ScintillatorPlane*> scintPlaneVect = trgPlaneVect[i]->GetScintillatorPlaneVector();
    for(int j = 0 ; j < scintPlaneVect.size() ; j++){
        std::vector<Scintillator*> scintVect = scintPlaneVect[j]->GetScintVector();
        for(int k = 0 ; k < scintVect.size(); k++){
            std::cout<< " Channele Name : " << scintVect[k]->GetName() << std::endl;
        }
    }
    
}

//setup->SetEventDetected("TRG",1);
//

Tomography::ScintillatorPlane::SetClusterSize(1);
for(int evNo= 19 ;evNo <20;  evNo++){
std::cout<<"--------------------Event No :  " << evNo << " ---------------------------------"<<std::endl;
        setup->SetEventDetected("TRG",evNo);
        std::cout<<"Event Detected : "<< setup->EventDetected() << std::endl;
        for(int j = 0 ; j < trgPlaneVect.size() ; j++){
            std::cout<<"---- Triggering Plane no : "<< j << " ---- "<< std::endl;
            for(int i = 0 ; i < trgPlaneVect[j]->GetNumOfPlanes() ; i++){
                std::cout<< "Plane num : "<< i <<" : "<< trgPlaneVect[j]->GetPlane(i)->GetFiredStripsVector().size() << std::endl;
                std::vector<Scintillator*> scintVector = trgPlaneVect[j]->GetPlane(i)->GetScintVector();
                std::vector<int> stripVect = trgPlaneVect[j]->GetPlane(i)->GetFiredStripsVector();
                int stripVectorSize = stripVect.size();
                for(int k = 0 ; k < stripVectorSize ; k++){
                        std::cout<<"strip no : "<< stripVect[k] << " : Value : " << scintVector[stripVect[k]]->GetValue() << std::endl;
                }

                }

        }
        }

return 0;

  
}
