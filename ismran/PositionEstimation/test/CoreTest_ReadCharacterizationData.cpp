/*
**	Filename : CoreTest_ReadCharacterizationData.cpp
**	2021-06-24
**	username : rsehgal
*/
#include <iostream>
#include <string>
#include "CharacterizationFileReader.h"
#include <fstream>
int main(int argc, char *argv[])
{
  std::string dirname = argv[1];
  // std::string filename = argv[1];
  std::vector<std::string> vecOfFileNames = GetVectorOfFiles(dirname.c_str());

  CharacterizationFileReader cfReader;
  for (unsigned int i = 0; i < vecOfFileNames.size(); i++) {

    std::string compFileName = dirname+"/"+vecOfFileNames[i];
    cfReader.ResetWith(compFileName);

    std::cout << "**************************************" << std::endl;
    std::cout << "Actual Source Position : " << cfReader.GetActualPosition() << std::endl;
    std::cout << "BarName : " << cfReader.GetBarName() << std::endl;
  }
  //cfReader.GetEvent(5)->Print();

  return 0;
}
