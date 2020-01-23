#include "Voxelator_Evolution.h"
#include <cmath>
#include "Delta.h"
#include "CommonFunc.h"
#include <cmath>
#include "VoxelV2.h"

namespace Tomography {
namespace evolution{
Voxelator *Voxelator::s_instance = 0;

#if(0)
//Taking bottom layers as layer of noisy voxels
void Voxelator::PredictThreshold(){
	unsigned int numOfLayersToBeUsed = 2;
	unsigned int numOfVoxelsInALayer = fVoxelatorDim.x()*fVoxelatorDim.y();
	unsigned int numOfVoxelsUsedForThresholdSelection = numOfLayersToBeUsed*numOfVoxelsInALayer;
	for(unsigned int i = 0 ; i < fVoxelVector.size() ; i++){
		if(fVoxelVector[i]->GetVoxelNum() < numOfVoxelsUsedForThresholdSelection)
			fNoisyLayerPointCountHist->Fill(fVoxelVector[i]->GetPointCount());
	}

	double stddev = fNoisyLayerPointCountHist->GetStdDev();
	double mean = fNoisyLayerPointCountHist->GetMean();
	double valsigma = Tomography::confidenceInterval;

	double startx = mean - valsigma * stddev;
	double endx = mean + valsigma * stddev;
	fThresholdVal = endx;
}
#endif

#if(1)
//Taking complete envelop of outer voxels as layers of noisy voxels
void Voxelator::PredictThreshold(){
	/*unsigned int numOfLayersToBeUsed = 2;
	unsigned int numOfVoxelsInALayer = fVoxelatorDim.x()*fVoxelatorDim.y();
	unsigned int numOfVoxelsUsedForThresholdSelection = numOfLayersToBeUsed*numOfVoxelsInALayer;
	for(unsigned int i = 0 ; i < fVoxelVector.size() ; i++){
			if(fVoxelVector[i]->GetVoxelNum() < numOfVoxelsUsedForThresholdSelection)
				fNoisyLayerPointCountHist->Fill(fVoxelVector[i]->GetPointCount());
		}*/

	for(unsigned int i = 0 ; i < fBackgroundVoxelNumberVector.size() ; i++){
		int voxelNum = fBackgroundVoxelNumberVector[i];
		int voxelID = IfVoxelExist(voxelNum);
		if(voxelID >=0 && voxelID < GetTotalNumberOfVoxels()){
			fNoisyLayerPointCountHist->Fill(fVoxelVector[voxelID]->GetPointCount());
		}
	}


	double stddev = fNoisyLayerPointCountHist->GetStdDev();
	double mean = fNoisyLayerPointCountHist->GetMean();
	double valsigma = Tomography::confidenceInterval;

	double startx = mean - valsigma * stddev;
	double endx = mean + valsigma * stddev;
	fThresholdVal = endx;
}
#endif


#if(0)
void Voxelator::PredictThreshold(){

#define USE_MAX_VAL
	//std::cout << "Entered PredictThreshold : " << __FILE__ <<" : " << __LINE__ << std::endl;
	double stddev = fVoxelsIn1DCount->GetStdDev();
	double mean = fVoxelsIn1DCount->GetMean();

	/* Selecting point in the voxels which comes under X Sigma
	 * The interval value is defined in "confidenceInterval" variable
	 * defined in Global.h
	 */

	double valsigma = Tomography::confidenceInterval;

	double startx = mean-valsigma*stddev;
	double endx = mean+valsigma*stddev;

	//default value
	double threshold = 0.;
	int sum = 0;
	int count = 0;
	for(unsigned int i = 0 ; i < fVoxelVector.size() ; i++){
		if(fVoxelVector[i]->GetVoxelNum() < startx ||
				fVoxelVector[i]->GetVoxelNum() > endx){

			int pointcount = fVoxelVector[i]->GetPointCount();
			//std::cout << "PointCount : " << pointcount << " :: Threshold : " << threshold << std::endl;
#ifndef USE_MAX_VAL
				sum += pointcount;
				count++;
#else
			if( pointcount > threshold)
				threshold = fVoxelVector[i]->GetPointCount();
#endif
		}
	}

#ifndef USE_MAX_VAL
	std::cout << "Sum : " << sum << " : Count : " << count << std::endl;
	fThresholdVal = sum/count;
#else
	fThresholdVal = threshold;
#endif
}

#endif

void Voxelator::PredictWeightedThreshold(TH1F *hist){

#define USE_MAX_VAL
	//std::cout << "Entered PredictThreshold : " << __FILE__ <<" : " << __LINE__ << std::endl;
	double stddev = hist->GetStdDev();
	double mean = hist->GetMean();

	/* Selecting point in the voxels which comes under X Sigma
	 * The interval value is defined in "confidenceInterval" variable
	 * defined in Global.h
	 */

	double valsigma = Tomography::confidenceInterval;

	double startx = mean-valsigma*stddev;
	double endx = mean+valsigma*stddev;

	//default value
	double threshold = 0.;
	int sum = 0;
	int count = 0;
	for(unsigned int i = 0 ; i < fVoxelVector.size() ; i++){
		if(fVoxelVector[i]->GetVoxelNum() < startx ||
				fVoxelVector[i]->GetVoxelNum() > endx){

			int pointcount = fVoxelVector[i]->GetPointCount();
			//std::cout << "PointCount : " << pointcount << " :: Threshold : " << threshold << std::endl;
#ifndef USE_MAX_VAL
				sum += pointcount;
				count++;
#else
			if( pointcount > threshold)
				threshold = fVoxelVector[i]->GetPointCount();
#endif
		}
	}

#ifndef USE_MAX_VAL
	std::cout << "Sum : " << sum << " : Count : " << count << std::endl;
	fWeightedThresholdVal = sum/count;
#else
	fWeightedThresholdVal = threshold;
#endif
}


int Voxelator::IfVoxelExist(int voxelNum){
//std::cout<< "VoxelVectorSize : " << run->fVoxelVector.size() << std::endl;
	if(fVisitedVoxelNumVector.size()){
		for(int i = 0 ; i < fVisitedVoxelNumVector.size() ; i++){
			if(fVisitedVoxelNumVector[i] == voxelNum){
				//std::cout<<"Voxel Hit.........Found previously created Voxel... :  "  << fVisitedVoxelNumVector[i] << std::endl;
				//return fVisitedVoxelNumVector[i];
				return i;
			}
		}
		return -1;
	}else{
		return -1;
	}
}

bool Voxelator::IfVoxelExistInNoisyEnvelop(int voxelNum){
	for (unsigned int i = 0 ; i < fBackgroundVoxelNumberVector.size() ; i++){
		unsigned int voxNum = fBackgroundVoxelNumberVector[i];
		if(voxNum == voxelNum){
			return true;
		}
	}
	return false;
}

std::vector<Voxel_V2*> Voxelator::GetFilteredVoxelVector(){
	std::vector<Voxel_V2*> filteredVoxelVector;
	for(int i= 0 ; i < fVoxelVector.size() ; i++){

#ifndef USE_TWO_SIGMA_THRESHOLD
		if(!fVoxelVector[i]->IsOutlier())
#else
		Tomography::evolution::Voxelator::instance()->PredictThreshold();
		if(fVoxelVector[i]->GetPointCount() > Tomography::evolution::Voxelator::instance()->GetThresholdVal())
		//if(fVoxelVector[i]->GetPointCount() > Tomography::minPointsInAVoxel)
#endif
		{
			filteredVoxelVector.push_back(fVoxelVector[i]);

		}
	}

	std::cout <<  "@@@@@@@@@@@ Filtered Voxel Vector Size : "<< filteredVoxelVector.size() << " @@@@@@@@@@@@ " << std::endl;
	return filteredVoxelVector;
}

std::vector<unsigned int> Voxelator::GetFilteredTrackIndexVector(){
	std::vector<unsigned int> filteredTrackIndexVector;
	std::vector<Tracking::Vector3D<double>> pocaPtVector = GetFilteredPocaPtVector();
	for(unsigned int i = 0 ; i < pocaPtVector.size() ; i++){
		filteredTrackIndexVector.push_back(pocaPtVector[i].GetTrackId());
	}
	return filteredTrackIndexVector;
}

std::vector<Tracking::Vector3D<double>> Voxelator::GetFilteredPocaPtVector(){
	std::vector<Voxel_V2*> filteredVoxelVector = GetFilteredVoxelVector();
	std::vector<Tracking::Vector3D<double>> filteredPocaVector;
	for(int i = 0 ; i < filteredVoxelVector.size() ; i++){
		for(int j = 0 ; j < filteredVoxelVector[i]->GetPocaPointsVector().size() ; j++){
			filteredPocaVector.push_back(filteredVoxelVector[i]->GetPocaPointsVector()[j]);
		}
	}

	std::cout <<  "@@@@@@@@@@@ Filtered Poca Vector Size : "<< filteredPocaVector.size() << " @@@@@@@@@@@@ " << std::endl;
	return filteredPocaVector;

}

std::vector<Voxel_V2*> Voxelator::GetFilteredVoxelVectorUsingCleanVoxel(){
	std::vector<Voxel_V2*> filteredVoxelVector;
	int signalVoxelCounter = 0;
	for (int i = 0; i < fVoxelVector.size(); i++) {
		if (fVoxelVector[i]->GetCleanVoxelCount() < Tomography::cleanVoxelCount) {
			signalVoxelCounter++;

			filteredVoxelVector.push_back(fVoxelVector[i]);

		}
	}

	std::cout << "SignalVoxelCounter : " << signalVoxelCounter << std::endl;

	return filteredVoxelVector;
}


std::vector<Voxel_V2*> Voxelator::GetFilteredVoxelVectorUsingCleanVoxel(std::vector<Voxel_V2*> voxelVector){
	std::vector<Voxel_V2*> filteredVoxelVector;
	int signalVoxelCounter = 0;
	for (int i = 0; i < voxelVector.size(); i++) {
		if (voxelVector[i]->GetCleanVoxelCount() < Tomography::evolution::Voxelator::instance()->GetThresholdVal()) {
			signalVoxelCounter++;
			filteredVoxelVector.push_back(fVoxelVector[i]);
		}
	}

	std::cout << "SignalVoxelCounter : " << signalVoxelCounter << std::endl;

	return filteredVoxelVector;
}

void Voxelator::NormalizeEachVoxelCount(){
	int maxCount = 0 ;
	//Finding the maximum count in the Voxel Vector
	for(unsigned int i = 0 ; i < fVoxelVector.size() ; i++){
		if(fVoxelVector[i]->GetPointCount() > maxCount){
			maxCount = fVoxelVector[i]->GetPointCount();
		}
	}

	for(unsigned int i = 0 ; i < fVoxelVector.size() ; i++){
		fVoxelVector[i]->SetNormalizedCount((double)fVoxelVector[i]->GetPointCount()/maxCount);
	}


}

void Voxelator::NormalizeEachVoxelScatteringValue(){
	double maxScatteringValue = 0. ;
	//Finding the maximum count in the Voxel Vector
	for(unsigned int i = 0 ; i < fVoxelVector.size() ; i++){
		if(fVoxelVector[i]->GetMeanScattering() > maxScatteringValue){
			maxScatteringValue = fVoxelVector[i]->GetMeanScattering();
		}
	}

	for(unsigned int i = 0 ; i < fVoxelVector.size() ; i++){
		fVoxelVector[i]->SetNormalizedScatteringValue(fVoxelVector[i]->GetMeanScattering()/maxScatteringValue);
	}


}


std::vector<Tracking::Vector3D<double>> Voxelator::GetFilteredPocaPtVector(std::vector<Voxel_V2*> filteredVoxelVector){

		std::vector<Tracking::Vector3D<double>> filteredPocaVector;
		for(int i = 0 ; i < filteredVoxelVector.size() ; i++){
			for(int j = 0 ; j < filteredVoxelVector[i]->GetPocaPointsVector().size() ; j++){
				filteredPocaVector.push_back(filteredVoxelVector[i]->GetPocaPointsVector()[j]);
			}
		}
		return filteredPocaVector;
}


std::vector<Voxel_V2*> Voxelator::GetFilteredVoxelVectorBasedOnNormalizedCount(){
	std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&& GetFilteredVoxelVectorBasedOnNormalizedCount &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
	std::vector<Voxel_V2*> filteredVoxelVector;
	int normalizedCounter = 0;
	//Calling this in Store function of RunHelper.cpp
	//NormalizeEachVoxelCount();
	int pointCount = 0;
	for (int i = 0; i < fVoxelVector.size(); i++) {
		double normalizedCount = fVoxelVector[i]->GetNormalizedCount();
		//Be carefule of this threshold limit
		if(normalizedCount > 0.6){
			normalizedCounter++;
			filteredVoxelVector.push_back(fVoxelVector[i]);
			pointCount += fVoxelVector[i]->GetPointCount();
		}
	}

	std::cout << "Total Number of Filtered POints : " << pointCount << std::endl;
	std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
	return filteredVoxelVector;
}

std::vector<Voxel_V2*> Voxelator::GetFilteredVoxelVectorBasedOnNormalizedScatteringValue(){
	std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&& GetFilteredVoxelVectorBasedOnNormalizedScatteringValue &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;
	std::vector<Voxel_V2*> filteredVoxelVector;
	int normalizedCounter = 0;
	//Calling this in Store function of RunHelper.cpp
	//NormalizeEachVoxelCount();
	int pointCount = 0;
	for (int i = 0; i < fVoxelVector.size(); i++) {
		double normalizedCount = fVoxelVector[i]->GetNormalizedScatteringValue();
		//Be carefule of this threshold limit
		if(normalizedCount > 0.00){
			normalizedCounter++;
			filteredVoxelVector.push_back(fVoxelVector[i]);
			pointCount += fVoxelVector[i]->GetPointCount();
		}
	}
	std::cout << "Total Number of Filtered POints : " << pointCount << std::endl;
	std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;


	return filteredVoxelVector;
}


std::vector<Voxel_V2*> Voxelator::GetFilteredVoxelVectorBasedOnWeightedCount(){
	std::vector<Voxel_V2*> filteredVoxelVector;
	int weightedCounter = 0;
	for (int i = 0; i < fVoxelVector.size(); i++) {
		//fVoxelVector[i]->CalcWeightedCount();
		//std::cout <<"AyWeightedCount from Voxelator : " << fVoxelVector[i]->GetWeightedCount() << std::endl;
		int weightedCount = fVoxelVector[i]->GetWeightedCount();
		//if (fVoxelVector[i]->GetWeightedCount() > 0) {
		if(weightedCount > 0){
			weightedCounter++;
			filteredVoxelVector.push_back(fVoxelVector[i]);

		}
	}
	std::cout << " @@@@@@@@@@ Weighted Counter @@@@@@@@@@@@" << std::endl << "VoxelVector Size : " << fVoxelVector.size() << ":   RAMANN        " << weightedCounter <<std::endl << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
	return filteredVoxelVector;

}


std::vector<Voxel_V2*> Voxelator::GetFilteredVoxelVectorBasedOnWeightedCount(TH1F *hist){
	std::vector<Voxel_V2*> filteredVoxelVector;
	Tomography::evolution::Voxelator::instance()->PredictWeightedThreshold(hist);

	for(int i= 0 ; i < fVoxelVector.size() ; i++){

		if(fVoxelVector[i]->GetPointCount() > Tomography::evolution::Voxelator::instance()->GetWeightedThresholdVal()){
			filteredVoxelVector.push_back(fVoxelVector[i]);
		}
	}
	return filteredVoxelVector;
}


/*std::vector<Tracking::Vector3D<double>> Voxelator::GetFilteredPocaPtVectorUsingCleanedVoxel(){
	std::vector<Voxel_V2*> filteredVoxelVector = GetFilteredVoxelVectorUsingCleanVoxel();
		std::vector<Tracking::Vector3D<double>> filteredPocaVector;
		for(int i = 0 ; i < filteredVoxelVector.size() ; i++){
			int voxelNum = filteredVoxelVector[i]->GetVoxelNum();
			if(voxelNum < 0 || voxelNum > Tomography::evolution::Voxelator::instance()->GetTotalNumberOfVoxels())
				continue;
			for(int j = 0 ; j < filteredVoxelVector[i]->GetPocaPointsVector().size() ; j++){
				filteredPocaVector.push_back(filteredVoxelVector[i]->GetPocaPointsVector()[j]);
			}
		}
		return filteredPocaVector;
}*/

//std::vector<Tracking::Vector3D<double>>
void Voxelator::GetFilteredPocaPtVectorUsingCleanedVoxel(std::vector<Tracking::Vector3D<double>> &pocaPtVector){
	Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::instance();
	std::vector<Tomography::Voxel_V2*> voxelVector = voxelator->GetVoxelVector();
	//std::vector<Tracking::Vector3D<double>> filteredPocaPtVector = voxelator->GetFilteredPocaPtVectorUsingCleanedVoxel();
	std::vector<Tomography::Voxel_V2*> filteredVoxelVectorUsingCleanedVoxel = voxelator->GetFilteredVoxelVectorUsingCleanVoxel();
	//std::cout << "FilteredVoxelVectorUsingCleanedVoxel Size : "	<< filteredVoxelVectorUsingCleanedVoxel.size() << std::endl;

	//int pocaCounter = 0;
	for (int i = 0; i < filteredVoxelVectorUsingCleanedVoxel.size(); i++) {
		//pocaCounter += (filteredVoxelVectorUsingCleanedVoxel[i]->GetPocaPointsVector()).size();
		//std::cout << "FilteredVoxel Num : " << i << std::endl;
		//filteredVoxelVectorUsingCleanedVoxel[i]->Print();
		Voxel_V2 *voxel = filteredVoxelVectorUsingCleanedVoxel[i];
		  int voxelNum = voxel->GetVoxelNum();
		  if(voxelNum < 0 || voxelNum > Tomography::evolution::Voxelator::instance()->GetTotalNumberOfVoxels())
				return;
			//std::cout << "Total Num Of PocaPoint in Voxel : " << fVoxelNum << " :: " << fVectPointsInVoxel.size() << std::endl;
		    std::vector<Tracking::Vector3D<double>> vectPointsInVoxel = voxel->GetPocaPointsVector();
			for(int i = 0 ; i < vectPointsInVoxel.size() ; i++){
				pocaPtVector.push_back(vectPointsInVoxel[i]);//.Print();
			}
			//std::cout << std::endl;
	}

	//return pocaPtVector;

}

void Voxelator::Insert(Tracking::Vector3D<double> point, int voxelNum, bool useEnclosingVoxels, double scatteringDensity){
	fVoxelVector.push_back(new Voxel_V2(point,voxelNum));
		fVisitedVoxelNumVector.push_back(voxelNum);
}

void Voxelator::InsertCleanVoxel(int voxelNum){
	fVoxelVector.push_back(new Voxel_V2(voxelNum));
	fVisitedVoxelNumVector.push_back(voxelNum);
}

void Voxelator::PrintCleanVoxelNumAndCleanCount(std::vector<int> cleanHittedVoxelNumVector){
	std::cout <<"=========== Hitted Clean Voxel Number and CleanCount ===========" << std::endl;
		for(int i = 0 ; i < cleanHittedVoxelNumVector.size() ; i++){
			std::cout << "Clean Voxel Num : " << cleanHittedVoxelNumVector[i] << " :: CleanCount : " <<
					GetVoxelFromVoxelator(cleanHittedVoxelNumVector[i])->GetCleanVoxelCount() << std::endl;
		}
		std::cout << "===============================================================" << std::endl;
		std::cout << "CleanHittedVoxelVectorSize : " << cleanHittedVoxelNumVector.size() << std::endl;
}
/*
void Voxelator::Insert(Tracking::Vector3D<double>, int voxelNum){
	fVoxelVector.push_back(new Voxel(fPocaPt,voxelNum));
	fVisitedVoxelNumVector.push_back(voxelNum);
}
*/

void Voxelator::Reset(){
	for(int i = 0 ; i < fVoxelVector.size() ; i++){
		fVoxelVector[i]->Reset_V2();
	}
	fVisitedVoxelNumVector.clear();
	fVoxelVector.clear();
}

Voxelator* Voxelator::instance(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
			 double voxelX,double voxelY, double voxelZ) {
        if (!s_instance)
          s_instance = new Voxelator(voxelizedVolHalfX,voxelizedVolHalfY, voxelizedVolHalfZ,voxelX, voxelY, voxelZ);
        return s_instance;
    }


Voxelator* Voxelator::instance() {
        if (s_instance)
          return s_instance;
        else{
        	std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
        	std::cout<<"@@ Instance of Voxelator does not exist in Memory, please create it first....@@" << std::endl;
        	std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
        }
    }

Voxelator* Voxelator::Create(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
			 double voxelX,double voxelY, double voxelZ) {
        if (!s_instance)
          s_instance = new Voxelator(voxelizedVolHalfX,voxelizedVolHalfY, voxelizedVolHalfZ,voxelX, voxelY, voxelZ);
        return s_instance;
    }



Voxelator::Voxelator(){
//fVoxelsIn1D = new TH1F();
fVoxelizedVolumeDim.Set(500,500,400); //Currently setting to values used in Simulations
fEachVoxelDim.Set(10,10,80); // Ideally this should corresponds to strip size of detectors
			  // so setting it to 3
//fVoxelatorDim.Set(int(fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),int(fVoxelizedVolumeDim.y()/fEachVoxelDim.y()),
	//			      int(fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );

fVoxelatorDim.Set((fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),(fVoxelizedVolumeDim.y()/fEachVoxelDim.y()),
				      (fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );


CreateHistogram();

}

Voxelator::Voxelator(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
			 double voxelX,double voxelY, double voxelZ){
	SetVoxelator(voxelizedVolHalfX,voxelizedVolHalfY,voxelizedVolHalfZ,voxelX,voxelY,voxelZ);
    CalculateVoxelCenters();
	fVoxelizedVolumeDim.Print();
	fVoxelatorDim.Print();
	int totalNumOfVoxels = fVoxelatorDim.x()*fVoxelatorDim.y()*fVoxelatorDim.z();
	int numOfLayers = fVoxelatorDim.z();
	int numOfParts = fVoxelatorDim.z();
	int numOfVoxelsInOneLayer=fVoxelatorDim.x()*fVoxelatorDim.y();
	//LowerLayer
	for(unsigned int voxelNum = 0 ; voxelNum < numOfVoxelsInOneLayer; voxelNum++){
		fBackgroundVoxelNumberVector.push_back(voxelNum);
	}
	//UpperLayer
	for(unsigned int voxelNum = (numOfParts-1)*numOfVoxelsInOneLayer ; voxelNum < totalNumOfVoxels; voxelNum++){
			fBackgroundVoxelNumberVector.push_back(voxelNum);
	}
	//LeftLayer and RightLayer
	for(unsigned int lNo = 0 ; lNo < numOfLayers; lNo++){
		for(unsigned int i = 0 ; i < numOfParts; i++){
			unsigned int voxelNum = lNo*numOfVoxelsInOneLayer+(i*numOfParts);
			fBackgroundVoxelNumberVector.push_back(voxelNum);

			//RightLayer
			voxelNum += (numOfParts-1);
			fBackgroundVoxelNumberVector.push_back(voxelNum);
		}
	}
	//FrontLayer and RearLayer
	for(unsigned int lNo = 0 ; lNo < numOfLayers; lNo++){
		for(unsigned int i = 0 ; i < numOfParts; i++){
			unsigned int voxelNum =lNo*numOfVoxelsInOneLayer+i;
			fBackgroundVoxelNumberVector.push_back(voxelNum);

			//RearLayer
			voxelNum += (numOfParts*(numOfParts-1));
			fBackgroundVoxelNumberVector.push_back(voxelNum);
		}
	}


	//Setting the size of fEachVoxelCountForAcceptance
	fEachVoxelCountForAcceptance.resize(totalNumOfVoxels);
	std::fill(fEachVoxelCountForAcceptance.begin(), fEachVoxelCountForAcceptance.end(), 0);




}

Voxelator::~Voxelator(){
//delete histVoxelValue;
//delete histVoxelCount;
}

void Voxelator::SetVoxelDim(double x, double y , double z){
	fEachVoxelDim.Set(x,y,z);
}

void Voxelator::SetVoxelizedVolumeDim(double halfX, double halfY, double halfZ){
	fVoxelizedVolumeDim.Set(halfX,halfY,halfZ);
}

void Voxelator::SetVoxelatorDim(double x, double y , double z){
	//fVoxelatorDim.Set(int(2*fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),int(2*fVoxelizedVolumeDim.y()/fEachVoxelDim.y()),
		//			  int(2*fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );
	fVoxelatorDim.Set((2*fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),(2*fVoxelizedVolumeDim.y()/fEachVoxelDim.y()),
						  (2*fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );
}

void Voxelator::SetVoxelator(double voxelizedVolHalfX,double voxelizedVolHalfY, double voxelizedVolHalfZ,
							 double voxelX,double voxelY, double voxelZ){
	fVoxelizedVolumeDim.Set(2*voxelizedVolHalfX,2*voxelizedVolHalfY,2*voxelizedVolHalfZ);
	fEachVoxelDim.Set(voxelX,voxelY,voxelZ);
	fVoxelatorDim.Set(int(fVoxelizedVolumeDim.x()/fEachVoxelDim.x()),int(fVoxelizedVolumeDim.y()/fEachVoxelDim.y()),
			          int(fVoxelizedVolumeDim.z()/fEachVoxelDim.z()) );
	CreateHistogram();
}


bool Voxelator::IsGenuine(Tracking::Vector3D<double> pocaPt){
	return (std::fabs(pocaPt.x()) < ((double)fVoxelizedVolumeDim.x())/2.) &&
		   (std::fabs(pocaPt.y()) < ((double)fVoxelizedVolumeDim.y())/2.) &&
		   (std::fabs(pocaPt.z()) < ((double)fVoxelizedVolumeDim.z())/2.);
}

#ifdef FIND_CANDIDATE_VOXEL
std::vector<int> Voxelator::FindCandidateVoxels(Track incoming, Track outgoing,
												Tracking::Vector3D<double> &inComingHitPt,
												Tracking::Vector3D<double> &outGoingHitPt){
	std::vector<int> vectOfVoxels;
	inComingHitPt = Delta::GetIntersection(incoming,GetVoxelizedVolumeDim().z()/2.,1);
	outGoingHitPt = Delta::GetIntersection(outgoing,-1*(GetVoxelizedVolumeDim().z()/2.),2);
	  std::cout << "InComingHitPoint : ";
	  inComingHitPt.Print();
	  std::cout << "OutgoingHitPoint : " ;
	  outGoingHitPt.Print();

	  double epsilon = 0.001;
	  Tomography::Track tr(inComingHitPt,outGoingHitPt);
	  Tracking::Vector3D<double> prevPoint(0.,0.,0.);
	  for(int i = 0 ; i < GetVoxelatorDim().z() ; i++){

		  Tracking::Vector3D<double> voxelHitPt = Delta::GetIntersection(tr,
				  	  	  	  (GetVoxelizedVolumeDim().z()/2.)-(i*GetEachVoxelDim().z()),
							  3);
		  voxelHitPt.SetZ(voxelHitPt.z()+epsilon);

		  std::cout<<"Voxel Hit Point for Slice : " << i << " : VoxelNumber : " << GetVoxelNumber(voxelHitPt) << std::endl;
					//<< " : PocaPt_VoxelNumber : " << GetVoxelNumber(fPocaPt) << " : Voxel Point ";
		  voxelHitPt.Print();
		  int voxNum=GetVoxelNumber(voxelHitPt);
		  vectOfVoxels.push_back(voxNum);
		  //fEachVoxelCountForAcceptance[voxNum]++;

		  if(i!=0){
			  double voxelDist = CommonFunc::Distance(voxelHitPt,prevPoint);
			  std::cout<< " VoxelDist : " << voxelDist << std::endl;
		  }

		  prevPoint = voxelHitPt;
	  }

	  return vectOfVoxels; // This is basically a vector of index of Hitted Voxel that influenced the muon track.
}

//Version for detector acceptance
std::vector<int> Voxelator::FindCandidateVoxels(Track incoming){
	std::vector<int> vectOfVoxels;
	Tracking::Vector3D<double> inComingHitPt = Delta::GetIntersection(incoming,GetVoxelizedVolumeDim().z()/2.,1);
	Tracking::Vector3D<double> outGoingHitPt = Delta::GetIntersection(incoming,-1*(GetVoxelizedVolumeDim().z()/2.),1);
	  std::cout << "InComingHitPoint in VOI : ";
	  inComingHitPt.Print();
	  std::cout << "OutgoingHitPoint in VOI  : " ;
	  outGoingHitPt.Print();

	  double epsilon = 0.001;
	  Tomography::Track tr(inComingHitPt,outGoingHitPt);
	  Tracking::Vector3D<double> prevPoint(0.,0.,0.);
	  for(int i = 0 ; i <= GetVoxelatorDim().z() ; i++){

		  Tracking::Vector3D<double> voxelHitPt = Delta::GetIntersection(tr,
				  	  	  	  (GetVoxelizedVolumeDim().z()/2.)-(i*GetEachVoxelDim().z()),
							  3);
		  voxelHitPt.SetZ(voxelHitPt.z()+epsilon);

		  std::cout<<"Voxel Hit Point for Slice : " << i << " : VoxelNumber : " << GetVoxelNumber(voxelHitPt) << std::endl;
					//<< " : PocaPt_VoxelNumber : " << GetVoxelNumber(fPocaPt) << " : Voxel Point ";
		  voxelHitPt.Print();
		  //vectOfVoxels.push_back(GetVoxelNumber(voxelHitPt));
		  int voxNum=GetVoxelNumber(voxelHitPt);
		  bool valid = voxNum >=0 && voxNum < GetTotalNumberOfVoxels();
		  if(valid){
		  vectOfVoxels.push_back(voxNum);
		  fEachVoxelCountForAcceptance[voxNum]++;
		  }

		  if(i!=0){
			  double voxelDist = CommonFunc::Distance(voxelHitPt,prevPoint);
			  std::cout<< " VoxelDist : " << voxelDist << std::endl;
		  }

		  prevPoint = voxelHitPt;
	  }
	  return vectOfVoxels; // This is basically a vector of index of Hitted Voxel that influenced the muon track.
}
#endif


int Voxelator::GetVoxelNumber(double x, double y, double z){
	int onX = (int)(x+fVoxelizedVolumeDim.x()/2.)/fEachVoxelDim.x();// + 1;
	int onY = (int)(y+fVoxelizedVolumeDim.y()/2.)/fEachVoxelDim.y();// + 1;
	int onZ = (int)(z+fVoxelizedVolumeDim.z()/2.)/fEachVoxelDim.z();// + 1;

	// std::cout<< onX <<" : "<< onY << " : " << onZ << std::endl;
	// return fVoxelatorDim.x()*fVoxelatorDim.y()*(onZ+fVoxelizedVolumeDim.z()/2.) +
	// 	   fVoxelatorDim.x()*(onY+fVoxelizedVolumeDim.y()/2.) + (onX+fVoxelizedVolumeDim.x()/2.) ;
	return GetVoxelNumber(onX,onY,onZ);

}

int Voxelator::GetVoxelNumber(int x, int y, int z){

	return fVoxelatorDim.x()*fVoxelatorDim.y()*z +
		   fVoxelatorDim.x()*y + x ;

}

int Voxelator::GetVoxelNumber(Tracking::Vector3D<double> vox){
	return GetVoxelNumber(vox.x(),vox.y(),vox.z());
}

Tracking::Vector3D<double> Voxelator::GetVoxelCenter(int voxelNum, bool &valid){

    //std::cout<< "Voxel Num from Voxelator : " << voxelNum << std::endl;
	valid = voxelNum < GetTotalNumberOfVoxels() && voxelNum >= 0;
    if(valid)
	   return fVoxelCenters[voxelNum];
    else 
    	return Tracking::Vector3D<double>(-1000000.,-1000000.,-1000000.);
       //return Tracking::Vector3D<double>(0.,0.,0.);
}

Tracking::Vector3D<double> Voxelator::GetVoxelCenter(double x, double y, double z, bool& valid){

	int voxelNumber = GetVoxelNumber(x,y,z);
	std::cout<< "######### Voxel Number : " << voxelNumber << std::endl;
	valid = voxelNumber > 0 && voxelNumber < GetTotalNumberOfVoxels();
	if(valid)
		return fVoxelCenters[voxelNumber];
	else
		return Tracking::Vector3D<double>(-1000000.,-1000000.,-1000000.);
}

Tracking::Vector3D<double> Voxelator::GetVoxelCenter(Tracking::Vector3D<double> vox, bool &valid){

	return GetVoxelCenter(vox.x(),vox.y(),vox.z(),valid);
}

void Voxelator::CalculateVoxelCenters(){

//	std::cout << fVoxelizedVolumeDim.x() << " : " << fVoxelatorDim.x() << std::endl;
//	std::cout << fVoxelizedVolumeDim.y() << " : " << fVoxelatorDim.y() << std::endl;
//	std::cout << fVoxelizedVolumeDim.z() << " : " << fVoxelatorDim.z() << std::endl;
	//fVoxelizedVolumeDim.Print();
	for(int k = 0 ; k < fVoxelatorDim.z() ; k++){
		for(int j = 0 ; j < fVoxelatorDim.y() ; j++){
			for(int i = 0 ; i < fVoxelatorDim.x() ; i++){
				double x = -fVoxelizedVolumeDim.x()/2. + fEachVoxelDim.x()/2. + (fEachVoxelDim.x() * (i));
				double y = -fVoxelizedVolumeDim.y()/2. + fEachVoxelDim.y()/2. + (fEachVoxelDim.y() * (j));
				double z = -fVoxelizedVolumeDim.z()/2. + fEachVoxelDim.z()/2. + (fEachVoxelDim.z() * (k));
				fVoxelCenters.push_back(Tracking::Vector3D<double>(x,y,z));
			}
		}
	}
}

void Voxelator::PrintVoxelCenters(){
	std::cout<<"Size of VoxelCenters : " << fVoxelCenters.size() << std::endl;
	for(int i = 0 ; i < fVoxelCenters.size() ; i++){
		fVoxelCenters[i].Print();
	}
}


/*
void Voxelator::CreateSDGraph(){
	std::vector<int> x;
	int numOfVoxels = fVoxelatorDim.x()*fVoxelatorDim.y()*fVoxelatorDim.z();
	for(int i = 0 ; i < numOfVoxels ; i++){
		x.push_back(i);
	}
	//fGraphSD = new TGraph(numOfVoxels, &x[0],);
}
*/

void Voxelator::CreateHistogram(){
	//Create the required 3D Hist which corresponds to voxelized volume
	histVoxelValue = new  TH3F("histVoxelValue", "HistVoxelValue", fVoxelatorDim.x(), -fVoxelizedVolumeDim.x()/2., fVoxelizedVolumeDim.x()/2.,
						                                           fVoxelatorDim.y(), -fVoxelizedVolumeDim.y()/2., fVoxelizedVolumeDim.y()/2.,
																   fVoxelatorDim.z(), -fVoxelizedVolumeDim.z()/2., fVoxelizedVolumeDim.z()/2.) ;

	histVoxelCount = new TH3F("histVoxelCount", "HistVoxelCount", fVoxelatorDim.x(), -fVoxelizedVolumeDim.x()/2., fVoxelizedVolumeDim.x()/2.,
																  fVoxelatorDim.y(), -fVoxelizedVolumeDim.y()/2., fVoxelizedVolumeDim.y()/2.,
																  fVoxelatorDim.z(), -fVoxelizedVolumeDim.z()/2., fVoxelizedVolumeDim.z()/2.) ;

	std::cout << "Total num of voxel for simulations : " << GetTotalNumberOfVoxels() << std::endl;
	fVoxelsIn1D = new TH1F("IDHistOfVoxels","IDHistOfVoxels",GetTotalNumberOfVoxels(), 0, GetTotalNumberOfVoxels());
	fVoxelsIn1DCount = new TH1F("IDHistOfVoxelsCount","IDHistOfVoxelsCount",GetTotalNumberOfVoxels(), 0, GetTotalNumberOfVoxels());

	fSDInVoxels = new TH1F("SDInVoxels","SDInVoxels",100,0,.30);
	fRLInVoxels = new TH1F("RLInVoxels","RLInVoxels",100,0,20.);

	fWeightedCountHist = new TH1F("WeighedCountHist","WeighedCountHist",GetTotalNumberOfVoxels(), 0, GetTotalNumberOfVoxels());
	fNoisyLayerPointCountHist = new TH1F("fNoisyLayerPointCountHist","fNoisyLayerPointCountHist",50,0.,50.);



}

double Voxelator::GetAverageScatteringAngleInAVoxel(Vector3D<double> vox){
return GetAverageScatteringAngleInAVoxel(vox.x(),vox.y(),vox.z());
}

double Voxelator::GetAverageScatteringAngleInAVoxel(int x, int y, int z){
//return histVoxelValue->GetBinContent(x,y,z);
	return fVoxelsIn1D->GetBinContent(GetVoxelNumber(x,y,z));
}

void Voxelator::Insert(double x, double y, double z, double w){
//std::cout<<"Inside INSERTTTTTTTTTTTTTTTTTTT...................." << std::endl;
histVoxelCount->Fill(x,y,z);
histVoxelValue->Fill(x,y,z,w);
int voxelNumber = GetVoxelNumber(x,y,z);
fVoxelsIn1D->Fill(voxelNumber,std::fabs(w));
fVoxelsIn1DCount->Fill(voxelNumber);

#ifdef STORE_SLICE
int numOfVoxelsInASlice = fVoxelatorDim.x()*fVoxelatorDim.y();
//std::cout<<"NumofVoxelsInAslice : " << numOfVoxelsInASlice << std::endl;
int sliceNum = 0;
if(voxelNumber % numOfVoxelsInASlice)
	sliceNum = (voxelNumber/numOfVoxelsInASlice) + 1;
else
	sliceNum = (voxelNumber/numOfVoxelsInASlice);

/*if(sliceNum != 6 && sliceNum != 7 && sliceNum != 8 ){
	std::cout << "Not expected SliceNum : "<< sliceNum << std::endl;
}*/

if(sliceNum < 0 || sliceNum > fVoxelatorDim.z()){
	std::cout<<"Unexpected slice : "<< sliceNum <<"  : For VoxelNum : "<< voxelNumber << ".... Skipping this guy....." << std::endl;
	return;
}
std::string histName = "ZSlice_"+std::to_string(sliceNum);
fObjChecker.CheckAndInsert(histName.c_str(),histName.c_str(),fVoxelatorDim.x(), -fVoxelatorDim.x()/2.,fVoxelatorDim.x()/2.,
															 fVoxelatorDim.y(), -fVoxelatorDim.y()/2.,fVoxelatorDim.y()/2.,
															 (x/fEachVoxelDim.x()),(y/fEachVoxelDim.y()),w);
#endif

}

void Voxelator::Insert(Vector3D<double> vox){
Insert(vox.x(),vox.y(),vox.z(),vox.w());
}

void Voxelator::Insert(double x, double y, double z, int w){
Insert(x,y,z,double(w));
}

//Very important. Used to set the average value in each voxel
void Voxelator::AverageOut(){
/*  for(int x = 0 ; x < fVoxelatorDim.x() ; x++){
    for(int y = 0 ; y < fVoxelatorDim.y() ; y++){
      for(int z = 0 ; z < fVoxelatorDim.z() ; z++){
	if(histVoxelCount->GetBinContent(x,y,z)){
	double averageVal = histVoxelValue->GetBinContent(x,y,z)/histVoxelCount->GetBinContent(x,y,z);
	histVoxelValue->SetBinContent(x,y,z,averageVal);
	}
      }
    }
  }
  */

  //Using Root histogram divide function
  histVoxelValue->Divide(histVoxelCount);
}

void Voxelator::Insert(std::vector<Vector3D<double>> pocaPtVect){
int size = pocaPtVect.size();
for (auto &pocaPt : pocaPtVect) {
      Insert(pocaPt);
  }
AverageOut();
}

void Voxelator::InsertCleanVoxels(std::vector<Vector3D<double>> ptVect){

}

void Voxelator::FillSDAndRLHist(){
	//std::vector<Voxel*> fVoxelVector = Voxel::GetVoxelVector();
	std::vector<Voxel_V2*> fVoxelVector = Tomography::evolution::Voxelator::instance()->GetVoxelVector();
	for (auto &voxel : fVoxelVector) {
		int voxelNumber = voxel->GetVoxelNum();
		double sd = voxel->GetStandardDeviation();
		double rl = voxel->GetRadiationLength();
		if(sd != 0.){
			//fSDInVoxels->Fill(voxelNumber,sd);
			fSDInVoxels->Fill(sd);
		}
		if(rl < 10){
			//fRLInVoxels->Fill(voxelNumber,rl);
			fRLInVoxels->Fill(rl);
		}
	}
}

void Voxelator::SetMaxMinSDAndRL(){
	//std::vector<Voxel*> fVoxelVector = Voxel::GetVoxelVector();
	std::vector<Voxel_V2*> fVoxelVector = Tomography::evolution::Voxelator::instance()->GetVoxelVector();
	std::vector<double> sdVect, rlVect;
	for (auto &voxel : fVoxelVector) {
		double sd = voxel->GetStandardDeviation();
		double rl = voxel->GetRadiationLength();
		if(std::isfinite(sd) && std::isfinite(rl) ){
			sdVect.push_back(sd);
			rlVect.push_back(rl);
		}
	}
	/*
	fMaxSD = *max_element(sdVect.begin(), sdVect.end());
	fMinSD = *min_element(sdVect.begin(), sdVect.end());
	fMaxRL = *max_element(rlVect.begin(), rlVect.end());
	fMinRL = *min_element(rlVect.begin(), rlVect.end());
	*/
}

}//end of evolution namespace
} // end of Tomography namespace
