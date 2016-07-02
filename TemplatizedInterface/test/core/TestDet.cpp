#include <iostream>
#include <SetupManager.h>
#include <Properties.h>
#include <GlassRpc.h>
#include <ScintillatorPlane.h>

typedef Tomography::Properties Detector;
using namespace Tomography;
int main(int argc , char *argv[]){

        std::string temp_str = std::to_string(atoi(argv[1]));
        temp_str += ".root";
        Tracking::Tree::instance()->ReadTree(temp_str.c_str(), "BSC_DATA_TREE", 0);

	Detector *rpc1 = new GlassRpc(2,"MT-1",60,31);
	rpc1->SetClusterSize(1);

	Detector *rpc2 = new GlassRpc(4,"MT-2",-60,31);
	rpc2->SetClusterSize(1);
        SetupManager *setup = Tomography::SetupManager::instance();
        setup->Register(rpc1);
        setup->Register(rpc2);

        std::cout<<"Size : "<< setup->GetDetectorVector("GLASS").size() << std::endl;

        
        std::vector<Detector*> detVect = setup->GetDetectorVector("GLASS");

#if 0
	for(int k = 0 ; k < detVect.size() ; k++){

        	std::cout<< detVect[k]->GetName() << std::endl;
	        std::cout<< detVect[k]->GetNumOfPlanes() << std::endl;
	        std::cout<< detVect[k]->GetTotalNumOfChannels() << std::endl;
        	std::vector<ScintillatorPlane*> planeVect = detVect[k]->GetScintillatorPlaneVector();

		for(int i = 0 ; i < detVect[k]->GetNumOfPlanes() ; i++){

		        std::cout<<"Plane : " << i << " : " << planeVect[i]->GetNumOfScintillators() << std::endl;
				int numOfStrips = planeVect[i]->GetNumOfScintillators();
		        std::vector<Scintillator*> scintVector = planeVect[i]->GetScintVector();

			  for(int j = 0 ; j < numOfStrips ; j++){

				   std::cout<<"StripName : "<< scintVector[j]->GetName() << std::endl;
	
		          }
	        }
	}

#endif

    //    Tomography::ScintillatorPlane::SetClusterSize(1);
for(int evNo= 19 ;evNo <20;  evNo++){
std::cout<<"--------------------Event No :  " << evNo << " ---------------------------------"<<std::endl;
        setup->SetEventDetected("GLASS",evNo);
        std::cout<<"Event Detected : "<< setup->EventDetected() << std::endl;
        for(int j = 0 ; j < detVect.size() ; j++){
	    for(int i = 0 ; i < detVect[j]->GetNumOfPlanes() ; i++){
		std::cout<< "Plane num : "<< i <<" : "<< detVect[j]->GetPlane(i)->GetFiredStripsVector().size() << std::endl;
		std::vector<Scintillator*> scintVector = detVect[j]->GetPlane(i)->GetScintVector();
		std::vector<int> stripVect = detVect[j]->GetPlane(i)->GetFiredStripsVector();
		int stripVectorSize = stripVect.size();
		for(int k = 0 ; k < stripVectorSize ; k++){
			std::cout<<"strip no : "<< stripVect[k] << " : Value : " << scintVector[stripVect[k]]->GetValue() << std::endl;
		}
		
		}

	}
        }
	return 0;
  
}
