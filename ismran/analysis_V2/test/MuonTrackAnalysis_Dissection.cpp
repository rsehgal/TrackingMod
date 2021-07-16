/*
**	Filename : MuonTrackAnalysis_Dissection.cpp
**	2021-07-16
**	username : rsehgal
*/
#include <iostream>
#include <includes.h>
int main(int argc, char *argv[])
{
  bool verbose = false;
  // TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);

  for (unsigned int i = 0; i < smtVec.size(); i++) {
    std::vector<lite_interface::ScintillatorBar_V2 *> scintVec = smtVec[i]->GetMuonTrack();

    std::cout << RED << "============= Processing Track Num : " << i << " =========================" << RESET << std::endl;
    for (unsigned int j = 0; j < scintVec.size(); j++) {
      scintVec[j]->Print();
    }
  }

  return 0;
}
