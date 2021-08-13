/*
**	Filename : Test_ZFromTheoreticalFormula.cpp
**	2021-07-10
**	username : rsehgal
*/

//DOCBEGIN
/*
** Function to do the mapping from Q To Z, but using the
** theoretical formula (1/(2*mu))*(log(Q_F/Q_N))
*/
//DOCEND
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
  TH1F *hist                     = new TH1F("ZfromQ", "ZfromQ", 100, -150., 150.);
  for (unsigned int i = 0; i < eventsVec.size(); i++) {
    hist->Fill(eventsVec[i]->GetExpectedGammaPos());
  }

  hist->Draw();
  fApp->Run();
}
