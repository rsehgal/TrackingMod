/*
 * ScatteringDensityV2.h
 *
 *  Created on: 02-Jan-2019
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_SCATTERINGDENSITYV2_H_
#define HELPERS_INC_SCATTERINGDENSITYV2_H_

#include "Voxelator_Evolution.h"

namespace Tomography {

class ScatteringDensityV2 {
	static ScatteringDensityV2 *s_instance;
	static bool iniflag;

	int sNumOfVoxels;
	std::vector<double> sLambda;

	std::vector<double> sTotalPathLength;
	ScatteringDensityV2();

public:
	//ScatteringDensityV2();
	static ScatteringDensityV2 *instance();
	static ScatteringDensityV2 *Create();
	virtual ~ScatteringDensityV2();

	void FillLambdaValue(int voxelNum, double value){
			//std::cout << "VoxelNum : " << voxelNum << __FILE__ << " : " << __LINE__ << std::endl;
			if(voxelNum > 0 && voxelNum < Tomography::evolution::Voxelator::instance()->GetTotalNumberOfVoxels()){
				sLambda[voxelNum] = value;

			}

	}


	void InitializeZero(){
				if(iniflag)
					return;

				iniflag = true;
				for(unsigned int i = 0 ; i < sNumOfVoxels ; i++ ){
					//Initializing to default value of zero
					sLambda.push_back(0.);
					sTotalPathLength.push_back(0.);
				}
	}

	void AddPathLengthInVoxel(unsigned voxelNum, double pathLength){
		sTotalPathLength[voxelNum] += pathLength;
	}

	void Initialize(){
			if(iniflag)
				return;

			iniflag = true;
			for(unsigned int i = 0 ; i < sNumOfVoxels ; i++ ){
				//Initializing to default value of zero
				sLambda.push_back(0.);
				sTotalPathLength.push_back(0.);
			}

			std::vector<Voxel_V2*> voxelVector = Tomography::evolution::Voxelator::instance()->GetVoxelVector();
			for(unsigned int i = 0  ; i < voxelVector.size() ; i++){
				unsigned int voxelNum = voxelVector[i]->GetVoxelNum();
				double rl = voxelVector[i]->GetRadiationLength();
				if(rl != 0.){
					//sLambda[voxelNum] = (1./rl);
					std::cout <<"Sehgal : RL :  " << rl << std::endl;

					FillLambdaValue(voxelNum,(1./rl));
				}
				//FillLambdaValue(voxelNum,(1.e-5));

			}
		}

	void Print(){
			Tomography::evolution::Voxelator *voxelator = Tomography::evolution::Voxelator::instance();
			int numOfVoxelsInOneSlice = voxelator->GetVoxelatorDim().x()*voxelator->GetVoxelatorDim().y();
			std::cout << "Num Of Voxels in One Slice : " << numOfVoxelsInOneSlice << std::endl;
			for(unsigned int i = 0 ; i < sNumOfVoxels ; i++){

				std::cout << sLambda[i] << " ";

				if(!((i+1)%numOfVoxelsInOneSlice) && i!=0)
					std::cout << std::endl << " i :  " << i << std::endl;
			}
	}

	std::vector<double> GetLambda() const {return sLambda;}
	std::vector<double> GetTotalPathLengthVector() const {return sTotalPathLength;}
};

} /* namespace Tomography */

#endif /* HELPERS_INC_SCATTERINGDENSITYV2_H_ */
