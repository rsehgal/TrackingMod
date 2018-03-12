#include <fstream>
#include <iostream>
#include "TRandom3.h"
int main(){
  std::ofstream outfile("gauss.txt");
  double val = 0.;
  TRandom3 r;

  int dim = 2;
  if(dim == 1){
  for(int i = 0 ; i < 20000 ; i++){
	
	val = r.Gaus(12.,2.);
        //std::cout << val << std::endl;
	outfile << val << std::endl;
  }
  }else{
  double meanX = 12. , meanY = 10.;
  for(int i = 0 ; i < 20000 ; i++){

	double valX = r.Gaus(meanX,2.);
	double valY = r.Gaus(meanY,2.);
        //std::cout << val << std::endl;
	outfile << valX << "  " << valY << std::endl;
  }

  meanX = 0.; meanY = 0.;
  for(int i = 0 ; i < 30000 ; i++){

	double valX = r.Gaus(meanX,2.);
	double valY = r.Gaus(meanY,2.);
        //std::cout << val << std::endl;
	outfile << valX << "  " << valY << std::endl;
  }

  }
/*
  for(int i = 0 ; i < 35000 ; i++){
  
  val = r.Gaus(8.,2.);
        //std::cout << val << std::endl;
  outfile << val << std::endl;
  }
*/

  outfile.close();
  return 0;
}
