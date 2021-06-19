#include "CharacterizationFileReader.h"
#include <numeric>
#include <bits/stdc++.h>

CharacterizationFileReader::CharacterizationFileReader() {}

CharacterizationFileReader::CharacterizationFileReader(std::string filename)
{
  fTestProportion = 0.25;
  fFileName       = filename;
  fp              = new TFile(filename.c_str(), "r");
  ConnectTree();
}

void CharacterizationFileReader::ConnectTree(std::string treeName)
{
  fTree = (TTree *)fp->Get("ftree");
  fTree->SetBranchAddress("fBrCh", &fBrch);
  fTree->SetBranchAddress("fQlong", &fQlong);
  fTree->SetBranchAddress("fTstamp", &fTstamp);
  fTree->SetBranchAddress("fTime", &fTime);
  fTree->SetBranchAddress("fDelt", &fDelt);
}

TTree *CharacterizationFileReader::GetTree() const
{
  return fTree;
}

TFile *CharacterizationFileReader::GetFilePointer() const
{
  return fp;
}

Event *CharacterizationFileReader::GetEvent(unsigned int evNo)
{
  fTree->GetEntry(evNo);
  return (new Event(fBrch, fQlong, fTstamp, fTime, fDelt, GetActualPosition()));
}

void CharacterizationFileReader::RandomizeIt()
{
  fEventNumVec.resize(fTree->GetEntries());
  std::iota(fEventNumVec.begin(), fEventNumVec.end(), 0);
  unsigned seed = time(0);
  std::shuffle(fEventNumVec.begin(), fEventNumVec.end(), std::default_random_engine(seed));
}

std::vector<Event *> CharacterizationFileReader::GetTrainingData()
{
  unsigned int numOfEventsInTraining = (1 - fTestProportion) * fEventNumVec.size();
  std::vector<Event *> trainingEventsVec;

  std::cout << "======================================" << std::endl;
  std::cout << "Size of Training Data : " << numOfEventsInTraining << std::endl;
  std::cout << "======================================" << std::endl;
  for (unsigned int i = 0; i < numOfEventsInTraining; i++) {
    if (!(i % 50000) && i != 0)
      std::cout << "Processed Training Event : " << i << " : Tree Entry : " << fEventNumVec[i] << std::endl;
    trainingEventsVec.push_back(GetEvent(fEventNumVec[i]));
  }
  std::cout << "Going to return the training vector of size : " << trainingEventsVec.size() << std::endl;
  return trainingEventsVec;
}

std::vector<Event *> CharacterizationFileReader::GetTestingData()
{
  unsigned int numOfEventsInTraining = (1 - fTestProportion) * fEventNumVec.size();
  unsigned int numOfEventsInTesting = fTestProportion * fEventNumVec.size();
  std::vector<Event *> testingEventsVec;

  std::cout << "======================================" << std::endl;
  std::cout << "Size of Testing Data : " << numOfEventsInTesting << std::endl;
  std::cout << "======================================" << std::endl;
  unsigned int testingEventNo = 0;
  for (unsigned int i = numOfEventsInTraining; i < fEventNumVec.size(); i++) {
    if (!(testingEventNo % 50000) && testingEventNo != 0)
      std::cout << "Processed Testing Event : " << testingEventNo << " : Tree Entry : " << fEventNumVec[i] << std::endl;
    testingEventsVec.push_back(GetEvent(fEventNumVec[i]));
    testingEventNo++;
  }
  std::cout << "Going to return the testing vector of size : " << testingEventsVec.size() << std::endl;
  return testingEventsVec; 
}

void CharacterizationFileReader::PrintRandom()
{
  RandomizeIt();

  std::cout << "Random Event Numbers : ";
  for (unsigned int i = 0; i < 10; i++) {
    std::cout << fEventNumVec[i] << " ,  ";
  }
  std::cout << std::endl;
}

int CharacterizationFileReader::GetActualPosition()
{
  unsigned int locationOfDot            = fFileName.find(".");
  unsigned int locationOfLastUnderscore = fFileName.find_last_of("_");
  unsigned int charsToPick              = locationOfDot - locationOfLastUnderscore - 1;
  std::string locationString            = fFileName.substr(locationOfLastUnderscore + 1, charsToPick);
  return std::stoi(locationString);
}

std::string CharacterizationFileReader::GetBarName()
{
  std::string justFileName = fFileName.substr(fFileName.find_last_of("/") + 1);
  // std::cout << "JUST FILE NAME : " << justFileName << std::endl;
  unsigned int locationOfPS      = justFileName.find("PS");
  unsigned int locationOfCouples = justFileName.find("Couples");
  std::string barName            = justFileName.substr(locationOfPS, locationOfCouples - 7);
  return barName;
}

unsigned int CharacterizationFileReader::GetTotalNumOfEvents(){
return fTree->GetEntries();
}
