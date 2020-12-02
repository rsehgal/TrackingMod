#include "Plotter.h"
#include <iterator>
#include "HardwareNomenclature.h"
#include "HelperFunctions.h"
#include "Analyzer.h"
#include "ScintillatorBar_V2.h"
#include "SingleMuonTrack.h"

namespace lite_interface{

	TH1F* PlotQ(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec, ushort barIndex){
		TH1F *hist = new TH1F("RawHist","RawHist",40000,0,40000);
		std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
		for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
			if((*itr)->fBarIndex == barIndex){
				hist->Fill((*itr)->GetQLongMean());
			}
		}
		return hist;
	}

	TH1F* PlotQ_0123(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec, ushort barIndex, ushort opt){
		TH1F *hist;
		unsigned int nbins = 1000;
		unsigned int start = 0;
		unsigned int end = 40;
		if(opt == 0){
			hist = new TH1F("HistQMean","HistQMean",nbins,start,end);
			std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				if(IsSimulation){
				if(barIndex==65535){
					hist->Fill((*itr)->GetQLongMean());
				}else{
					if((*itr)->fBarIndex == barIndex){
						hist->Fill((*itr)->GetQLongMean());
					}
				}
				}else{
					if(barIndex==65535){
						hist->Fill((*itr)->GetQLongMean()/1000.);
					}else{
						if((*itr)->fBarIndex == barIndex){
							hist->Fill((*itr)->GetQLongMean()/1000.);
						}
					}
					}
			}	
		}

		if(opt == 1){
			hist = new TH1F("HistQNear","HistQNear",nbins,start,end);
			std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				if(barIndex==65535){
						hist->Fill((*itr)->GetQLongNear()/1000.);
					}else{
						if((*itr)->fBarIndex == barIndex){
						hist->Fill((*itr)->GetQLongNear()/1000.);
					}
				}
			}
		}

		if(opt == 2){
			hist = new TH1F("HistQFar","HistQFar",nbins,start,end);
			std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				if(barIndex==65535){
					hist->Fill((*itr)->GetQLongFar()/1000.);
				}else{
					if((*itr)->fBarIndex == barIndex){
						hist->Fill((*itr)->GetQLongFar()/1000.);
					}
				}
			}
		}
		if(opt == 3){
			hist = new TH1F("HistQMeanCorrected","HistQMeanCorrected",nbins,start,end);
			std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				if(barIndex==65535){
					//std::cout << (*itr)->GetBarIndex() << std::endl;
					hist->Fill((*itr)->GetQMeanCorrected());
				}else{
					if((*itr)->fBarIndex == barIndex){
						hist->Fill((*itr)->GetQMeanCorrected());
					}
				}
			}
		}
		hist->Scale(1/hist->Integral());
		return hist;
	}

	TH1F* PlotQMeanCorrected_V2(std::vector<ScintillatorBar_V2> scintBarVec,ushort barIndex){
		unsigned int nbins = 1000;
				unsigned int start = 0;
				unsigned int end = 40;
		TH1F *hist = new TH1F("HistQMeanCorrected","HistQMeanCorrected",nbins,start,end);
					std::vector<ScintillatorBar_V2>::iterator itr;
					for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
						if(barIndex==65535){
							//std::cout << (*itr).GetBarIndex() << std::endl;
							hist->Fill((*itr).GetQMeanCorrected());
						}else{
							if((*itr).fBarIndex == barIndex){
								hist->Fill((*itr).GetQMeanCorrected());
							}
						}
					}
		return hist;
	}

	TH1F* PlotQMeanCorrected(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec,ushort barIndex){
		return PlotQ_0123(scintBarVec,barIndex,3);
	}

	TH1F* PlotQMean(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec,ushort barIndex){
		return PlotQ_0123(scintBarVec,barIndex,0);
	}
	
	TH1F* PlotQNear(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec,ushort barIndex){
		return PlotQ_0123(scintBarVec,barIndex,1);
	}

	TH1F* PlotQFar(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec,ushort barIndex){
		return PlotQ_0123(scintBarVec,barIndex,2);
	}
	TH1F* PlotDelT(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec, ushort barIndex){
		TH1F *hist = new TH1F("HistDelT","HistDelT",200,-25,25); //Histogram with entries in nanosecond
		std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
		for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
			if((*itr)->fBarIndex == barIndex){
				//std::cout << "BarIndex : " << (*itr)->fBarIndex << " : DelT : " << (*itr)->GetDelT()/1000. << std::endl;
				if(IsSimulation)
					hist->Fill((*itr)->GetDelT()/1000.);
				else
					hist->Fill((*itr)->GetDelT()/1000.);
			}
		}
		return hist;
	}
	TH1F* PlotDelTCorrected(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec, ushort barIndex){
		std::string barName = vecOfBarsNamess[barIndex];
		TH1F *hist = new TH1F(barName.c_str(),barName.c_str(),200,-25,25); //Histogram with entries in nanosecond
		std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
		for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
			if((*itr)->fBarIndex == barIndex){
				if(IsSimulation)
					hist->Fill((*itr)->GetDelTCorrected()/1000.);
				else
					hist->Fill((*itr)->GetDelTCorrected()/1000.);
			}
		}
		return hist;
	}

	TH1F* PlotBarsProfile(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec) {
		//std::string barName = vecOfBarsNamess[barIndex];
		TH1F *hist = new TH1F("Bars Profile", "Bars Profile", numOfLayers*numOfBarsInEachLayer, 0, numOfLayers*numOfBarsInEachLayer); //Histogram with entries in nanosecond
		std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
		for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {

				hist->Fill((*itr)->GetBarIndex());
		}
		return hist;
	}

	TH2F* PlotHitPointsOnBar(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec, ushort barIndex){
		ushort nbinsx = 9;
		ushort nbinsz = 10;
		std::string name = "Hit points on "+vecOfBarsNamess[barIndex];
		TH2F *hist = new TH2F(name.c_str(),name.c_str(),nbinsx,-45,45,nbinsz,-50,50);
		std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
		for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
			if((*itr)->fBarIndex == barIndex){
				lite_interface::Point3D *hitPt = (*itr)->EstimateHitPosition();
				//hitPt->Print();
				hist->Fill(hitPt->GetX(),hitPt->GetZ());
			}
		}
		return hist;
	}
//#ifdef USE_FOR_SIMULATION
	TGraph* PlotDelTvsZ(std::vector<lite_interface::ScintillatorBar_V2*> scintBarVec, ushort barIndex, bool linear){
		ushort nbinsx = 9;
		ushort nbinsz = 10;
		std::string name = "DelT Vs Z : "+vecOfBarsNamess[barIndex];

		std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
		std::vector<double> zVec;
		std::vector<double> delTVec;
		for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
			if((*itr)->fBarIndex == barIndex){
#ifdef USE_FOR_SIMULATION
				zVec.push_back((*itr)->hitZ);
#else
				if(linear)
					zVec.push_back((*itr)->EstimateHitPosition()->GetZ());
				else
					zVec.push_back((*itr)->EstimateHitPosition_Param()->GetZ());
#endif
				delTVec.push_back((*itr)->fDelTstamp/1000. );

			}
		}
		TGraph *delTvsZ = new TGraph(delTVec.size(),&delTVec[0],&zVec[0]);
		return delTvsZ;
	}
//#endif

	TGraphErrors* PlotMuonTrack(lite_interface::SingleMuonTrack *smt,int opt){
		return PlotMuonTrack(smt->Get3DHitPointVector(),opt);
	}

	TGraphErrors* PlotMuonTrack(std::vector<lite_interface::Point3D*> vecOfPoint3D, int opt){
		std::vector<Double_t> xVec, yVec, zVec;
		std::vector<Double_t> xVecErr, yVecErr, zVecErr;
		std::vector<lite_interface::Point3D*>::iterator itr;
		for(itr = vecOfPoint3D.begin() ; itr!=vecOfPoint3D.end() ; itr++){
			//std::cout <<"=================" << std::endl;
			//(*itr)->Print();
			xVec.push_back((*itr)->GetX());
			xVecErr.push_back(5);
			yVec.push_back((*itr)->GetY());
			yVecErr.push_back(5.);
			zVec.push_back((*itr)->GetZ());
			zVecErr.push_back(5.);
		}
		/*TGraphErrors *grxy = new TGraphErrors(xVec.size(), &xVec[0], &yVec[0],&xVecErr[0],&yVecErr[0]);
		grxy->SetMarkerStyle(8);
		grxy->SetMarkerColor(kMagenta);
		TGraphErrors *grzy = new TGraphErrors(zVec.size(), &zVec[0], &yVec[0], &zVecErr[0], &yVecErr[0]);
		grzy->SetMarkerStyle(8);
		grzy->SetMarkerColor(kBlue);
		*/
		if(opt==1){
			TGraphErrors *grxy = PlotMuonTrack(xVec,yVec);
			grxy->SetMarkerColor(kMagenta);
			return grxy;
		}
		if(opt==2){
			TGraphErrors *grzy = PlotMuonTrack(zVec,yVec);
			grzy->SetMarkerColor(kBlue);
			return grzy;
		}
	}

	TGraphErrors* PlotMuonTrack(std::vector<Double_t> xVec, std::vector<Double_t> yVec){
		std::vector<Double_t> xVecErr, yVecErr;//, zVecErr;

		for(unsigned int i = 0 ; i < xVec.size() ; i++){
			xVecErr.push_back(5);
			yVecErr.push_back(5.);
			//zVecErr.push_back(5.);
		}

		TGraphErrors *grxy = new TGraphErrors(xVec.size(), &xVec[0], &yVec[0],&xVecErr[0],&yVecErr[0]);
		grxy->SetMarkerStyle(8);

		return grxy;

	}

	TGraphErrors* PlotMuonTrackXY(lite_interface::SingleMuonTrack *smt){
		return PlotMuonTrack(smt,1);
	}

	TGraphErrors* PlotMuonTrackZY(lite_interface::SingleMuonTrack *smt){
		return PlotMuonTrack(smt,2);
	}

	TGraphErrors* PlotMuonTrackXY(std::vector<lite_interface::Point3D*> vecOfPoint3D){
		return PlotMuonTrack(vecOfPoint3D,1);
	}

	TGraphErrors* PlotMuonTrackZY(std::vector<lite_interface::Point3D*> vecOfPoint3D){
		return PlotMuonTrack(vecOfPoint3D,2);
	}

	TH1F* PlotEnergySum(std::vector<lite_interface::SingleMuonTrack*> smtVec){
		TH1F *energSumHist = new TH1F("EnergySumHist","Histogram of Energy Sum",1000,0,400);
		std::vector<lite_interface::SingleMuonTrack*>::iterator itr;
		int count=0;
		for(itr = smtVec.begin() ; itr!=smtVec.end() ; itr++){
			//std::cout << "@@@@@@@@@@@@@@@ EVENT : " << count++ << " @@@@@@@@@@@@@@@@@@@" << std::endl;
			//(*itr)->Print();
			energSumHist->Fill((*itr)->GetEnergySum());
		}
		//energSumHist->Scale(1/energSumHist->Integral());
		return energSumHist;
	}

	std::vector<TH1D*> PlotEnergyDistributionWithMultiplicity(std::vector<SingleMuonTrack*> muonTrackVec){

		std::vector<TH1D*> vecOfHists;
				    for(unsigned int i = 0 ; i < numOfLayers ; i++){
				    	std::cout << "Inserting histogram for layer : " << i << std::endl;
				    	std::string title = "layer-"+std::to_string(i);
				    	vecOfHists.push_back(new TH1D(title.c_str(),title.c_str(),500,10,250));
				    	vecOfHists[i]->SetLineColor(i+1);
				    }

				    std::cout << "Szie of MuonTrcke Vector : " << muonTrackVec.size()  << std::endl;

				    for(unsigned int i = 0 ; i < muonTrackVec.size() ; i ++){
				    	if(muonTrackVec[i]->size() > 0 && muonTrackVec[i]->size() <= numOfLayers){
				    		//std::cout << "Size : " << muonTrackVec[i]->size() <<" : EnergySum : " << muonTrackVec[i]->GetEnergySum() << std::endl;
				    		vecOfHists[muonTrackVec[i]->size()-1]->Fill(muonTrackVec[i]->GetEnergySum());
				    	}

				    }

				    for(unsigned int i = 0 ; i < vecOfHists.size() ; i++){
				    	vecOfHists[i]->Scale(1/vecOfHists[i]->Integral());
				    }
				    return vecOfHists;
	}
	//TCanvas* PlotEnergyDistributionWithMultiplicity(std::vector<SingleMuonTrack*> muonTrackVec, unsigned int multiplicity){
	 std::vector<TH1D*> PlotEnergyDistributionWithMultiplicity_Old(std::vector<SingleMuonTrack*> muonTrackVec, unsigned int multiplicity){

	std::vector<short int> multiplicityVec;
	if(multiplicity == 0){
		for(unsigned int i = 0 ; i < numOfLayers ; i++){
			multiplicityVec.push_back(i+1);
		}
	}else{
		multiplicityVec.push_back(multiplicity);
	}

	TCanvas *can = new TCanvas();
	if(multiplicityVec.size()!=1){
		float t = sqrt(multiplicityVec.size());
		unsigned short rows = std::floor(t);
		unsigned short cols = std::ceil(t);
		can->Divide(cols,rows);

	}

	if(multiplicityVec.size()==1){
		char *title = Form("Energy histogram for Multiplicity %d",multiplicity);
		TH1D *histEnergyWithMultiplicity = new TH1D(title,title,500,100000,250000);
		for(unsigned int i = 0 ; i < muonTrackVec.size() ; i ++){
			if(muonTrackVec[i]->size() == multiplicity){
				histEnergyWithMultiplicity->Fill(muonTrackVec[i]->GetEnergySum());
			}
		}
		histEnergyWithMultiplicity->Draw();
	}else{
		std::vector<TH1D*> vecOfHists;
		for(unsigned int i = 0 ; i < multiplicityVec.size() ; i++){
			char *str = Form("Multiplicity-%d",i+1);
			int xlow = multiplicityVec[i]*20000. - 60000;
			int xhigh = multiplicityVec[i]*20000. + 60000;
			xlow = 0;
			xhigh = 240000;
			vecOfHists.push_back(new TH1D(str,str,50,xlow,xhigh));
			vecOfHists[i]->GetXaxis()->SetTitle("Energy sum");
			vecOfHists[i]->GetXaxis()->CenterTitle(true);
			vecOfHists[i]->GetYaxis()->SetTitle("Normalized Counts");
			vecOfHists[i]->GetYaxis()->CenterTitle(true);
		}
		for(unsigned int i = 0 ; i < muonTrackVec.size() ; i ++){
			//std::cout << "BREAK at Muon Track vector of size :  " << (muonTrackVec[i]->fSingleMuonTrack).size() << std::endl;
			if(muonTrackVec[i]->size() <= numOfLayers && muonTrackVec[i]->size() > 0)
				vecOfHists[muonTrackVec[i]->size()-1]->Fill(muonTrackVec[i]->GetEnergySum() );
		}
		for(unsigned int i = 0 ; i < multiplicityVec.size() ; i++){
			can->cd(i+1);
			vecOfHists[i]->Draw();
		}

		TLegend *legend = new TLegend();//0.1,0.7,0.48,0.9);
	    legend->SetHeader("Multiplicity","C");
		new TCanvas();

		for(unsigned int i = 0 ; i < multiplicityVec.size() ; i++){
			vecOfHists[i]->SetLineColor(i+1);
			vecOfHists[i]->Scale(1/vecOfHists[i]->Integral());
			legend->AddEntry(vecOfHists[i],Form("Multiplicity_%d",i+1),"l");
			vecOfHists[i]->GetXaxis()->SetNdivisions(12);
			vecOfHists[i]->Draw("same");
		}
		legend->Draw();

		return vecOfHists;

	}

	//return can;


	//histEnergyWithMultiplicity->Draw();
}
	std::vector<lite_interface::Point3D*> CreateFittedTrack(std::vector<lite_interface::Point3D*> vecOfPoint3D){
		std::vector<Double_t> xVec, yVec, zVec;
		std::vector<Double_t> xVecErr, yVecErr, zVecErr;
		std::vector<lite_interface::Point3D*>::iterator itr;
		for (itr = vecOfPoint3D.begin(); itr != vecOfPoint3D.end(); itr++) {
			//std::cout <<"=================" << std::endl;
			//(*itr)->Print();
			xVec.push_back((*itr)->GetX());
			xVecErr.push_back(5.);
			yVec.push_back((*itr)->GetY());
			yVecErr.push_back(5.);
			zVec.push_back((*itr)->GetZ());
			zVecErr.push_back(5.);
		}
		//std::cout <<"================= Trying to create the Fitted Track from Class FittedTracks ==================== : " << __FILE__ << " : " << __LINE__ << std::endl;
		TGraphErrors *grxy = new TGraphErrors(xVec.size(), &xVec[0], &yVec[0],&xVecErr[0],&yVecErr[0]);
		//std::cout << "++++++++++++++ Fit XY ++++++++++++++++" << std::endl;
		std::vector<double> fittedX = GetFittedXorZ(grxy,vecOfPoint3D);
		TGraphErrors *grzy = new TGraphErrors(zVec.size(), &zVec[0], &yVec[0], &zVecErr[0], &yVecErr[0]);
		//std::cout << "++++++++++++++ Fit ZY ++++++++++++++++" << std::endl;
		std::vector<double> fittedZ = GetFittedXorZ(grzy,vecOfPoint3D);
		std::vector<Point3D*>fittedMuonTrack;
		for (unsigned int i = 0; i < xVec.size(); i++) {
			/*Point3D *pt = new Point3D(fittedX[i],yVec[i],fittedZ[i]);
			std::cout << "From CreateFitted Track : " << __FILE__ <<" : " << __LINE__ ; pt->Print();
			fittedMuonTrack.push_back(pt);*/
			fittedMuonTrack.push_back(new Point3D(fittedX[i],yVec[i],fittedZ[i]));

		}
		return fittedMuonTrack;
	}

	std::vector<double> GetFittedXorZ(TGraphErrors *gr,std::vector<lite_interface::Point3D*> vecOfPoint3D){
		std::vector<Double_t> xVec, yVec, zVec;
		std::vector<Double_t> xVecErr, yVecErr, zVecErr;
		std::vector<lite_interface::Point3D*>::iterator itr;
		for (itr = vecOfPoint3D.begin(); itr != vecOfPoint3D.end(); itr++) {
			//std::cout <<"=================" << std::endl;
			//(*itr)->Print();
			xVec.push_back((*itr)->GetX());
			xVecErr.push_back(5);
			yVec.push_back((*itr)->GetY());
			yVecErr.push_back(5.);
			zVec.push_back((*itr)->GetZ());
			zVecErr.push_back(5);
		}
		TF1 *formula = new TF1("Formula",LinearFit,-45,45,2);
		gr->Fit(formula,"qn");
		//gr->Fit(formula,"r");
		double c = formula->GetParameter(0);
		double m = formula->GetParameter(1);
		delete formula;

		std::vector<double> vecOfEstimatedXorZ;
		for(unsigned int i = 0 ; i < xVec.size() ; i++){
			vecOfEstimatedXorZ.push_back( (yVec[i] - c)/m );
		}
		return vecOfEstimatedXorZ;
	}


	double GetZenithAngle(std::vector<lite_interface::Point3D*> vecOfPoint3D){
		TVector3 ref(0.,-1.,0.);
		Point3D *startPoint = vecOfPoint3D[0];
		Point3D *endPoint = vecOfPoint3D[vecOfPoint3D.size() - 1];
		TVector3 muonDir(TVector3(endPoint->GetX(), endPoint->GetY(), endPoint->GetZ())	- TVector3(startPoint->GetX(), startPoint->GetY(),startPoint->GetZ()));
		return muonDir.Angle(ref);
	}

	TH1F* PlotZenithAngle(std::vector<double> zenithAngleVect,int opt){
		int numOfBins = 50;
		std::string title="";
		if(opt==1)
			title = "ZenithAngleLinear";
		if(opt==2)
			title = "ZenithAngleParam";
		if(opt==3)
			title = "ZenithAngleMeanHitPoint";
		if(opt==3)
			title = "ZenithAngleCRY";
		TH1F *zenithAngleHist = new TH1F(title.c_str(), title.c_str(),numOfBins,0.02,0.96);
		for (unsigned int i = 0 ; i < zenithAngleVect.size();  i++){
			if(zenithAngleVect[i] < 0.96)
				zenithAngleHist->Fill(zenithAngleVect[i]);
		}
		zenithAngleHist->Scale(1/zenithAngleHist->Integral());
		TF1 *formula = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.05,0.96);
		zenithAngleHist->Fit(formula, "r");
		return zenithAngleHist;
	}

	TH1F* PlotZenithAngle(std::vector<SingleMuonTrack*> muonTrackVec, int opt){
		TVector3 ref(0.,-1.,0.);
		int numOfBins = 50;
		std::string title="";
		if(opt==1)
			title = "ZenithAngleLinear";
		if(opt==2)
			title = "ZenithAngleParam";
		if(opt==3)
			title = "ZenithAngleMeanHitPoint";
		TH1F *zenithAngleHist = new TH1F(title.c_str(), title.c_str(),numOfBins,0.02,0.96);
		std::cout << "Size of Muon Track Vector : " << muonTrackVec.size() << std::endl;
		for(unsigned int trackIndex = 0 ; trackIndex < muonTrackVec.size() ; trackIndex++){
			if(opt == 1){
				double angleVal = muonTrackVec[trackIndex]->GetZenithAngle_Linear();
				if(angleVal < 0.96)
					zenithAngleHist->Fill(angleVal);
			}
			if(opt == 2){
				double angleVal = muonTrackVec[trackIndex]->GetZenithAngle_Param();
				if(angleVal < 0.96)
					zenithAngleHist->Fill(angleVal);
				//zenithAngleHist->Fill(muonTrackVec[trackIndex]->GetZenithAngle_Param());
			}
#ifdef USE_FOR_SIMULATION
			if(opt == 3){
				double angleVal = muonTrackVec[trackIndex]->GetZenithAngle_MeanHitPoint();
				//std::cout << "Angle Value : " << angleVal << std::endl;
				if(angleVal < 0.96)
					zenithAngleHist->Fill(angleVal);
				//zenithAngleHist->Fill(muonTrackVec[trackIndex]->GetZenithAngle_Param());
			}
#endif
		}
		//TF1 *formula = new TF1("Cos2ThetaFit",Cos2ThetaFit,0.02,0.96,2);
		zenithAngleHist->Scale(1/zenithAngleHist->Integral());
		TF1 *formula = new TF1("zenForm", "[0]*sin(x)*cos(x)*pow(cos(x),[1])", 0.05,0.96);
		zenithAngleHist->Fit(formula, "r");
		return zenithAngleHist;
		//gr->Fit(formula,"r");
//		double c = formula->GetParameter(0);
//		double m = formula->GetParameter(1);
//		delete formula;

		/*TH1F *solidAngleCorrectedHist = new TH1F("Solid angle corrected AngularDistribution ","Solid angle corrected AngularDistribution",numOfBins,0.2,0.96);
		solidAngleCorrectedHist->GetYaxis()->SetTitle("I_{#theta}  ( cm^{-2}sec^{-1}st^{-1})");
		solidAngleCorrectedHist->GetXaxis()->SetTitle("#theta (radian)");

		for(int i =0  ; i < numOfBins ; i++){
		 	double binCenter = zenithAngleHist->GetXaxis()->GetBinCenter(i);
		   	double binContent = zenithAngleHist->GetBinContent(i);
		   	//std::cout << "binContent : " << binContent << " : binCenter : " << binCenter << std::endl;
		   	solidAngleCorrectedHist->SetBinContent(i,binContent/(2*M_PI*std::sin(binCenter)*std::cos(binCenter)));
		}
		//new TCanvas();
		TF1 *cosSqr = new TF1("cosSqr", "[0]*pow(cos(x),[1])", 0.2,0.96);
		solidAngleCorrectedHist->Fit(cosSqr,"r");
		//solidAngleCorrectedHist->Draw();

		//
		return solidAngleCorrectedHist;*/

		//zenithAngleHist->Scale(1/zenithAngleHist->Integral());
		/*std::cout << "@@@@@@@@@@ Fitted Parameter for ZenithAngle Histogram @@@@@@@@@" << std::endl;
		new TCanvas();
		TF1 *zenForm = new TF1("zenForm", "[0]*sin(x)*pow(cos(x),[1])", 0.05,M_PI/2.);
		//TF1 *zenForm = new TF1("zenForm", "[0]*pow(cos(x),[1]+1)", 0.05,M_PI/2.);
		zenithAngleHist->Fit(zenForm,"r");
		zenithAngleHist->Draw();

		std::cout << "@@@@@@@@@@ Fitted Parameter for SolidAngle Corrected Histogram @@@@@@@@@" << std::endl;
		TH1F *solidAngleCorrectedHist = new TH1F("Solid angle corrected AngularDistribution ","Solid angle corrected AngularDistribution",numOfBins,0.,M_PI/2.);
		solidAngleCorrectedHist->GetYaxis()->SetTitle("I_{#theta}  ( cm^{-2}sec^{-1}st^{-1})");
		solidAngleCorrectedHist->GetXaxis()->SetTitle("#theta (radian)");

		for(int i =0  ; i < numOfBins ; i++){
		 	double binCenter = zenithAngleHist->GetXaxis()->GetBinCenter(i);
		   	double binContent = zenithAngleHist->GetBinContent(i);
		   	//std::cout << "binContent : " << binContent << " : binCenter : " << binCenter << std::endl;
		   	solidAngleCorrectedHist->SetBinContent(i,binContent/(2*M_PI*std::sin(binCenter)*std::cos(binCenter)));
		}
		new TCanvas();
		//TF1 *cosSqr = new TF1("cosSqr",Cos2ThetaFit,0,M_PI/2,2);
		TF1 *cosSqr = new TF1("cosSqr", "[0]*pow(cos(x),[1])", 0.2,0.96);
		solidAngleCorrectedHist->Fit(cosSqr,"r");
		solidAngleCorrectedHist->Draw();


		//New Histogram
		TH1F *zenithAngle2D = new TH1F("ZenithAngle in 2D","ZenithAngle in 2D using  #frac{z_{2}-z_{1}}{#sqrt{(x_{2}-x_{1})^{2} + (y_{2}-y_{1})^{2} + (z_{2}-z_{1})^{2}}}",numOfBins,-1*M_PI/2.,M_PI/2.);
		//zenithAngle2D->GetYaxis()->SetTitle("I_{#theta}  ( cm^{-2}sec^{-1}st^{-1})");
		zenithAngle2D->GetXaxis()->SetTitle("#theta (radian)");
		for(unsigned int trackIndex = 0 ; trackIndex < fittedMuonTracks.size() ; trackIndex++){
				std::vector<Point3D*> singleMuonTrack = fittedMuonTracks[trackIndex];
				Point3D *startPoint = singleMuonTrack[0];
				//std::cout <<"Start Point : ";
				//startPoint->Print();
				Point3D *endPoint = singleMuonTrack[singleMuonTrack.size()-1];
				//std::cout << "End Point : " ;
				//endPoint->Print();
				TVector3 muonDir(TVector3(endPoint->x,endPoint->y,endPoint->z)-TVector3(startPoint->x,startPoint->y,startPoint->z));
				double angVal = asin((endPoint->z-startPoint->z)/muonDir.Mag());
				//std::cout << "Angle Value : " << angVal << std::endl;
				//zenithAngle2D->Fill((angVal*M_PI/180.)*1000.);
				//if(fabs(startPoint->z) < 50. && fabs(endPoint->z) < 50.)
				zenithAngle2D->Fill(angVal);
		}
		new TCanvas();
		zenithAngle2D->Draw();

*/	}

} /* End of lite_interface */
