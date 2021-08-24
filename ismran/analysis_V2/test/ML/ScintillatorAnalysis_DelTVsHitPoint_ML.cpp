#include <iostream>
#include "includes.h"
int main(int argc, char *argv[])
{

  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  AutoCallers();

  ushort barIndex                    = std::atoi(argv[2]);
  lite_interface::Calibration *calib = lite_interface::Calibration::instance("completeCalib2.root");
  lite_interface::Analyzer analyzerObj(argv[1]);
  std::vector<lite_interface::ScintillatorBar_V2 *> scintBarVec = analyzerObj.GetVectorOfScintillators();
  std::ofstream outfile("Model_Bar_" + std::string(argv[2])+"_"+std::string(argv[3]) + ".txt");
  for (unsigned int scintIndex = 0; scintIndex < scintBarVec.size(); scintIndex++) {
    lite_interface::ScintillatorBar_V2 *scint = scintBarVec[scintIndex];
    if (scint->GetBarIndex() == barIndex) {
      std::cout << "DelTCorr : " << scint->GetDelTCorrected() << " : " << std::endl;
      (scint->EstimateHitPosition_Param())->Print();
      //(scint->GetExactHitPosition())->Print();
      //outfile << scint->GetDelTCorrected() << "," << (scint->GetExactHitPosition())->GetZ() << std::endl;
      outfile << scint->GetDelTCorrected() << "," << (scint->EstimateHitPosition_Param())->GetZ() << std::endl;
    }
  }
  outfile.close();
  return 0;
}
