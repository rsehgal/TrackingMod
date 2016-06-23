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


return 0;

  
}