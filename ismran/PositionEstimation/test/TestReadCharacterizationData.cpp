#include <iostream>
#include <string>
#include "CharacterizationFileReader.h"
#include <fstream>
int main(int argc, char *argv[])
{
  std::string filename = argv[1];
  CharacterizationFileReader cfReader(filename);
  std::cout << "Actual Source Position : " << cfReader.GetActualPosition() << std::endl;
  std::cout << "BarName : " << cfReader.GetBarName() << std::endl;
  cfReader.GetEvent(5)->Print();

  cfReader.PrintRandom();

  std::vector<Event *> trainingVec = cfReader.GetTrainingData();
  std::vector<Event *> testingVec  = cfReader.GetTestingData();
  std::cout << "Size of Training Data : " << trainingVec.size() << std::endl;
  std::cout << "Size of Testing Data : " << testingVec.size() << std::endl;
  std::cout << "Total Num Of Events in the Tree : " << cfReader.GetTotalNumOfEvents() << std::endl;
  std::string trainingFileName =
      cfReader.GetBarName() + "_" + std::to_string(cfReader.GetActualPosition()) + "_training.csv";
  std::string testingFileName =
      cfReader.GetBarName() + "_" + std::to_string(cfReader.GetActualPosition()) + "_testing.csv";

  std::ofstream outfileTraining(trainingFileName);
  for (unsigned int i = 0; i < trainingVec.size(); i++) {
    outfileTraining << trainingVec[i]->GetQNear() << "," << trainingVec[i]->GetQFar() << ","
                    << trainingVec[i]->GetDelT() << "," << trainingVec[i]->GetActualSourcePosition() << std::endl;
  }
  outfileTraining.close();

  std::ofstream outfileTesting(testingFileName);
  for (unsigned int i = 0; i < testingVec.size(); i++) {
    outfileTesting << testingVec[i]->GetQNear() << "," << testingVec[i]->GetQFar() << "," << testingVec[i]->GetDelT()
                   << "," << testingVec[i]->GetActualSourcePosition() << std::endl;
  }
  outfileTesting.close();

  return 0;
}
