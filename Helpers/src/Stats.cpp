/*
 * Stats.cpp
 *
 *  Created on: 23-Jan-2019
 *      Author: rsehgal
 */

#include "Stats.h"
#include <TFile.h>
#include "Voxelator_Evolution.h"
#include "base/Vector3D.h"
#include "RunHelper.h"
#include "CommonFunc.h"

namespace Tomography {

using Tracking::Vector3D;

Stats::Stats() {
	// TODO Auto-generated constructor stub
	fStats = new TFile("test.root","recreate");
	fRunHelper = Tomography::RunHelper::instance();

}

Stats::Stats(std::string filename){
	fStats = new TFile((filename+".root").c_str(),"recreate");
	fRunHelper = Tomography::RunHelper::instance();
}

Stats::~Stats() {
	// TODO Auto-generated destructor stub

	//Closing the opened files
	fStats->Close();
}


void Stats::CreateScatteringHistogram(){
	int nbins = 10000;
	std::vector<double> scatteringVector = fRunHelper->GetScatteringAngleVector();
	double max = *(std::max_element(scatteringVector.begin(), scatteringVector.end()));
	double min = *(std::min_element(scatteringVector.begin(), scatteringVector.end()));
	std::cout << "Min : " << min << "  ::  Max : " << max << " ::  " << __FILE__ <<" : " << __LINE__ << std::endl;
	fHistScattering = new TH1F("ScatteringHistogram","ScatteringHistogram",nbins,min,max);
	for(std::size_t i = 0  ; i < scatteringVector.size() ; i++){
		fHistScattering->Fill(scatteringVector[i]);
	}

	fHistScattering->Write();

	std::vector<Tracking::Vector3D<double>> truePositivePocaPtVector = fRunHelper->GetTruePositivePocaPtVector();
	std::vector<Tracking::Vector3D<double>> falsePositivePocaPtVector = fRunHelper->GetFalsePositivePocaPtVector();

	TH1F *truePositiveHist = new TH1F("TruePositiveHist","TruePositiveHist",nbins,min,max);
	TH1F *falsePositiveHist = new TH1F("FalsePositiveHist","FalsePositiveHist",nbins,min,max);

	std::vector<double> truePositiveScatteringAngleVector = CommonFunc::Functions::GetScatteringAngleVector(truePositivePocaPtVector);
	std::vector<double> falsePositiveScatteringAngleVector = CommonFunc::Functions::GetScatteringAngleVector(falsePositivePocaPtVector);

	for(std::size_t i = 0  ; i < truePositiveScatteringAngleVector.size() ; i++){
		truePositiveHist->Fill(truePositiveScatteringAngleVector[i]);
	}
	truePositiveHist->Write();

	for(std::size_t i = 0  ; i < falsePositiveScatteringAngleVector.size() ; i++){
			double scatteringAngle = falsePositiveScatteringAngleVector[i];
			falsePositiveHist->Fill(scatteringAngle);
			if(scatteringAngle > 0.07)
				std::cout <<"I : " << " :: Value : " << scatteringAngle << std::endl;

	}
	falsePositiveHist->Write();


}

} /* namespace Tomography */
