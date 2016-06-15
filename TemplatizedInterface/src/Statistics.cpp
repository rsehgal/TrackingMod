#include "Statistics.h"

Statistics::Statistics() {

}

void Statistics::Initialize() {

	detectors = Tomography::SetupManager::instance()->GetDetectorVector("GLASS");
	FiredXStrips.resize(32);
	FiredYStrips.resize(32);
	TotalCluster.resize(detectors.size());
	for(int i = 0 ; i < TotalCluster.size() ; i++)
	{
		TotalCluster[i].resize(1024);
		for(int j = 0 ; j < 1024 ; j++)
			TotalCluster[i][j] = 0;
	}
}

void Statistics::GetXPlaneHistogram() {

//Application *app = new TApplication("Histograms", NULL, NULL);
std::vector<TCanvas*> c;
c.resize(detectors.size());
int count[detectors.size()];
for(int i = 0 ; i < detectors.size() ; i++)
count[i] = 0;

for(int i = 0 ; i < c.size() ; i++)
{
	c[i] = new TCanvas(detectors[i]->GetName().c_str(), "Fired Strip/Pixel Data", 200, 10);
	c[i]->Divide(2,2);
}

  int nxbins = 32;
  int nybins = 32;
  int pbins = 1024;
  int xlow = 0;
  int xhigh = 10;
 
 	std::vector<TH1F*> histogram;
 	histogram.resize(detectors.size()*3);
for(int i = 0 ; i < histogram.size() ; i = i+3)
{
	histogram[i] = new TH1F(detectors[i/3]->GetPlane(0)->GetName().c_str(), "Data", nxbins, 0, nxbins);
	histogram[i+1] = new TH1F(detectors[i/3]->GetPlane(1)->GetName().c_str(), "Data", nybins, 0, 32);
	histogram[i+2] = new TH1F(("pixels_"+detectors[i/3]->GetPlane(0)->GetName()).c_str(), "Data", pbins,0, 1024);
}
 // TH2F *h2d = new TH2F("h2d", "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh);

//TCanvas *c1 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);

int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();
  //  Tracking::Vector3D<double> temp;
   // std::vector<HittedPixel *> hittedPixelVector;

   // std::vector<Detector *> detectors = Tomography::SetupManager::instance()->GetDetectorVector("GLASS");

    for (int evNo = 0; evNo < numOfEvents; evNo++) {
     //std::cout<<" EvNo : "<< evNo << std::endl;

      for (int j = 0; j < detectors.size(); j++) {
        detectors[j]->SetFiredStripsVector(evNo);
        //std::cout<<"Hello Vatsal" << std::endl;
        //std::cout<<"Value : "<< detectors[j]->GetPlane(0)->GetFiredStripsVector().size() <<std::endl;

      //  detectors[j]->GetPlane(0)->GetFiredStripsVector().size()
       // for (int xval = 0; xval < detectors[j]->GetPlane(0)->GetFiredStripsVector().size(); xval++) 
       // {
        	histogram[j*3]->Fill(detectors[j]->GetPlane(0)->GetFiredStripsVector().size());	
        
        //  for (int yval = 0; yval < detectors[j]->GetPlane(1)->GetFiredStripsVector().size(); yval++) 
         // {
          	histogram[j*3+1]->Fill(detectors[j]->GetPlane(1)->GetFiredStripsVector().size());
         // }
           histogram[j*3+2]->Fill(detectors[j]->GetPlane(0)->GetFiredStripsVector().size() * detectors[j]->GetPlane(1)->GetFiredStripsVector().size());
           TotalCluster[j][detectors[j]->GetPlane(0)->GetFiredStripsVector().size() * detectors[j]->GetPlane(1)->GetFiredStripsVector().size()]++;
      //  } 
           if(detectors[j]->GetPlane(0)->GetFiredStripsVector().size() == 0 || detectors[j]->GetPlane(1)->GetFiredStripsVector().size() == 0)
          //  if(detectors[j]->GetPlane(0)->GetFiredStripsVector().size()  || detectors[j]->GetPlane(1)->GetFiredStripsVector().size())
           	count[j]++;
          

          }
      }
      //double tmp = ((double)(numOfEvents-count[0]))*100.;
      // double tmp1 = ((double)(numOfEvents-count[1]))*100.;
     // std::cout<<"Efficiency for Detector 1: "<<tmp/(double)numOfEvents<<"%"<<std::endl;
      // std::cout<<"Efficiency for Detector 2: "<<tmp1/(double)numOfEvents<<"%"<<std::endl;
      //std::cout<<"Efficiency for Detector 1: "<<count/(double)numOfEvents * 100<<"%"<<std::endl;
        for(int i = 0 ; i < detectors.size() ; i++)
        {
        	c[i]->cd(1);
        	histogram[i*3]->Draw();

        	c[i]->cd(2);
        	histogram[i*3+1]->Draw();

        	c[i]->cd(3);
        	histogram[i*3+2]->Draw();
        }    

        for(int j = 0 ; j < detectors.size() ; j++)
        { 
        	double tmp = ((double)(numOfEvents-count[j]))*100.;
        	
	        std::cout<<"Efficiency for Detector "<<j+1<<": "<<tmp/(double)numOfEvents<<"%"<<std::endl;

	        for(int i = 0 ; i < TotalCluster[j].size() ; i++)
	        {
	        	if(TotalCluster[j][i] != 0)
	        	{  
	        		tmp = (double)TotalCluster[j][i]*100;
					std::cout<<"Total Pixels Fired: "<<i<<"   "<<"Percentage: "<<tmp/(double)numOfEvents<<std::endl;
	       		 }
	        }
    	}

}

void Statistics::GetYPlaneHistogram() {

}

void Statistics::GetClusterHistogram() {

}

void Statistics::StripProfile() {

std::vector<TCanvas*> c;
c.resize(detectors.size());
int count[detectors.size()];

for(int i = 0 ; i < detectors.size() ; i++)
count[i] = 0;

for(int i = 0 ; i < c.size() ; i++)
{
	c[i] = new TCanvas(detectors[i]->GetName().c_str(), detectors[i]->GetName().c_str(), 800, 600);
	c[i]->Divide(2,1);
}

  int nxbins = 64;
  int nybins = 64;
  int pbins = 1024;
  int xlow = 0;
  int xhigh = 10;
 
 	std::vector<TH1F*> histogram;
 	histogram.resize(detectors.size());

for(int i = 0 ; i < histogram.size() ; i++)
{
	histogram[i] = new TH1F(detectors[i/2]->GetName().c_str(), "StripProfile", nxbins, 0, nxbins);
	//histogram[i+1] = new TH1F(detectors[i/2]->GetPlane(1)->GetName().c_str(), "StripYProfile", nybins, 0, nybins);
	//histogram[i+2] = new TH1F(("pixels_"+detectors[i/3]->GetPlane(0)->GetName()).c_str(), "Data", pbins,0, 1024);
}
 // TH2F *h2d = new TH2F("h2d", "Timing", nxbins, xlow, xhigh, nybins, ylow, yhigh);

//TCanvas *c1 = new TCanvas("c2", "Timing-Info", 200, 10, 700, 500);

int numOfEvents = Tracking::Tree::instance()->GetNumOfEvents();


	for (int evNo = 0; evNo < numOfEvents; evNo++) {
     //std::cout<<" EvNo : "<< evNo << std::endl;

      for (int j = 0; j < detectors.size(); j++) {
        detectors[j]->SetFiredStripsVector(evNo);
        //std::cout<<"Hello Vatsal" << std::endl;
        //std::cout<<"Value : "<< detectors[j]->GetPlane(0)->GetFiredStripsVector().size() <<std::endl;

      //  detectors[j]->GetPlane(0)->GetFiredStripsVector().size()
       // for (int xval = 0; xval < detectors[j]->GetPlane(0)->GetFiredStripsVector().size(); xval++) 
       // {
       	for(int k = 0 ; k < detectors[j]->GetNumOfPlanes() ; k++)
       	{
       		for(int l = 0 ; l < detectors[j]->GetPlane(k)->GetNumOfScintillators() ; l++)
       		{
       			if(Tracking::Tree::instance()->GetEntry(detectors[j]->GetPlane(k)->GetScintVector()[l]->GetName(),evNo)->size())
       			{
       				//std::cout<<Tracking::Tree::instance()->GetEntry(detectors[j]->GetPlane(k)->GetScintVector()[l]->GetName(),evNo)->size()<<std::endl;
       				histogram[j]->Fill(detectors[j]->GetPlane(k)->GetScintVector()[l]->GetStripNum());
       			}
       		}	
        	
       	}
     //   histogram[j*2]->Fill(detectors[j]->GetPlane(0)->GetFiredStripsVector().size());	
        //  for (int yval = 0; yval < detectors[j]->GetPlane(1)->GetFiredStripsVector().size(); yval++) 
         // {
  //        	histogram[j*2+1]->Fill(detectors[j]->GetPlane(1)->GetFiredStripsVector().size());
         // }
    //       histogram[j*3+2]->Fill(detectors[j]->GetPlane(0)->GetFiredStripsVector().size() * detectors[j]->GetPlane(1)->GetFiredStripsVector().size());
     //      TotalCluster[j][detectors[j]->GetPlane(0)->GetFiredStripsVector().size() * detectors[j]->GetPlane(1)->GetFiredStripsVector().size()]++;
      //  } 
      //    if(detectors[j]->GetPlane(0)->GetFiredStripsVector().size() == 0 || detectors[j]->GetPlane(1)->GetFiredStripsVector().size() == 0)
          //  if(detectors[j]->GetPlane(0)->GetFiredStripsVector().size()  || detectors[j]->GetPlane(1)->GetFiredStripsVector().size())
         //  	count[j]++;
          

          }
      }

 		for(int i = 0 ; i < detectors.size() ; i++)
        {
        	c[i]->cd(1);
        	histogram[i]->Draw();

        //	c[i]->cd(2);
        //	histogram[i*2+1]->Draw();

        } 
}

