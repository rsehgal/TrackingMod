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
extern void GenerateTrainingData_9(std::string filename, unsigned int inspectedLayerIndex,
                                   std::string outfileName = "training.txt");
extern void GenerateTrainingData_All(std::string filename, unsigned int inspectedLayerIndex,
                                     std::string outfileName = "training.txt");
extern void GenerateTrainingData_All_V2(std::string filename, unsigned int inspectedLayerIndex,
                                        std::string outfileName = "training.txt");
extern void GenerateTrainingData_All_Level1(std::string filename, unsigned int inspectedLayerIndex,
                                            std::string outfileName = "training.txt");
extern void GenerateModelForAngle(std::string filename, unsigned int inspectedLayerIndex, std::string outfileName);

/*Parameterization improvement related functions*/
extern void GenerateParamData(std::string filename, unsigned int inspectedLayerIndex);
#endif
