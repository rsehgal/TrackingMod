#include <fstream>
#include <iostream>
#include "TRandom3.h"
int main(){
  std::ofstream outfile("gauss.txt");
  double val = 0.;
  TRandom3 r;
  for(int i = 0 ; i < 20000 ; i++){
	
	val = r.Gaus(5.,4.);
        //std::cout << val << std::endl;
	outfile << val << std::endl;
  }

  for(int i = 0 ; i < 35000 ; i++){
  
  val = r.Gaus(8.,2.);
        //std::cout << val << std::endl;
  outfile << val << std::endl;
  }


  outfile.close();
  return 0;
}
