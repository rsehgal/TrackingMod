#include "GlassRpc.h"
#include <iostream>
#include <vector>
//#include "VisualizationHelper.h"
typedef Tomography::Properties Detector;

#undef USE_EVE
#include "SetupManager.h"

int main(){

  //Tomography::VisualizationHelper v;
  Tomography::SetupManager s;
  //TEveManager::Create();
  Detector *rpc1 = new Tomography::GlassRpc(3,"FirstGlassRpc", 90,-1);
  Detector *rpc2 = new Tomography::GlassRpc(3,"SecondGlassRpc", 90,-1);
  Detector *rpc3 = new Tomography::GlassRpc(3,"ThirdGlassRpc", 90,-1);
  std::cout<<"Name : " << rpc1->GetName() << std::endl;
  std::cout<<"Num of Planes : " << rpc1->GetNumOfPlanes() << std::endl;
  std::cout<<"Total Number of Channels : " << rpc1->GetTotalNumOfChannels() << std::endl;

  for(int i = 0 ; i < rpc1->GetNumOfPlanes() ; i++){
    std::cout<<"Num of Strips in plane : " << i << " : " << rpc1->GetPlane(i)->GetNumOfScintillators() << std::endl;
    std::cout<<"Dimension of plane : " << i << " : " << rpc1->GetPlane(i)->GetLength() << "," << rpc1->GetPlane(i)->GetBreadth() << std::endl;
  }

  std::cout<<"-------------------------------------" << std::endl;
  s.Register(rpc1);
  s.Register(rpc2);
  s.Register(rpc3);

  std::vector<Detector*> detVector = s.GetDetectorVector("GLASS");
  for(int i=0 ; i < detVector.size() ; i++){
    std::cout<<"Detector : " << i << " : " << detVector[i]->GetName() <<std::endl;
  }

  return 0;
}
