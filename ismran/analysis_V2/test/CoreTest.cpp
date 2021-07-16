#include "HelperFunctions.h"
#include "base/Vector3D.h"
#include "HardwareNomenclature.h"
#include "Point3D.h"
#include <TApplication.h>
// using  lite_interface::Point3D;
int main()
{
TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();

  for (unsigned int i = 0; i < vecOfBarsNamess.size(); i++) {
std::cout << vecOfBarsNamess[i] <<" : ";
    vecOfScintXYCenter[i].Print();
  }
  for (unsigned int i = 10; i > 0; i--) {

    std::cout << "Y of layer : " << i - 1 << " : " << GetYOfLayer(i - 1) << std::endl;
  }

  lite_interface::Point3D startp(25, GetYOfLayer(9), 30);
  lite_interface::Point3D endp(25, GetYOfLayer(8), 30);
  /*
    lite_interface::Point3D *extraPolated = ExtrapolatePointOnLayer(&startp, &endp, 7);
    std::cout << "Extrapolated Point on Layer 7: ";
    extraPolated->Print();
    extraPolated = ExtrapolatePointOnLayer(&startp, &endp, 1);
    std::cout << "Extrapolated Point on Layer 1: ";
    extraPolated->Print();*/

  /* Checking Start and End layer index calculation for layer*/

  for (unsigned int i = 0; i < 10; i++) {
    std::cout << "For Inspected Layer : " << i << " :: StartIndex : " << GetStartIndex(i)
              << " : EndIndex : " << GetEndIndex(i) << std::endl;
  }

  TH1F *hist = new TH1F("hist","hist",300,-150.,150.);
  for(unsigned int i = 0 ; i < 10000 ; i++){
double val = GetGaussianRandomSample(0., 100.);
//std::cout << val << std::endl;
hist->Fill(val);
}

std::cout <<"Testing Split string functionality...." << std::endl;
std::string s="Hello Raman how r you.";
std::vector<std::string> v = SplitString(s);

for(unsigned int i = 0 ; i < v.size() ; i++){
std::cout << v[i] << std::endl;
}
 
hist->Draw();
fApp->Run();
  return 0;
}
