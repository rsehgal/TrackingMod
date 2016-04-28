/*
 * Statistics.h
 *
 *  Created on: Mar 16, 2016
 *      Author: rsehgal
 */

#ifndef INC_STATISTICS_H_
#define INC_STATISTICS_H_
#include <sstream>
#include <iostream>
#include "base/Global.h"
#include <TH1F.h>
#include <TH2F.h>
#include "Scintillator.h"
#include <TCanvas.h>
#include <TFile.h>

//#include "OfflineAnalysis.h"

namespace Tracking{

  class Statistics{

  private:
    int fStationNum;
    std::string fStationName;
    std::string fEtaPartition;
    std::string fTriggerLayer;

    //OfflineAnalysis fOffAnal;
    std::string fTitleStringStripProfile;
    std::string fTitleStringNoiseProfile;
    std::string fTitleStringStripClusterSize;
    std::string fTitleStringClusterMultiplicity;
    std::string fTitleStringCrossTalk;
    std::string fTitleStringHitMultiplicity;

    //Histograms variables
    TH1F *h_Strip;
    TH1F *h_Stripnoise;
    TH1F *h_clustersize;
    TH1F *h_clustermultiplicity;
    TH1F *h_hitmultiplicity;
    TH1F *h_xtalk;
    TH2F *h_Strip2d;

    // Variables settings
    int trg_windows_min;
    int trg_windows_max;// = 120; // Time window in ns.
    int delaytrg;// = 100;                                // Time in ns between: [master trigger] and [small counter]
    //int scint_multiplicity_max;// = 2;         // Number of counts at same time found in one SCINT layer (min. value = 2).
    bool master_trigger_cutedge;// = 0;        // Once enabled (=1) it does not consider trigger in lateral SCINT.
    int scint_max;// = 500;                    // Max distance in time between SCINT. (ns*10) for shower detection
    bool shower;// = 1;                        // Activates shower detection 1=ON, 0=OFF
    int canale_cut;// = 0;                     // Strips not to be considered (at the edges)
    int reflimit;// = 2;                       // Number of strips not to be considered when using additional RPC as TRG
    int maxprocessable;// = 0;                 // Leave zero for automatic, put a value if you like to impose a value
    std::string site;// = "BARC";                   // This is the site production
    int onerpctriggermultiplicity_max;// = 10; // Maximum number of hits in the 1-RPC ref trigger
    int tworpctriggermultiplicity_max;// = 10;

    // Cosmetics settings strip, noise, cluster plots
    int strip_start;// = 0, strip_finish = 96;                   // MIN-MAX X AXIS limit for occupancy
    int strip_finish;
    int maxbins;// = 100;                                        // MAX Y AXIS - Strip profile
    int hit_max;// = 100;                                        // MAX Y AXIS - Hit multiplicity
    int noisemaxbins;
    //int maxbins;// * 80;                          // MAX Y AXIS - Noise profile
    int csize_max;// = maxbins * 3;                              // MAX Y AXIS - Cluster size
    int cmulti_max;// = maxbins * 3;                             // MAX Y AXIS - Number of clusters
    int timemin;//
    int timemax;
    int framext_start;
    int framext_finish;
    int xtmax;
    int max_modules;// = 6;
    int strip_bin;// = strip_finish - strip_start;
    int framext_bin;
    int xtbins;

  public:
    Statistics();

    ~Statistics(){}

    //Function to generate title string for all the histograms
    TRACKING_INLINE
    void SetStatistics(std::string stationName,int stationNum,std::string etaPartition,std::string triggerLayer){
      fStationNum = stationNum;
      fStationName = stationName;
      fEtaPartition = etaPartition;
      fTriggerLayer = triggerLayer;

      fTitleStringStripProfile="Strips Chamber ID: ";
      fTitleStringNoiseProfile="Noisy strips Chamber ID: ";
      fTitleStringStripClusterSize="Cluster size Chamber ID: ";
      fTitleStringClusterMultiplicity="Cluster multiplicity Chamber ID: ";
      fTitleStringCrossTalk="CrossTalk Chamber ID: ";
      fTitleStringHitMultiplicity="Hit multiplicity Chamber ID: ";

      GenerateAllTitleStrings();
    }

    void GenerateAllTitleStrings();

    void GenerateStripProfile();
    void GenerateNoiseProfile();
    void GenerateClusterSize();
    void GenerateClusterMultiplicity();
    void GenerateCrossTalkProfile();
    void GenerateHitMultiplicity();

    void GenerateTimingHistogram();

  };//end of Statistics class
}//end of Tracking namespace



#endif /* INC_STATISTICS_H_ */
