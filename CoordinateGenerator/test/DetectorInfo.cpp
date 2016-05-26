#include "GlassRpc.h"
#include <iostream>
#include <vector>
//#include "VisualizationHelper.h"
//#include "SetupManager.h"

typedef Tomography::Properties Detector;

int main(){

  //Tomography::VisualizationHelper v;
  //Tomography::SetupManager s;
  Detector *rpc1 = new Tomography::GlassRpc(3,"SecondGlassRpc", 90,-1);
  std::cout<<"Name : " << rpc1->GetName() << std::endl;
  std::cout<<"Num of Planes : " << rpc1->GetNumOfPlanes() << std::endl;
  std::cout<<"Total Number of Channels : " << rpc1->GetTotalNumOfChannels() << std::endl;

  for(int i = 0 ; i < rpc1->GetNumOfPlanes() ; i++){
    std::cout<<"Num of Strips in plane : " << i << " : " << rpc1->GetPlane(i)->GetNumOfScintillators() << std::endl;
  }

  return 0;
}
