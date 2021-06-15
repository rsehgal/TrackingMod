#include <iostream>
#include "includes.h"
int main(int argc, char *argv[])
{
  TApplication *fApp = new TApplication("Test", NULL, NULL);
  GenerateScintMatrixXYCenters();
  lite_interface::Calibration *calib                    = lite_interface::Calibration::instance("completeCalib2.root");
  std::string filename                                  = argv[1];
  std::vector<lite_interface::SingleMuonTrack *> smtVec = GetMuonTracksVector(filename);//, 100000);
  TH2F *hist = new TH2F("QRatioVsDelT", "QRatioVsDelT", 1000, -20., 20., 1000, -10., 10.);
  for (unsigned int i = 0; i < smtVec.size(); i++) {
    lite_interface::SingleMuonTrack *smt = smtVec[i];
    //    if (i < 5)
    {
      //std::cout << "*************** Muon Track : " << i << "***********************" << std::endl;
      std::vector<lite_interface::ScintillatorBar_V2 *> scintVec = smt->GetMuonTrack();
      for (unsigned int scintIndex = 0; scintIndex < smt->size(); scintIndex++) {
        lite_interface::ScintillatorBar_V2 *scint = scintVec[scintIndex];
        /*std::cout << scint->GetQLongNear() << " , " << scint->GetQLongFar() << " , " << scint->GetQLongMean()
                  << std::endl;
        std::cout << scint->GetTNearCorr() << " , " << scint->GetTFarCorr() << " , " << scint->GetDelTCorrected()
                  << std::endl;*/
       //std::cout << (scint->GetDelTCorrected() / 1000.) <<" , " << (1.*scint->GetQLongNear() / scint->GetQLongFar()) << std::endl;
        if (scint->GetBarIndex() == 85) {
          hist->Fill(scint->GetDelTCorrected() / 1000., (1.* scint->GetQLongNear() / scint->GetQLongFar()));
        }
      }
    }
  }

  hist->Draw("colz");
  fApp->Run();
}
