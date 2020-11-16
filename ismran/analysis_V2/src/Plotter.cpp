#include "Plotter.h"
#include <iterator>
#include "HardwareNomenclature.h"

namespace lite_interface{

	TH1F* PlotQ(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex){
		TH1F *hist = new TH1F("RawHist","RawHist",40000,0,40000);
		std::vector<ScintillatorBar_V2*>::iterator itr;
		for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
			if((*itr)->fBarIndex == barIndex){
				hist->Fill((*itr)->GetQLongMean());
			}
		}
		return hist;
	}

	TH1F* PlotQ_0123(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex, ushort opt){
		TH1F *hist;
		unsigned int nbins = 1000;
		unsigned int start = 0;
		unsigned int end = 40;
		if(opt == 0){
			hist = new TH1F("HistQMean","HistQMean",nbins,start,end);
			std::vector<ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				if(barIndex==65535){
					hist->Fill((*itr)->GetQLongMean()/1000.);
				}else{
					if((*itr)->fBarIndex == barIndex){
						hist->Fill((*itr)->GetQLongMean()/1000.);
					}
				}
			}	
		}

		if(opt == 1){
			hist = new TH1F("HistQNear","HistQNear",nbins,start,end);
			std::vector<ScintillatorBar_V2*>::iterator itr;
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
			std::vector<ScintillatorBar_V2*>::iterator itr;
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
			std::vector<ScintillatorBar_V2*>::iterator itr;
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

	TH1F* PlotQMeanCorrected(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex){
		return PlotQ_0123(scintBarVec,barIndex,3);
	}

	TH1F* PlotQMean(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex){
		return PlotQ_0123(scintBarVec,barIndex,0);
	}
	
	TH1F* PlotQNear(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex){
		return PlotQ_0123(scintBarVec,barIndex,1);
	}

	TH1F* PlotQFar(std::vector<ScintillatorBar_V2*> scintBarVec,ushort barIndex){
		return PlotQ_0123(scintBarVec,barIndex,2);
	}
	TH1F* PlotDelT(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex){
		TH1F *hist = new TH1F("HistDelT","HistDelT",200,-25,25); //Histogram with entries in nanosecond
		std::vector<ScintillatorBar_V2*>::iterator itr;
		for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
			if((*itr)->fBarIndex == barIndex){
				//std::cout << "BarIndex : " << (*itr)->fBarIndex << " : DelT : " << (*itr)->GetDelT()/1000. << std::endl;
				hist->Fill((*itr)->GetDelT()/1000.);
			}
		}
		return hist;
	}
	TH1F* PlotDelTCorrected(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex){
		std::string barName = vecOfBarsNamess[barIndex];
		TH1F *hist = new TH1F(barName.c_str(),barName.c_str(),200,-25,25); //Histogram with entries in nanosecond
		std::vector<ScintillatorBar_V2*>::iterator itr;
		for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
			if((*itr)->fBarIndex == barIndex)
				hist->Fill((*itr)->GetDelTCorrected()/1000.);
		}
		return hist;
	}

	TH1F* PlotBarsProfile(std::vector<ScintillatorBar_V2*> scintBarVec) {
		//std::string barName = vecOfBarsNamess[barIndex];
		TH1F *hist = new TH1F("Bars Profile", "Bars Profile", numOfLayers*numOfBarsInEachLayer, 0, numOfLayers*numOfBarsInEachLayer); //Histogram with entries in nanosecond
		std::vector<ScintillatorBar_V2*>::iterator itr;
		for (itr = scintBarVec.begin(); itr != scintBarVec.end(); itr++) {

				hist->Fill((*itr)->GetBarIndex());
		}
		return hist;
	}

	TH2F* PlotHitPointsOnBar(std::vector<ScintillatorBar_V2*> scintBarVec, ushort barIndex){
		ushort nbinsx = 9;
		ushort nbinsz = 10;
		std::string name = "Hit points on "+vecOfBarsNamess[barIndex];
		TH2F *hist = new TH2F(name.c_str(),name.c_str(),nbinsx,-45,45,nbinsz,-50,50);
		std::vector<lite_interface::ScintillatorBar_V2*>::iterator itr;
		for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
			if((*itr)->fBarIndex == barIndex){
				lite_interface::Point3D *hitPt = (*itr)->EstimateHitPosition();
//				/hitPt->Print();
				hist->Fill(hitPt->GetX(),hitPt->GetZ());
			}
		}
		return hist;
	}

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
			xVecErr.push_back(0.);
			yVec.push_back((*itr)->GetY());
			yVecErr.push_back(0.);
			zVec.push_back((*itr)->GetZ());
			zVecErr.push_back(0.);
		}
		TGraphErrors *grxy = new TGraphErrors(xVec.size(), &xVec[0], &yVec[0],&xVecErr[0],&yVecErr[0]);
		grxy->SetMarkerStyle(8);
		grxy->SetMarkerColor(kMagenta);
		TGraphErrors *grzy = new TGraphErrors(zVec.size(), &zVec[0], &yVec[0], &zVecErr[0], &yVecErr[0]);
		grzy->SetMarkerStyle(8);
		grzy->SetMarkerColor(kBlue);
		if(opt==1)
			return grxy;
		if(opt==2)
			return grzy;
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
			std::cout << "@@@@@@@@@@@@@@@ EVENT : " << count++ << " @@@@@@@@@@@@@@@@@@@" << std::endl;
			(*itr)->Print();
			energSumHist->Fill((*itr)->GetEnergySum());
		}
		//energSumHist->Scale(1/energSumHist->Integral());
		return energSumHist;
	}

	TCanvas* PlotEnergyDistributionWithMultiplicity(std::vector<SingleMuonTrack*> muonTrackVec, unsigned int multiplicity){
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

	}

	return can;


	//histEnergyWithMultiplicity->Draw();
}
} /* End of lite_interface */
