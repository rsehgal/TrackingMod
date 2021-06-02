/*
**	Filename : ml.h
**	2021-05-31
**	username : rsehgal
*/
#ifndef ml_h
#define ml_h

#include <vector>
#include <TGraph.h>
#include <TApplication.h>
#include "PsBar.h"
#include <TH1F.h>
#include "ScintillatorBar_V2.h"
#include "SingleMuonTrack.h"
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>

extern void GenerateTrainingData(std::string filename, unsigned int inspectedLayerIndex,
                                 std::string outfileName = "training.txt");

#endif