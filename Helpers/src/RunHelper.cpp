/*
 * RunHelper.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: rsehgal
 */

#include "RunHelper.h"
#include "Voxel.h"
#include "CommonFunc.h"
#include <TFile.h>
#include "Voxelator_Evolution.h"

namespace Tomography {

void RunHelper::Reset(){
	fScatteringAngleVector.clear();
	fPocaPtVector.clear();
	fSDOfVoxelsVector.clear();
	fRLOfVoxelsVector.clear();
	for(int i = 0 ; i < fVoxelVector.size() ; i++){
		delete fVoxelVector[i];
	}
	fVoxelVector.clear();
}

RunHelper::RunHelper() {
	// TODO Auto-generated constructor stub
    /* Here we should do the stuff that we want to do at the end of Run
     * Like..  Calculation of SD for the whole run.
     *         Calculation of SD for individual voxel
     */

    //WARNING : DO NOT ALTER THE BELOW MENTIONED SEQUENCE
    fFileType = "";
    //fVoxelVector = Voxel::GetVoxelVector();
    fVoxelVector = Tomography::evolution::Voxelator::instance()->GetVoxelVector();
    FillPocaVector();
    FillScatteringAngleVector();
//    CalcSDOfEachVoxel();
//    FillSDVector();
//    CalcRLOfEachVoxel();
//    FillRLVector();

    WriteToFile();
     

}

RunHelper::RunHelper(std::string fileType) {
    // TODO Auto-generated constructor stub
    /* Here we should do the stuff that we want to do at the end of Run
     * Like..  Calculation of SD for the whole run.
     *         Calculation of SD for individual voxel
     */

	//Resetting RunHelper
	//Reset();

    //WARNING : DO NOT ALTER THE BELOW MENTIONED SEQUENCE
    fFileType = fileType;
    //fVoxelVector = Voxel::GetVoxelVector();
    fVoxelVector = Tomography::evolution::Voxelator::instance()->GetVoxelVector();
    std::cout << "Hello : VoxelVectorSize :  " << fVoxelVector.size() << std::endl;
    FillPocaVector();
    FillScatteringAngleVector();
    CalcSDOfEachVoxel();
    FillSDVector();
    CalcRLOfEachVoxel();
    FillRLVector();
    SetMaxMinSDAndRL();


    WriteToFile();

    //Resetting RunHelper in the help, This will do the resetting process for next processing
    Reset();

}

void RunHelper::PrintMaxMinSDAndRL() const{
	std::cout << "MinSD : " << fMinSD << " : MaxSD : " << fMaxSD << std::endl;
	std::cout << "MinRL : " << fMinRL << " : MaxRL : " << fMaxRL << std::endl;
}

void RunHelper::SetMaxMinSDAndRL(){
	Tomography::evolution::Voxelator::instance()->SetMaxMinSDAndRL();
	fMaxSD = Tomography::evolution::Voxelator::instance()->GetMaxSD();
	fMinSD = Tomography::evolution::Voxelator::instance()->GetMinSD();
	fMaxRL = Tomography::evolution::Voxelator::instance()->GetMaxRL();
	fMinRL = Tomography::evolution::Voxelator::instance()->GetMinRL();
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

/*One should not do filtering here itself.
* Hence commenting the code that does the filtering
*/
void RunHelper::FillPocaVector(){
    for(int i = 0 ; i < fVoxelVector.size() ; i++){

    	std::vector<Tracking::Vector3D<double>> pocaPointsVector = fVoxelVector[i]->GetPocaPointsVector();
        //std::vector<Tracking::Vector3D<double>*> pocaPointsVector = fVoxelVector[i]->GetPocaPointsVector();
    	        for(int j = 0 ; j  < pocaPointsVector.size() ; j++){
    	            Insert(pocaPointsVector[j]);
    	        	//Insert(Tracking::Vector3D<double>(pocaPointsVector[j]->x(),pocaPointsVector[j]->y(),pocaPointsVector[j]->z()));
    	}
    }
}

#ifdef STORE
void RunHelper::Store(){
	Tomography::evolution::Voxelator *vox = Tomography::evolution::Voxelator::instance();
	vox->FillSDAndRLHist();

	TFile flVox((fFileType+".root").c_str(),"recreate");
	vox->Insert(fPocaPtVector); //Voxelized Poca Ready
	std::ofstream voxTrack;
	voxTrack.open("VoxelizedTracks.txt");
	Vector3D<double> voxelatorDim = vox->GetVoxelatorDim();
	std::cout<< " -+-+-+-+--+-+-+--+--+-+--+-------+---++- " << std::endl;
	voxelatorDim.Print();
	Vector3D<double> voxelizedVolumeDim = vox->GetVoxelizedVolumeDim();
	Vector3D<double> voxelDim = vox->GetEachVoxelDim();
	for(int x = 0 ; x < voxelatorDim.x()-1 ; x++){
	    for(int y = 0 ; y < voxelatorDim.y()-1 ; y++){
	      for(int z = 0 ; z < voxelatorDim.z()-1 ; z++){
		if(vox->GetVoxelizedCount()->GetBinContent(x,y,z))
		//voxTrack << x << " " << y << " " << z << " " <<  vox.GetVoxelizedHist()->GetBinContent(x,y,z) << std::endl;
		voxTrack << (-voxelizedVolumeDim.x()+x*voxelDim.x()) << " " << (-voxelizedVolumeDim.y()+y*voxelDim.y())
				 << " " << (-voxelizedVolumeDim.z()+z*voxelDim.z())  << " " <<  vox->GetVoxelizedHist()->GetBinContent(x,y,z) << std::endl;
	      }
	    }
	  }

	vox->GetVoxelIn1D()->Write();
	vox->GetVoxelIn1DCount()->Write();
	vox->GetSDInVoxelsHist()->Write();
	vox->GetRLInVoxelsHist()->Draw("COLZ");
	vox->GetRLInVoxelsHist()->Write();


	#ifdef STORE_SLICE
	std::set<Tomography::ObjectChecker> histSet = vox->GetObjectChecker().GetSet();
	for (std::set<Tomography::ObjectChecker>::iterator it=histSet.begin(); it!=histSet.end(); ++it){
		(*it).GetHist()->Write();
	}
	#endif

}

#endif

void RunHelper::WriteToFile(){
#ifdef STORE
	Store();
#endif
    CommonFunc::Functions::instance()->WriteToFile("scatteringRunHelper-"+fFileType+".txt",fScatteringAngleVector);
    CommonFunc::Functions::instance()->WriteToFile("PocaPtRunHelper-"+fFileType+".txt",fPocaPtVector);
//    CommonFunc::Functions::instance()->WriteToFile("VoxelsRunHelper-"+fFileType+".txt",Tomography::Voxel::GetVoxelVector());
//    CommonFunc::Functions::instance()->WriteToFile("filteredVoxelsRunHelper-"+fFileType+".txt",Tomography::Voxel::GetFilteredVoxelVector());
//    CommonFunc::Functions::instance()->WriteToFile("filteredPocaPtRunHelper-"+fFileType+".txt",Tomography::Voxel::GetFilteredPocaPtVector());
    CommonFunc::Functions::instance()->WriteToFile("VoxelsRunHelper-"+fFileType+".txt",Tomography::evolution::Voxelator::instance()->GetVoxelVector());
    CommonFunc::Functions::instance()->WriteToFile("filteredVoxelsRunHelper-"+fFileType+".txt",Tomography::evolution::Voxelator::instance()->GetFilteredVoxelVector());
    CommonFunc::Functions::instance()->WriteToFile("filteredPocaPtRunHelper-"+fFileType+".txt",Tomography::evolution::Voxelator::instance()->GetFilteredPocaPtVector());


}

RunHelper::~RunHelper() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
