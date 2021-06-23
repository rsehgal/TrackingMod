/*
**	Filename : Test_ListFiles.cpp
**	2021-06-23
**	username : rsehgal
*/
#include <iostream>
#include <vector>
#include <string>
#include "HelperFunctions.h"
int main()
{
  std::vector<std::string> vecOfFileNames = GetVectorOfFiles("/home/rsehgal/DevShare/CalibrationFiles/PS55_S1AA6646/");

  for (unsigned int i = 0; i < vecOfFileNames.size(); i++) {
    std::cout << i << " : " << vecOfFileNames[i] << std::endl;
  }
  return EXIT_SUCCESS;
}
