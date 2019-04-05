/*
 * MLEM.h
 *
 *  Created on: 19-Dec-2018
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_MLEM_H_
#define HELPERS_INC_MLEM_H_

#include "base/Vector3D.h"
#include <string>
#include "VoxelV2.h"
#include "Voxelator_Evolution.h"
#include <TMatrixD.h>
#include <TMatrix.h>
#include "ScatteringDensityV2.h"
#include <fstream>

namespace Tomography {

class Track;

/*double Trace(TMatrixD T) {
	double trace = 0;
	for (int i = 0; i < T.GetNrows(); i++) {
		trace += T(i, i);
	}
	return trace;
}*/

struct ScatteringData{
	double sDeltaThetaX;
	double sDeltaThetaY;
	double sDeltaX;
	double sDeltaY;
	double sPr;

	double sEx;
	double sEy;
	double sExy;

	ScatteringData(){}

	ScatteringData(double deltaThetaX, double deltaThetaY,
				   double deltaX, double deltaY, double pr, double eX, double eY, double eXY):
				   sDeltaThetaX(deltaThetaX),sDeltaThetaY(deltaThetaY),
				   sDeltaX(deltaX),sDeltaY(deltaY),sPr(pr),sEx(eX), sEy(eY),
				   sExy(eXY){}

	void SetScatteringData(double deltaThetaX, double deltaThetaY,
			   double deltaX, double deltaY, double pr, double eX, double eY, double eXY){

		sDeltaThetaX = deltaThetaX;
		sDeltaThetaY = deltaThetaY;
		sDeltaX = deltaX;
		sDeltaY = deltaY;
		sPr = pr;
		sEx = eX;
		sEy = eY;
		sExy = eXY;

	}

	void Print(){
		std::cout << "===========================================================================" << std::endl;
		std::cout << "DeltaThetaX : " << sDeltaThetaX <<" : DeltaThetaY : " << sDeltaThetaY << std::endl;
		std::cout << "DeltaX : " << sDeltaX <<" : DeltaY : " << sDeltaY << std::endl;
		std::cout << "Ex : " << sEx << " : Ey : " << sEy << " : Exy : " << sExy << " : pR : " << sPr << std::endl;
		std::cout << "===========================================================================" << std::endl;
	}

};


/*
// May be NOT required
struct WeightedMatrix{

};

struct VoxelNumAndWeightedMatrix{
	int sVoxelNum;
	WeightedMatrix sWeightedMatrix;
	VoxelNumAndWeightedMatrix(int voxelNum , WeightedMatrix wm) : sVoxelNum(voxelNum){}
};
*/

struct LTOfEachVoxel{
	//Tomography::Track sIncomingTrack;
	//Tomography::Track sOutgoingTrack;
	int sVoxelNum;
	double sL;
	double sT;
	double sWx;
	double sWy;
	double sWxy;
	LTOfEachVoxel(int voxelNum, double l, double t):sVoxelNum(voxelNum),sL(l), sT(t), sWx(0.), sWy(0.), sWxy(0.){}
	/*LTOfEachVoxel(int voxelNum, double l, double t,Tomography::Track incoming,Tomography::Track outgoing):sVoxelNum(voxelNum),sL(l), sT(t), sWx(0.), sWy(0.), sWxy(0.){
		sIncomingTrack = incoming;
		sOutgoingTrack = outgoing;
	}*/
	void Print(){
		std::cout << "VoxelNum : " << sVoxelNum << " : VoxelPathLength : " << sL << " : TotalPathLength : " << sT ; // << std::endl;
		PrintWeightedMatrix();
	}
	void CreateWeightedMatrix(double totalPathLength){
		std::cout << "Supplied Total Path Length : " << totalPathLength << std::endl;
		sT = totalPathLength - sT;
		sWx = sL;
		sWy = 0.333333*sL*sL*sL + sL*sL*sT + sL*sT*sT;
		sWxy = 0.5*sL*sL + sL*sT;
	}
	void PrintWeightedMatrix(){
		std::cout << "  : Weighted Matrix : sWx : " << sWx <<" : sWy : " << sWy << " : sWxy : " << sWxy<< std::endl;
	}
};

//This SHOULD be a separate singleton class
//So that it can be access by individual event
//in different iteration.

struct ScatteringDensity{

	unsigned int sNumOfVoxels;
	std::vector<double> sLambda;


	ScatteringDensity() {
		sNumOfVoxels = Tomography::evolution::Voxelator::instance()->GetTotalNumberOfVoxels();
		Initialize();
	}

	ScatteringDensity(unsigned int numOfVoxels) : sNumOfVoxels(numOfVoxels){
			Initialize();
	}

	void FillLambdaValue(int voxelNum, double value){
		//std::cout << "VoxelNum : " << voxelNum << __FILE__ << " : " << __LINE__ << std::endl;
		if(voxelNum > 0 && voxelNum < Tomography::evolution::Voxelator::instance()->GetTotalNumberOfVoxels()){
			sLambda[voxelNum] = value;
		}

	}
	void Initialize(){
		for(unsigned int i = 0 ; i < sNumOfVoxels ; i++ ){
			//Initializing to default value of zero
			sLambda.push_back(0.);
		}

		std::vector<Voxel_V2*> voxelVector = Tomography::evolution::Voxelator::instance()->GetVoxelVector();
		for(unsigned int i = 0  ; i < voxelVector.size() ; i++){
			unsigned int voxelNum = voxelVector[i]->GetVoxelNum();
			double rl = voxelVector[i]->GetRadiationLength();
			if(rl != 0.){
				//sLambda[voxelNum] = (1./rl);
				FillLambdaValue(voxelNum,(1./rl));
			}
		}
	}

	void Print(){
		for(unsigned int i = 0 ; i < sNumOfVoxels ; i++){

			std::cout << sLambda[i] << " ";
			if(!((i+1)%25) && i!=0)
				std::cout << std::endl << " i :  " << i << std::endl;
		}
	}
};


struct EachMuonData{

	std::vector<LTOfEachVoxel> sLTVectorForEachMuon;
	ScatteringData sScatteringData;
	std::vector<double> sNewLambdaFromMuon;


	//Flag to store validity of Covariance Matrix
	bool sIsInvertibleCovarianceMatrix;

	EachMuonData(std::vector<LTOfEachVoxel> ltVectorForEachTrack):sLTVectorForEachMuon(ltVectorForEachTrack){}
	EachMuonData(std::vector<LTOfEachVoxel> ltVectorForEachTrack, ScatteringData scatteringData):sLTVectorForEachMuon(ltVectorForEachTrack){
		this->sScatteringData.sDeltaThetaX = scatteringData.sDeltaThetaX;
		this->sScatteringData.sDeltaThetaY = scatteringData.sDeltaThetaY;
		this->sScatteringData.sDeltaX = scatteringData.sDeltaX;
		this->sScatteringData.sDeltaY = scatteringData.sDeltaY;
		this->sScatteringData.sEx = scatteringData.sEx;
		this->sScatteringData.sEy = scatteringData.sEy;
		this->sScatteringData.sExy = scatteringData.sExy;
		this->sScatteringData.sPr = scatteringData.sPr;

		sIsInvertibleCovarianceMatrix = true;

		WriteScatteringData();
		CreateWeightedMatrix();
		Print();
		SetCovarianceMatrix();
		SetConditionalExpectation();
	}

	//Just changed the position of this function for readability
	void CreateWeightedMatrix() {
		int size = sLTVectorForEachMuon.size();
		for (int j = 0; j < size; j++) {
			double totalPathLength = sLTVectorForEachMuon[size - 1].sT;
			sLTVectorForEachMuon[j].CreateWeightedMatrix(totalPathLength);
		}
	}

	void WriteScatteringData(){
		std::ofstream outfile("ScatteringData.txt",std::ios::app);
		outfile << this->sScatteringData.sDeltaThetaX << " "
				<< this->sScatteringData.sDeltaThetaY << " "
				<< this->sScatteringData.sDeltaX << " "
				<< this->sScatteringData.sDeltaY << " "
				<< this->sScatteringData.sPr << std::endl;
		outfile.close();
	}

	//Variables to store covariance matrix
	double sCx;
	double sCy;
	double sCxy;



	double Trace(TMatrixD T) {
		double trace = 0;
		for (int i = 0; i < T.GetNrows(); i++) {
			trace += T(i, i);
		}
		return trace;
	}

	void SetConditionalExpectation(){

		Tomography::ScatteringDensityV2 *s = Tomography::ScatteringDensityV2::instance();

		int nVoxelsInEvent= sLTVectorForEachMuon.size() ; //(int)fVoxelDataCollection.size();

		//## Calculate S= 2xlambda + (a-b)*(pxlambda)^2
		//## a= Dt CInv W CInv D
		//## b= Tr(CInv W)
		TMatrixD Dx= TMatrix(2,1);
		Dx(0,0)= this->sScatteringData.sDeltaThetaX;	// fDeltaThetaX;
		Dx(1,0)= this->sScatteringData.sDeltaX; 		// fDeltaX;

		TMatrixD Dxt= TMatrix(1,2);
		Dxt= TMatrixD(TMatrixD::kTransposed, Dx);

		TMatrixD Dy= TMatrix(2,1);
		Dy(0,0)= this->sScatteringData.sDeltaThetaY; //fDeltaThetaY;
		Dy(1,0)= this->sScatteringData.sDeltaY; //fDeltaY;

		TMatrixD Dyt= TMatrix(1,2);
		Dyt= TMatrixD(TMatrixD::kTransposed, Dy);


		double CDet= sCx*sCy-sCxy*sCxy;

		if (CDet<=0) {
			cerr<<"MuonEventData::SetConditionalExpectation(): WARNING: Covariance matrix inversion failed (determinant is zero or negative)!"<<endl;
			//exit(1);
			sIsInvertibleCovarianceMatrix = false;
			return ;
		}

		TMatrixD CInv(2,2);
		CInv(0,0)= sCy/CDet;
		CInv(1,1)= sCx/CDet;
		CInv(0,1)= -sCxy/CDet;
		CInv(1,0)= CInv(0,1);

		//cout<<"*** TEST CINV ***"<<endl;
/*
		TMatrixD C(2,2);
		C(0,0)= fCx;
		C(1,1)= fCy;
		C(0,1)= fCxy;
		C(1,0)= C(0,1);

		TMatrixD CCInv(2,2);
		CCInv= C*CInv;
*/
		//CCInv.Print();
		//cout<<endl;



/*
		double LogLikelihood_x= 0.5*(log(CDet) + (Dxt*CInv*Dx)(0,0) );
		double LogLikelihood_y= 0.5*(log(CDet) + (Dyt*CInv*Dy)(0,0) );
		fLikelihood= LogLikelihood_x + LogLikelihood_y;
*/

		//cout<<"*** Dx ***"<<endl;
		//Dx.Print();

		//for(unsigned int j=0;j<fVoxelDataCollection.size();j++){
		for(unsigned int j=0;j< sLTVectorForEachMuon.size();j++){
			//VoxelData voxel= fVoxelDataCollection[j];
			int voxelId = sLTVectorForEachMuon[j].sVoxelNum ; //voxel.fId;

			TMatrixD W= TMatrixD(2,2);
			W(0,0)=  sLTVectorForEachMuon[j].sWx;  //voxel.fWx;
			W(1,1)=  sLTVectorForEachMuon[j].sWy;  // voxel.fWy;
			W(0,1)=  sLTVectorForEachMuon[j].sWxy; //voxel.fWxy;
			W(1,0)= W(0,1);

			double b= Trace( CInv*W );
			//cout<<"*** CInv *** "<<endl;
			//CInv.Print();
			//cout<<"*** W *** "<<endl;
			//W.Print();
			//cout<<"*** CInv W *** "<<endl;
			//(CInv*W).Print();
			//cout<<"Trace= "<<b<<endl;

			TMatrixD ProdMatrix(1,1);
			ProdMatrix.Zero();
			ProdMatrix= Dxt*CInv*W*CInv*Dx;
			double ax= ProdMatrix(0,0);
			double Sx = 2*s->GetLambda()[voxelId] + (ax-b)* pow(sScatteringData.sPr * (s->GetLambda()[voxelId]),2);//= 2.*par[voxelId] + (ax-b)* pow(fPr*par[voxelId],2);

			ProdMatrix.Zero();
			ProdMatrix= Dyt*CInv*W*CInv*Dy;
			double ay= ProdMatrix(0,0);
			double Sy=2*s->GetLambda()[voxelId] + (ay-b)* pow(sScatteringData.sPr * (s->GetLambda()[voxelId]),2);// 2.*par[voxelId] + (ay-b)* pow(fPr*par[voxelId],2);

			double S= (Sx+Sy)/2.;
			sNewLambdaFromMuon.push_back(S);

			//cout<<"voxelId="<<voxelId<<"  ay="<<ay<<"  by="<<by<<"  Sy="<<Sy<<endl;
			//## DEBUG
			/*
			if(voxelId==2850){
				cout<<"Voxel no. "<<voxelId<<"  Event no. "<<fEventId<<"  Sx="<<Sx<<"  Sy="<<Sy<<"  S="<<S<<endl;
				cout<<"W"<<endl;
				W.Print();
				cout<<"Dx"<<endl;
				Dx.Print();
				cout<<"Dy"<<endl;
				Dy.Print();
				cout<<"CInv"<<endl;
				CInv.Print();
			}
			*/

			//## add S info to each voxel
			//fVoxelDataCollection[j].fS= S;

	}
	}

	void SetCovarianceMatrix(){
		//ScatteringDensity s;
		//## Calculate covariance matrix
		Tomography::ScatteringDensityV2 *s = Tomography::ScatteringDensityV2::instance();

		TMatrixD scatteringMatrixSum(2, 2);
		scatteringMatrixSum.Zero();


		TMatrixD E(2, 2);
		E(0, 0) = sScatteringData.sEx;  // fEx;
		E(1, 1) = sScatteringData.sEy;  //fEy;
		E(0, 1) = sScatteringData.sExy; //fExy;
		E(1, 0) = E(0, 1);


		//cout<<"--> Muon Event no. "<<fEventId<<endl;

/*
		for (unsigned int j = 0; j < fVoxelDataCollection.size(); j++) {
			VoxelData voxel = fVoxelDataCollection[j];
			int voxelId = voxel.fId;
			TMatrixD W = TMatrixD(2, 2);
			W(0, 0) = voxel.fWx;
			W(1, 1) = voxel.fWy;
			W(0, 1) = voxel.fWxy;
			W(1, 0) = W(0, 1);

			//cout<<"== W"<<j<<" =="<<endl;
			//W.Print();

			scatteringMatrixSum += par[voxelId] * W;

		}		//end loop voxels in this event
*/
		for (unsigned int j = 0; j < sLTVectorForEachMuon.size(); j++) {
			//VoxelData voxel = fVoxelDataCollection[j];
			int voxelId = sLTVectorForEachMuon[j].sVoxelNum ; //voxel.fId;
			TMatrixD W = TMatrixD(2, 2);
			W(0, 0) = sLTVectorForEachMuon[j].sWx; //voxel.fWx;
			W(1, 1) = sLTVectorForEachMuon[j].sWy; //voxel.fWy;
			W(0, 1) = sLTVectorForEachMuon[j].sWxy; //voxel.fWxy;
			W(1, 0) = W(0, 1);

			//cout<<"== W"<<j<<" =="<<endl;
			//W.Print();

			//scatteringMatrixSum += s.sLambda[voxelId]*W;// par[voxelId] * W;
			scatteringMatrixSum += s->GetLambda()[voxelId]*W;

		}		//end loop voxels in this event



		TMatrixD C = TMatrixD(2, 2);
		//C = E + fPr * fPr * scatteringMatrixSum;
		C = E + sScatteringData.sPr * sScatteringData.sPr * scatteringMatrixSum;

		//## Store covariance matrix
		sCx = C(0, 0); //fCx = C(0, 0);
		sCy = C(1, 1); //fCy = C(1, 1);
		sCxy = C(0, 1); //fCxy = C(0, 1);


		//cout<<"fPr="<<fPr<<" C=("<<fCx<<","<<fCxy<<","<<fCxy<<","<<fCy<<")"<<endl;
	}

	void Print(){
		Tomography::ScatteringDensityV2 *s = Tomography::ScatteringDensityV2::instance();
		for(unsigned int i =0 ; i < sLTVectorForEachMuon.size() ; i++){
			sLTVectorForEachMuon[i].Print();
		}
		sScatteringData.Print();
		std::cout <<" ============= Covariance Matrix ==============" << std::endl;
		std::cout <<"Cx : " << sCx <<" : Cy : " << sCy << " : Cxy : " << sCxy << std::endl;
		std::cout << "========= New Lambda calculated from Muon ========= " << std::endl;
		std::cout << "NewLambdaSize : " << sNewLambdaFromMuon.size() << "  : LTVector for Each Muon Size : " << sLTVectorForEachMuon.size() << std::endl;
		for(int i = 0 ; i < sNewLambdaFromMuon.size() ; i++){
			std::cout << "Voxel Num : " << sLTVectorForEachMuon[i].sVoxelNum << " :: Calculated Lambda : " << sNewLambdaFromMuon[i]
					  << " :: Old Lambda Value : " << s->GetLambda()[sLTVectorForEachMuon[i].sVoxelNum] <<  std::endl;
		}
		std::cout <<"\n ======================================================" << std::endl;
	}



};

/*
struct SummationDInv{

};
*/


struct VoxelData{
	double sLambda;
	int count;
	VoxelData():sLambda(0.),count(0){}
};

class LambdaUpdater {
	int fTotalNumOfVoxels;
	std::vector<VoxelData> fVectorOfVoxelData;
	std::vector<Tomography::EachMuonData> fVectorOfMuonData;

public:
	LambdaUpdater();
	LambdaUpdater(std::vector<Tomography::EachMuonData> vectorOfMuonData);
	void CalculateNewLambda();
	virtual ~LambdaUpdater();
	void Print();
	std::vector<VoxelData> GetVectorOfVoxelData() const {return fVectorOfVoxelData;}
};

struct MuonTrack{
	Tomography::Track sIncoming;
	Tomography::Track sOutgoing;
	double sPr;
	MuonTrack(Tomography::Track incoming, Tomography::Track outgoing):sIncoming(incoming), sOutgoing(outgoing){}
	MuonTrack(Tomography::Track incoming, Tomography::Track outgoing, double pr):sIncoming(incoming), sOutgoing(outgoing),sPr(pr){}
};



class MLEM {
	int fNumOfTracks;
	int fNumOfVoxels;
	std::string fTracksFileName;
	std::vector<ScatteringData> fScatteringDataVector;

	//temporary vector, must be cleared one fVectorOfLtVector is filled
	std::vector<LTOfEachVoxel> fLTVectorForOneTrack;

	//THE most important vector for ImageReco
	std::vector<std::vector<LTOfEachVoxel>> fVectorOfLTVector;

	std::vector<MuonTrack> fVectorOfMuonTrack;

	//std::vector<SummationDInv> fVectorOfSummationDInv;
	std::vector<EachMuonData> fVectorOfMuonData;

	//May be NOT required
	//std::vector<VoxelNumAndWeightedMatrix> fVectorOfVoxelNumAndWeightedMatrixForOneMuon;
	//std::vector< std::vector<VoxelNumAndWeightedMatrix> > fVectorOfVoxelNumAndWeightedMatrixForAllMuons;

public:
	MLEM();
	MLEM(std::string tracksFileName);
	virtual ~MLEM();
	void DoExpectation();
	void DoMaximization();
	ScatteringData SetScatteringData(Tomography::Track trackIncoming,Tomography::Track trackOutgoing, double pr = 1.);
	void VoxelFinder(Tomography::Track trackIncoming,Tomography::Track trackOutgoing, double pr = 1.);
	void CreateWeightedMatrix();
	void EMUpdate();
	void UpdateScatteringDensity(LambdaUpdater  lambdaUpdater);


	void Print();
	void Print2();

	//Some debugging function to understand various steps in MLEm
	//May be deleted later
	void Debugger();

	//Function to get VectorofLTVector
	std::vector<std::vector<LTOfEachVoxel>> GetVectorOfLTVector() const {return fVectorOfLTVector;}
};

} /* namespace Tomography */

#endif /* HELPERS_INC_MLEM_H_ */
