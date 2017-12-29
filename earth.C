#include <random>
TCanvas *earth(){
  //this tutorial illustrate the special contour options
  //    "AITOFF"     : Draw a contour via an AITOFF projection
  //    "MERCATOR"   : Draw a contour via an Mercator projection
  //    "SINUSOIDAL" : Draw a contour via an Sinusoidal projection
  //    "PARABOLIC"  : Draw a contour via an Parabolic projection
  //
  //Author: Olivier Couet (from an original macro sent by Ernst-Jan Buis)

   //gStyle->SetPalette(57);
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(1);
   gStyle->SetOptStat(0);

   TCanvas *c1 = new TCanvas("c1","earth_projections",700,700);
   c1->Divide(2,2);

   int nbins =32;
   TH2F *hlego2 = new TH2F("ha","Lego2",    nbins, -50, 50 , nbins, -50, 50);
   TH2F *hcolz = new TH2F("ha","COLZ", nbins   , -50, 50 , nbins, -50, 50);
   TH2F *haitoff = new TH2F("ha","Aitoff",    nbins, -50, 50 , nbins, -50, 50);
   TH2F *hcont4 = new TH2F("ha","CONT4",    nbins, -50, 50 , nbins, -50, 50);
   //TH2F *ha = new TH2F("ha","Aitoff",    5, 0,5 , 5, 0, 5);
   //TH2F *ha = new TH2F("ha","Aitoff",    20, 0, 2, 20, 0, 10);
   //TH2F *ha = new TH2F("ha","Aitoff",    180, -180, 180, 179, -89.5, 89.5);
   //TH2F *hm = new TH2F("hm","Mercator",  5, 0, 5, 5, 0, 5);
   //TH2F *hm = new TH2F("hm","Mercator",  20, 0, 2, 20, 0, 10);
   //TH2F *hm = new TH2F("hm","Mercator",  180, -180, 180, 161, -80.5, 80.5);
   
   //TH2F *hs = new TH2F("hs","Sinusoidal",180, -180, 180, 181, -90.5, 90.5);
   //TH2F *hp = new TH2F("hp","Parabolic", 180, -180, 180, 181, -90.5, 90.5);

   TString dat = gSystem->UnixPathName(__FILE__);
   dat.ReplaceAll(".C",".dat");
   dat.ReplaceAll("/./","/");

   ifstream in;
   in.open(dat.Data());

   auto hprof2d  = new TProfile2D("hprof2d","Profile of pz versus px and py",nbins, -50, 50 , nbins, -50, 50 ,0,1600);
//   Float_t px, py, pz;
//   for ( Int_t i=0; i<25000; i++) {
//      gRandom->Rannor(px,py);
//      pz = px*px + py*py;
//      hprof2d->Fill(px,py,pz,1);
//   }
//   hprof2d->Draw();
   Float_t x,y,z;
   while (1) {
     in >> x >> y ;
     //in >> x >> y >> z;
     /*
     std::random_device rd;
         std::mt19937 gen(rd());
         std::uniform_real_distribution<> dis(-50., 50.);
	 x = dis(gen);

         //std::random_device rd;
         //std::mt19937 gen(rd());
         //std::uniform_real_distribution<> dis(-50., 50.);
         y = dis(gen);
  */

     if (!in.good()) break;
	//if( hlego2->RetrieveBinContent() < 155){
	int binNum = x*nbins+y;
        if(hprof2d->GetBinEntries(binNum) < 2400)
	{
  	hlego2->Fill(x,y);
  	hcolz->Fill(x,y);
  	haitoff->Fill(x,y);
  	hcont4->Fill(x,y);
        hprof2d->Fill(x,y,1);
	}
	//}
  	//ha->Fill(x,y);
  	//ha->Fill(x,y,z);
//	hm->Fill(x,y,z);
     //ha->Fill(x,y, 1);
     // hm->Fill(x,y, 1);
     //hs->Fill(x,y, 1);
     //hp->Fill(x,y, 1);
   }
   in.close();

   c1->cd(1); hlego2->Draw("lego2");
   c1->cd(2); hcolz->Draw("COLZ");
   c1->cd(3); haitoff->Draw("aitoff");
   c1->cd(4); hcont4->Draw("CONT4");
   //c1->cd(5); hprof2d->Draw("COLZ");
   //c1->cd(2); hm->Draw("mercator");
   //c1->cd(3); hs->Draw("sinusoidal");
   //c1->cd(4); hp->Draw("parabolic");

   return c1;
}
