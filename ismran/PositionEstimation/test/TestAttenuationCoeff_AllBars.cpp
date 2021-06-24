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
#include "includes.h"
int main(int argc, char *argv[])
{
  std::vector<double> vecOfAttenCoeff;
  std::string DIRNAME = argv[1];
  unsigned int start  = std::atoi(argv[2]);
  unsigned int jump   = 9;
  std::ofstream attCoeffFile("attenCoeffs.txt", std::ios_base::app);
  // for (unsigned int i = 0; i < vecOfBarsNamess.size(); i++) {
  for (unsigned int i = start; i < (start + jump); i++) {
    std::string barname                     = vecOfBarsNamess[i];
    std::string dirname                     = DIRNAME + "/" + barname;
    std::vector<std::string> vecOfFileNames = GetVectorOfFiles(dirname.c_str());
    std::cout << "Size of vecOffileName : " << vecOfFileNames.size() << std::endl;

    CharacterizationFileReader cfReader;
    double meanAttenValue = 0.;

    for (unsigned int i = 0; i < vecOfFileNames.size(); i++) {
      std::string compFileName = dirname + "/" + vecOfFileNames[i];
      cfReader.ResetWith(compFileName);

      std::cout << "**************************************" << std::endl;
      std::cout << "Actual Source Position : " << cfReader.GetActualPosition() << std::endl;
      std::cout << "BarName : " << cfReader.GetBarName() << std::endl;
      double attenCoeff = cfReader.GetMeanAttenuationCoeff();
      std::cout << "Attenuation Coeff at Position : " << cfReader.GetActualPosition() << " : " << attenCoeff
                << std::endl;
      meanAttenValue += attenCoeff;
    }

    meanAttenValue /= (vecOfFileNames.size() - 1);
    vecOfAttenCoeff.push_back(meanAttenValue);
    if (i == start) attCoeffFile << barname << " : ";
    std::cout << "Mean Attenuation Coeff Value of : " << cfReader.GetBarName() << " : " << meanAttenValue << std::endl;
    attCoeffFile << meanAttenValue << " , ";
  }
  attCoeffFile << std::endl;
  attCoeffFile.close();

  /*for (unsigned int i = 0; i < vecOfAttenCoeff.size(); i++) {
    std::cout << vecOfAttenCoeff[i] << ",";
    if (!(i % 9)) std::cout << std::endl;
  }*/
  return 0;
}
