/*
**	Filename : TestAttenuationCoeff.cpp
**	2021-06-22
**	username : rsehgal
*/
#include <iostream>
#include <string>
#include "CharacterizationFileReader.h"
#include <fstream>
#include <TH1F.h>
#include <TApplication.h>
#include <vector>
int main(int argc, char *argv[])
{
  TApplication *fApp   = new TApplication("Test", NULL, NULL);
  std::string filename = argv[1];
  CharacterizationFileReader cfReader(filename);
  std::cout << "Actual Source Position : " << cfReader.GetActualPosition() << std::endl;
  std::cout << "BarName : " << cfReader.GetBarName() << std::endl;
  double meanAttenValue = cfReader.GetMeanAttenuationCoeff(std::atoi(argv[2]));
  std::cout << "Calculated Mean Attentuation Value : " << meanAttenValue << std::endl;

  std::vector<Event *> eventsVec = cfReader.GetAllEvents(std::atoi(argv[2]));

  TH1F *hist = new TH1F("attenCoeff", "attenCoeff", 100, -0.5, 0.5);
  for (unsigned int i = 0; i < eventsVec.size(); i++) {
    hist->Fill(eventsVec[i]->GetTempAttenuationCoeffForAnEvent());
  }

  hist->Draw();
  fApp->Run();
  return 0;
}
