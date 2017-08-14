/*
 * RunHelper.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: rsehgal
 */

#include "RunHelper.h"
#include "Voxel.h"
#include "CommonFunc.h"

namespace Tomography {

RunHelper::RunHelper() {
	// TODO Auto-generated constructor stub
    /* Here we should do the stuff that we want to do at the end of Run
     * Like..  Calculation of SD for the whole run.
     *         Calculation of SD for individual voxel
     */

    //WARNING : DO NOT ALTER THE BELOW MENTIONED SEQUENCE
    fVoxelVector = Voxel::GetVoxelVector();
    FillPocaVector();
    FillScatteringAngleVector();
    CalcSDOfEachVoxel();
    FillSDVector();
    CalcRLOfEachVoxel();
    FillRLVector();

    WriteToFile();
     

}

void RunHelper::FillSDVector(){
    
     for(int i = 0 ; i < fVoxelVector.size() ; i++){
        fSDOfVoxelsVector.push_back(fVoxelVector[i]->GetStandardDeviation());
     }
}

void RunHelper::FillRLVector(){

     for(int i = 0 ; i < fVoxelVector.size() ; i++){
        fRLOfVoxelsVector.push_back(fVoxelVector[i]->GetRadiationLength());
     }
}

void RunHelper::CalcSDOfEachVoxel(){
    
     for(int i = 0 ; i < fVoxelVector.size() ; i++){
        fVoxelVector[i]->CalcSD();
     }
}

void RunHelper::CalcRLOfEachVoxel(){

     for(int i = 0 ; i < fVoxelVector.size() ; i++){
        fVoxelVector[i]->CalcRadiationLength();
     }
}


void RunHelper::FillScatteringAngleVector(){
    for(int i = 0 ; i < fPocaPtVector.size() ; i++){
        Insert(fPocaPtVector[i].GetColor());
    }
}

void RunHelper::FillPocaVector(){
    for(int i = 0 ; i < fVoxelVector.size() ; i++){
        if(fVoxelVector[i]->IsOutlier()){

        }else{
        std::vector<Tracking::Vector3D<double>> pocaPointsVector = fVoxelVector[i]->GetPocaPointsVector();
        for(int j = 0 ; j  < pocaPointsVector.size() ; j++){
            Insert(pocaPointsVector[j]);
        }
    }
    }

}

void RunHelper::WriteToFile(){
    CommonFunc::Functions::WriteToFile("scattering.txt",fScatteringAngleVector);
    CommonFunc::Functions::WriteToFile("PocaPt.txt",fPocaPtVector);
    CommonFunc::Functions::WriteToFile("Voxels.txt",Tomography::Voxel::GetVoxelVector());


}

RunHelper::~RunHelper() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
