/*
**	Filename : Test_QDistribution.cpp
**	2021-06-23
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
  FillAttenCoeffVec();
  std::vector<Event *> eventsVec = cfReader.GetAllEvents(std::atoi(argv[2]));
  TH1F *hist                     = new TH1F("HistLogQNearByQFar", "HistLogQNearByQFar", 100, -5., 5.);
  for (unsigned int i = 0; i < eventsVec.size(); i++) {
    //std::cout << eventsVec[i]->GetLogQNearByQFar() << std::endl;
    hist->Fill(eventsVec[i]->GetLogQNearByQFar());
  }

  hist->Draw();
  fApp->Run();
}
