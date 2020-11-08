#include "Plotter.h"
#include <iterator>

namespace lite_interface{

	TH1F* PlotQ_012(std::vector<ScintillatorBar_V2*> scintBarVec, ushort opt){
		TH1F *hist;
		if(opt == 0){
			hist = new TH1F("HistQMean","HistQMean",40,0,40);
			std::vector<ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				hist->Fill((*itr)->GetQLongMean());
			}	
		}

		if(opt == 1){
			hist = new TH1F("HistQNear","HistQNear",40,0,40);	
			std::vector<ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				hist->Fill((*itr)->GetQLongNear());
			}
		}

		if(opt == 2){
			hist = new TH1F("HistQFar","HistQFar",40,0,40);	
			std::vector<ScintillatorBar_V2*>::iterator itr;
			for(itr = scintBarVec.begin() ; itr != scintBarVec.end() ; itr++){
				hist->Fill((*itr)->GetQLongFar());
			}
		}
		return hist;
	}



	TH1F* PlotQMean(std::vector<ScintillatorBar_V2*> scintBarVec){
		return PlotQ_012(scintBarVec,0);
	}
	
	TH1F* PlotQNear(std::vector<ScintillatorBar_V2*> scintBarVec){
		return PlotQ_012(scintBarVec,1);
	}

	TH1F* PlotQFar(std::vector<ScintillatorBar_V2*> scintBarVec){
		return PlotQ_012(scintBarVec,2);
	}
} /* End of lite_interface */