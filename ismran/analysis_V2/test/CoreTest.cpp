#include "HelperFunctions.h"
#include "base/Vector3D.h"
#include "HardwareNomenclature.h"
#include "Point3D.h"
//using  lite_interface::Point3D;
int main(){

	GenerateScintMatrixXYCenters();
	for(unsigned int i = 10 ; i > 0 ; i--){
		
		std::cout << "Y of layer : " << i-1 << " : " << GetYOfLayer(i-1) <<  std::endl;
    }

    lite_interface::Point3D startp(25,GetYOfLayer(9),30);
    lite_interface::Point3D endp(25,GetYOfLayer(8),30);
    lite_interface::Point3D *extraPolated = ExtrapolatePointOnLayer(&startp,&endp,7);
    std::cout << "Extrapolated Point on Layer 7: "; extraPolated->Print();
    extraPolated = ExtrapolatePointOnLayer(&startp,&endp,1);
    std::cout << "Extrapolated Point on Layer 1: "; extraPolated->Print();

    return 0;
}