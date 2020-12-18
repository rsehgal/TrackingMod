/*
 * SampleAngle.cpp
 *
 *  Created on: 15-Dec-2020
 *      Author: rsehgal
 */

#include <TF1.h>
#include <TApplication.h>
#include <TH1F.h>

int main(){
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	 //TF1 *formula = new TF1("cos2Theta","pow(cos(x),2)",0,M_PI/2.);
	TF1 *formula = new TF1("cos2Theta","pow(cos(x),2)",0,M_PI/2.);
	 //TH1F *histSample = new TH1F("SampledData","SampledData",90,0,M_PI/2.);
	TH1F *histSample = new TH1F("SampledData","SampledData",90,0,0.98);
	 unsigned int numOfSamples = 100000;
	 for(unsigned int i = 0 ; i< numOfSamples ; i++){
		 double angle = formula->GetRandom();
		 if(angle < 0.98)
			 histSample->Fill(angle);
	 }

	 TF1 *fitFormula = new TF1("cos2Theta", "[0]*pow(cos(x),[1])",0,M_PI/2.);
	 histSample->Fit(fitFormula,"r");
	 histSample->Draw();

	 fApp->Run();


}


