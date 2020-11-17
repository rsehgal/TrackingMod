#include "PairFinder.h"
#include "Analyzer.h"
#include "Calibration.h"
#include <TApplication.h>
#include <TCanvas.h>
#include "Plotter.h"
#include "includes.hh"
#include "HardwareNomenclature.h"

int main(int argc, char *argv[]){
	GenerateScintMatrixXYCenters();
	TApplication *fApp = new TApplication("Test", NULL, NULL);
	//lite_interface::PairFinder pf("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_09hrs34mins_02Sep2020_0.root");
	ushort barIndex = 5;
	lite_interface::Calibration *calib = lite_interface::Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");
	//Calibration *calib = Calibration::instance("/home/rsehgal/BackBoneSoftwares/ismranData/completeCalib.root");

	/*
	//Graph used to get the fitted curve, done in the calibration
	TGraph *gr = lite_interface::Calibration::instance()->GetCalibrationDataOf(barIndex)->fGraphWithMuonPoint;
	new TCanvas();
	gr->SetMarkerStyle(8);
	gr->Draw();
	*/
	lite_interface::Analyzer pf("/home/rsehgal/BackBoneSoftwares/ismranData/ISMRAN_81bars_Th10All_CosmicRun_09hrs34mins_02Sep2020_0.root",argv[1]);
	//lite_interface::Analyzer pf("simulatedData.root",argv[1]);

	Double_t muonPeakPosition = lite_interface::Calibration::instance()->GetCalibrationDataOf(barIndex)->fEnergyCalibrationFactor;
	//Double_t muonPeakPosition = Calibration::instance()->GetCalibrationDataOf(barIndex)->fEnergyCalibrationFactor;

	std::cout << "Peak of Muon Bump is at : " << muonPeakPosition << std::endl;
	TF1 *enercalibFormula = lite_interface::Calibration::instance()->GetCalibrationDataOf(barIndex)->fEnergyCalibration_F;
	//TF1 *enercalibFormula = Calibration::instance()->GetCalibrationDataOf(barIndex)->fEnergyCalibration_F;

	Double_t ener = (enercalibFormula->Eval(muonPeakPosition)) ;
	std::cout << "Energy at Peak Position : " << ener << std::endl;

	/*
	//Manually plotting Uncorrected Qmean
	new TCanvas();
	PlotQ(pf.GetVectorOfScintillators(),barIndex)->Draw();
	*/
#if(1)
	/* Now lets see some plots */
	new TCanvas();
	TH1F *histDelT = PlotDelT(pf.GetVectorOfScintillators(),barIndex);
	histDelT->Draw();
	TH1F *histDelTCorrected = PlotDelTCorrected(pf.GetVectorOfScintillators(),barIndex);
	histDelTCorrected->SetLineColor(2);
	histDelTCorrected->Draw("same");

	(new TCanvas())->SetLogy();
	TH1F *histQmean = PlotQMean(pf.GetVectorOfScintillators(),barIndex);
	histQmean->SetLineColor(kBlack);
	TH1F *histQnear = PlotQNear(pf.GetVectorOfScintillators(),barIndex);
	histQnear->SetLineColor(kBlue);
	TH1F *histQfar = PlotQFar(pf.GetVectorOfScintillators(),barIndex);
	histQfar->SetLineColor(kMagenta);
	TH1F *histQmeanCorrected = PlotQMeanCorrected(pf.GetVectorOfScintillators(),barIndex);
	histQmeanCorrected->SetLineColor(kGreen);

	histQmean->Draw();
	histQnear->Draw("same");
	histQfar->Draw("same");
	histQmeanCorrected->Draw("same");
#endif
	fApp->Run();
	return 0;
}
