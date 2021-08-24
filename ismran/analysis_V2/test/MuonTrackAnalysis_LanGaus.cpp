/*
**	Filename : MuonTrackAnalysis_LanGaus.cpp
**	2021-08-16
**	username : rsehgal
*/
#include<iostream>
#include "langaus.h"
#include "TApplication.h"
int main(int argc, char *argv[]){
 TApplication *fApp = new TApplication("Test", NULL, NULL);

  langaus();
fApp->Run();
  return 0;
}
