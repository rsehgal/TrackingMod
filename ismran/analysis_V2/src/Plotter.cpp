#include "Plotter.h"
#include <iterator>

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
				if((*itr)->fBarIndex == barIndex){
					hist->Fill((*itr)->GetQLongMean()/1000.);
				}
			}	
		}

		if(opt == 1){
			hist = new TH1F("HistQNear","HistQNear",nbins,start,end);
			std::vector<ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				if((*itr)->fBarIndex == barIndex){
					hist->Fill((*itr)->GetQLongNear()/1000.);
				}
			}
		}

		if(opt == 2){
			hist = new TH1F("HistQFar","HistQFar",nbins,start,end);
			std::vector<ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				if((*itr)->fBarIndex == barIndex){
					hist->Fill((*itr)->GetQLongFar()/1000.);
				}
			}
		}
		if(opt == 3){
			hist = new TH1F("HistQMeanCorrected","HistQMeanCorrected",nbins,start,end);
			std::vector<ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				if((*itr)->fBarIndex == barIndex){
					hist->Fill((*itr)->GetQMeanCorrected());
				}
			}
		}
		hist->Scale(1/hist->Integral());
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
		TH1F *hist = new TH1F("HistDelT","HistDelT",100,-25,25); //Histogram with entries in nanosecond
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
		TH1F *hist = new TH1F("HistDelTCorrected","HistDelTCorrected",100,-25,25); //Histogram with entries in nanosecond
		std::vector<ScintillatorBar_V2*>::iterator itr;
		for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
			if((*itr)->fBarIndex == barIndex)
				hist->Fill((*itr)->GetDelTCorrected()/1000.);
		}
		return hist;
	}
} /* End of lite_interface */
