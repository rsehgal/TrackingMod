/*
 * SingleMuonTrack.cpp
 *
 *  Created on: 07-Sep-2020
 *      Author: rsehgal
 */

#include "SingleMuonTrack.h"
#include "ScintillatorBar_V2.h"
#include "SkimmedTracks.h"
#include "HelperFunctions.h"
#include "FittedTracks.h"

unsigned long int SingleMuonTrack::fMuonTrackNum = 0;

/*
double LinearFit(Double_t *x,Double_t *par) {

      double fitval = par[0] + par[1]*x[0];
      return fitval;
}
*/

void SingleMuonTrack::Reset(){
	ResetVector<ScintillatorBar_V2>(fSingleMuonTrack);
}

SingleMuonTrack::SingleMuonTrack() {
	// TODO Auto-generated constructor stub
	fTotalEnergyDeposited = 0.;
	fIsValid = false;

}

SingleMuonTrack::SingleMuonTrack(std::vector<ScintillatorBar_V2*> singleMuonTrack){
	fSingleMuonTrack = singleMuonTrack;
}

SingleMuonTrack::~SingleMuonTrack() {
	// TODO Auto-generated destructor stub
	if(fFittedTrack != NULL)
		delete fFittedTrack;
}

void SingleMuonTrack::push_back(ScintillatorBar_V2 *hittedBar){
	fSingleMuonTrack.push_back(hittedBar);
}

void SingleMuonTrack::CreateFittedMuonTrack(unsigned short int trackType){

	std::vector<double> xvec, yvec, zvec;
	std::vector<double> xvecErr, yvecErr, zvecErr;
	for(unsigned int i = 0 ; i < fSingleMuonTrack.size() ; i++){
		/* ------- Parameterized -------*/
		if(trackType == 0){
			xvec.push_back((fSingleMuonTrack[i]->fhitPositionParam).fHitPosition.x);
			yvec.push_back((fSingleMuonTrack[i]->fhitPositionParam).fHitPosition.y);
			zvec.push_back((fSingleMuonTrack[i]->fhitPositionParam).fHitPosition.z);
			xvecErr.push_back((fSingleMuonTrack[i]->fhitPositionParam).fHitPositionError.x);
			yvecErr.push_back((fSingleMuonTrack[i]->fhitPositionParam).fHitPositionError.y);
			zvecErr.push_back((fSingleMuonTrack[i]->fhitPositionParam).fHitPositionError.z);
		}else{
			/*------ SOL ------*/
			if(trackType==1){
				xvec.push_back((fSingleMuonTrack[i]->fhitPostionSOL).fHitPosition.x);
				yvec.push_back((fSingleMuonTrack[i]->fhitPostionSOL).fHitPosition.y);
				zvec.push_back((fSingleMuonTrack[i]->fhitPostionSOL).fHitPosition.z);
				xvecErr.push_back((fSingleMuonTrack[i]->fhitPostionSOL).fHitPositionError.x);
				yvecErr.push_back((fSingleMuonTrack[i]->fhitPostionSOL).fHitPositionError.y);
				zvecErr.push_back((fSingleMuonTrack[i]->fhitPostionSOL).fHitPositionError.z);
			}
			else{

			}
		}
	}
	fFittedTrack = new FittedTracks(xvec,yvec,zvec,xvecErr,yvecErr,zvecErr);
}



void SingleMuonTrack::CalculateTotalEnergyDeposited(){
	fTotalEnergyDeposited = 0.;
	for(unsigned int i = 0 ; i < fSingleMuonTrack.size() ; i++){
		//fTotalEnergyDeposited += fSingleMuonTrack[i]->qlongMean;
		fTotalEnergyDeposited += fSingleMuonTrack[i]->qlongMeanCorrected;
		if(i < (fSingleMuonTrack.size()-1))
			fSingleMuonTrack[i]->pathLength = fSingleMuonTrack[i]->hitPosition.Distance(fSingleMuonTrack[i+1]->hitPosition);
		else
			fSingleMuonTrack[i]->pathLength = 0.;
	}
	//std::cout << "*********** Total Energy Deposited : " << fTotalEnergyDeposited << " **************" << std::endl;
	if(fTotalEnergyDeposited > minEnergyDepositedByMuonTrack && fTotalEnergyDeposited < maxEnergyDepositedByMuonTrack){
		fIsValid = true;
	}
}

bool SingleMuonTrack::CompareBarIndexInScintillator(ScintillatorBar_V2 *i1, ScintillatorBar_V2 *i2)
{
	return (i1->barIndex > i2->barIndex);
}

void SingleMuonTrack::Sort(){
	std::sort(fSingleMuonTrack.begin(), fSingleMuonTrack.end(),CompareBarIndexInScintillator);
}

void SingleMuonTrack::clear(){
	fSingleMuonTrack.clear();
}

unsigned long int SingleMuonTrack::size(){
	return fSingleMuonTrack.size();
}

void SingleMuonTrack::Print(){
	for(unsigned int j = 0 ; j < fSingleMuonTrack.size() ; j++){
		fSingleMuonTrack[j]->Print();
	}
	std::cout << "@@@@@@@@@2 Total Energy Deposited : " << fTotalEnergyDeposited << " @@@@@@@@@" <<  std::endl;
}


std::vector<double> SingleMuonTrack::GetFittedXorZ(TGraph *gr){
	TF1 *formula = new TF1("Formula",LinearFit,-45,45,2);
	gr->Fit(formula,"qn");
	double c = formula->GetParameter(0);
	double m = formula->GetParameter(1);
	delete formula;

	std::vector<double> vecOfEstimatedXorZ;
	for(unsigned int i = 0 ; i < fSingleMuonTrack.size() ; i++){
		vecOfEstimatedXorZ.push_back( ((fSingleMuonTrack[i]->meanHitPosition).y - c)/m );
	}
	return vecOfEstimatedXorZ;
}

std::vector<double> SingleMuonTrack::GetFittedXorZ(TGraphErrors *gr){
	TF1 *formula = new TF1("Formula",LinearFit,-45,45,2);
	gr->Fit(formula,"qn");
	double c = formula->GetParameter(0);
	double m = formula->GetParameter(1);
	delete formula;

	std::vector<double> vecOfEstimatedXorZ;
	for(unsigned int i = 0 ; i < fSingleMuonTrack.size() ; i++){
		vecOfEstimatedXorZ.push_back( ((fSingleMuonTrack[i]->hitPosition).y - c)/m );
	}
	return vecOfEstimatedXorZ;
}
/*
std::vector<double> SingleMuonTrack::GetFittedZ(TGraph *grzy){
	TF1 *formulaZY = new TF1("FormulaZY",LinearFit,-45,45,2);
	grzy->Fit(formulaZY,"r");
	double c1 = formulaZY->GetParameter(0);
	double m1 = formulaZY->GetParameter(1);

}
*/

ULong64_t SingleMuonTrack::GetMeanTimeStampValue(){
	ULong64_t sum = 0;
	for(unsigned short int i =0 ; i < fSingleMuonTrack.size() ; i++){
		sum += (fSingleMuonTrack[i]->tsmallTimeStamp/1000.); //Addition done in nanosecond, NOT in picosecond
	}

	return (sum/fSingleMuonTrack.size());
}
void SingleMuonTrack::FillSkimmedMuonTracksVector(){
	std::vector<Double_t> xvec, yvec, zvec;
	std::vector<Double_t> xvecErr, yvecErr, zvecErr;
	std::vector<Point3D*> rawMuonTrack;
	std::vector<Point3D*> fittedMuonTrack;
	for (unsigned int index = 0; index < fSingleMuonTrack.size(); index++) {
			xvec.push_back((fSingleMuonTrack[index]->hitPosition).x);
			yvec.push_back((fSingleMuonTrack[index]->hitPosition).y);
			zvec.push_back((fSingleMuonTrack[index]->hitPosition).z);
			xvecErr.push_back((fSingleMuonTrack[index]->hitPositionError).x);
			yvecErr.push_back((fSingleMuonTrack[index]->hitPositionError).y);
			zvecErr.push_back((fSingleMuonTrack[index]->hitPositionError).z);
			rawMuonTrack.push_back(new Point3D(xvec[index],yvec[index],zvec[index]));
	}

	//Fitting in XY plane
	TGraphErrors *grxy = new TGraphErrors(xvec.size(), &xvec[0], &yvec[0],&xvecErr[0],&yvecErr[0]);
	std::vector<double> fittedX = GetFittedXorZ(grxy);
	//Fitting in ZY plane
	TGraphErrors *grzy = new TGraphErrors(zvec.size(), &zvec[0], &yvec[0], &zvecErr[0], &yvecErr[0]);
	std::vector<double> fittedZ = GetFittedXorZ(grzy);
	for (unsigned int i = 0; i < fSingleMuonTrack.size(); i++) {
		fittedMuonTrack.push_back(new Point3D(fittedX[i],yvec[i],fittedZ[i]));
	}
	vecOfSkimmedMuonTracks.push_back(new SkimmedMuonTrack(rawMuonTrack,fittedMuonTrack,fTotalEnergyDeposited,GetMeanTimeStampValue(),fMuonTrackNum));

	delete grxy;
	delete grzy;
	rawMuonTrack.clear();
	fittedMuonTrack.clear();
	fMuonTrackNum++;
}



std::vector<Point3D*> SingleMuonTrack::GetFittedMeanHitPointTrack(){

	std::vector<Double_t> xvec, yvec, zvec;
	std::vector<Point3D*> fittedMuonTrack;

	for (unsigned int index = 0; index < fSingleMuonTrack.size(); index++) {
		xvec.push_back((fSingleMuonTrack[index]->meanHitPosition).x);
		yvec.push_back((fSingleMuonTrack[index]->meanHitPosition).y);
		zvec.push_back((fSingleMuonTrack[index]->meanHitPosition).z);
	}

	TGraph *grxy = new TGraph(xvec.size(), &xvec[0], &yvec[0]);
	std::vector<double> fittedX = GetFittedXorZ(grxy);
	TGraph *grzy = new TGraph(zvec.size(), &zvec[0], &yvec[0]);
	std::vector<double> fittedZ = GetFittedXorZ(grzy);

	for (unsigned int i = 0; i < fSingleMuonTrack.size(); i++) {
		fSingleMuonTrackFitted.push_back(new Point3D(fittedX[i],yvec[i],fittedZ[i]));
	}

	return fSingleMuonTrackFitted;
}

#if(0)
std::vector<Point3D*> SingleMuonTrack::GetFittedHitPointTrackUsingParameterization(){

	std::vector<Double_t> xvec, yvec, zvec;
	std::vector<Point3D*> fittedMuonTrack;

	for (unsigned int index = 0; index < fSingleMuonTrack.size(); index++) {
		xvec.push_back((fSingleMuonTrack[index]->hitPositionParam).x);
		yvec.push_back((fSingleMuonTrack[index]->hitPositionParam).y);
		zvec.push_back((fSingleMuonTrack[index]->hitPositionParam).z);
	}

	TGraph *grxy = new TGraph(xvec.size(), &xvec[0], &yvec[0]);
	std::vector<double> fittedX = GetFittedXorZ(grxy);
	TGraph *grzy = new TGraph(zvec.size(), &zvec[0], &yvec[0]);
	std::vector<double> fittedZ = GetFittedXorZ(grzy);

	for (unsigned int i = 0; i < fSingleMuonTrack.size(); i++) {
		fSingleMuonTrackFitted.push_back(new Point3D(fittedX[i],yvec[i],fittedZ[i]));
	}

	return fSingleMuonTrackFitted;
}
#endif

std::vector<Point3D*> SingleMuonTrack::PlotTrack(bool showTracks){

	//TMultiGraph *mg = new TMultiGraph();
	//std::cout << "SHOW TRACKS from PLOTTRACK: " << showTracks << " : " << __FILE__ << " : " << __LINE__ << std::endl;
	std::vector<Double_t> xvec, yvec, zvec;
	std::vector<Double_t> xvecErr, yvecErr, zvecErr;
	std::vector<Point3D*> rawMuonTrack;
	std::vector<Point3D*> fittedMuonTrack;

	for (unsigned int index = 0; index < fSingleMuonTrack.size(); index++) {
		//if((singleMuonTrack[index]->hitPosition).x <= 100 && (singleMuonTrack[index]->hitPosition).y <=50){
		xvec.push_back((fSingleMuonTrack[index]->hitPosition).x);
		yvec.push_back((fSingleMuonTrack[index]->hitPosition).y);
		zvec.push_back((fSingleMuonTrack[index]->hitPosition).z);
		xvecErr.push_back((fSingleMuonTrack[index]->hitPositionError).x);
		yvecErr.push_back((fSingleMuonTrack[index]->hitPositionError).y);
		zvecErr.push_back((fSingleMuonTrack[index]->hitPositionError).z);
		rawMuonTrack.push_back(new Point3D(xvec[index],yvec[index],zvec[index]));
		//}
	}
	std::string trackname = "MuonTrack-" + std::to_string(fMuonTrackNum);
	//std::cout << "TrackName : " << trackname << " : Size : "<< fSingleMuonTrack.size() << std::endl;
	/*for(unsigned int i = 0 ; i< singleMuonTrack.size() ; i++){
	 (singleMuonTrack[i]->hitPosition).Print();
	 }*/


	/*TCanvas *c = new TCanvas(trackname.c_str(), trackname.c_str(), 800, 800);
	c->Divide(2,2);
	//TGraph *gr = new TGraph(xvec.size(), &zvec[0], &xvec[0]);
	c->cd(1);
	*/
	TGraphErrors *grxy = new TGraphErrors(xvec.size(), &xvec[0], &yvec[0],&xvecErr[0],&yvecErr[0]);
	grxy->SetMarkerStyle(8);
	grxy->SetMarkerSize(0.6);
	grxy->SetTitle("Muon Track in XY plane; X axis ; Y axis");
	/*DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
	grxy->Draw("p");*/
	//mg->Add(grxy);

	//c->cd(2);
	std::vector<double> fittedX = GetFittedXorZ(grxy);
	TGraph *grxFitted = new TGraph(fittedX.size(), &fittedX[0], &yvec[0]);
	grxFitted->SetMarkerStyle(8);
	grxFitted->SetMarkerSize(0.6);
	grxFitted->SetTitle("Fitted Muon Track in XY plane; X axis ; Y axis");
	grxFitted->SetMarkerColor(2);
	/*DrawGrid("Fitted Muon Track in XY plane; X axis ; Y axis", 9, 9);
	grxFitted->Draw("p");*/


	//c->cd(3);
	TGraphErrors *grzy = new TGraphErrors(zvec.size(), &zvec[0], &yvec[0], &zvecErr[0], &yvecErr[0]);
	grzy->SetMarkerStyle(8);
	grzy->SetMarkerSize(0.6);
	grzy->SetTitle("Muon Track in ZY plane; Z axis ; Y axis");
	/*DrawGrid("Muon Track in ZY plane;Z axis ; Y axis", 9, 9);
	grzy->Draw("p");*/

	//c->cd(4);
	std::vector<double> fittedZ = GetFittedXorZ(grzy);
	TGraph *grzFitted = new TGraph(fittedZ.size(), &fittedZ[0], &yvec[0]);
	grzFitted->SetMarkerStyle(8);
	grzFitted->SetMarkerSize(0.6);
	grzFitted->SetTitle("Fitted Muon Track in ZY plane; Z axis ; Y axis");
	grzFitted->SetMarkerColor(2);
	/*DrawGrid("Fitted Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
	grzFitted->Draw("p");*/

	//std::cout << "SHOW TRACKS : " << showTracks << std::endl;
	if(showTracks){
		TCanvas *c = new TCanvas(trackname.c_str(), trackname.c_str(), 800, 800);
		c->Divide(2,2);
		c->cd(1);
		DrawGrid("Muon Track in XY plane; X axis ; Y axis", 9, 9);
		grxy->Draw("p");

		c->cd(2);
		DrawGrid("Fitted Muon Track in XY plane; X axis ; Y axis", 9, 9);
		grxFitted->Draw("p");

		c->cd(3);
		DrawGrid("Muon Track in ZY plane;Z axis ; Y axis", 9, 9);
		grzy->Draw("p");

		c->cd(4);
		DrawGrid("Fitted Muon Track in ZY plane; Z axis ; Y axis", 9, 9);
		grzFitted->Draw("p");
	}

	for (unsigned int i = 0; i < fSingleMuonTrack.size(); i++) {
		fSingleMuonTrackFitted.push_back(new Point3D(fittedX[i],yvec[i],fittedZ[i]));
	}

	/*c->cd(3);
	TGraph *grzx = new TGraph(zvec.size(), &zvec[0], &xvec[0]);
	grzx->SetMarkerStyle(8);
	grzx->SetTitle("Muon Track in ZX plane; Z axis; X axis");
	DrawGrid("Muon Track in ZX plane; Z axis; X axis", 9, 9);
	grzx->Draw("p");*/

	fMuonTrackNum++;
	return fSingleMuonTrackFitted;
}

/*
 * Function to Draw customized Grid Lines
 */
#if(0)
void SingleMuonTrack::DrawGrid(std::string t, Int_t ngx, Int_t ngy)
{
//	std::cout << "DrawGrid Called........: " << t << " : ....." << std::endl;
   //new TCanvas();
   Double_t x1 = -45;
   Double_t x2 = 45;
   Double_t y1 = -45;
   Double_t y2 = 45;
   Double_t xs = (x2-x1)/ngx;
   Double_t ys = (y2-y1)/ngy;
   Int_t i;

   TH1F *h = gPad->DrawFrame(x1, y1, x2, y2);
   h->GetXaxis()->SetNdivisions(20);
   h->GetYaxis()->SetNdivisions(20);
   h->GetYaxis()->SetTickLength(0.);
   h->GetXaxis()->SetTickLength(0.);
   h->GetXaxis()->SetLabelSize(0.025);
   h->GetYaxis()->SetLabelSize(0.025);
   h->SetTitle(t.c_str());
   gPad-> Update();

   TLine l;
   l.SetLineColor(kGray);
   Double_t x = x1+xs;
   for (i = 0; i<ngx-1; i++) {
      l.DrawLine(x,y1,x,y2);
      x = x + xs;
   }

   Double_t y = y1+xs;
   for (i = 0; i<ngy-1; i++) {
      l.DrawLine(x1,y,x2,y);
      y = y +ys;
   }
}
#endif
