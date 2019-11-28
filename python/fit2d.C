#include <TMath.h>

Double_t Gaus2D(Double_t *x, Double_t *par)
{

if(par[2] > 0 && par[4] > 0) 
{ double rx=(x[0]-par[1])/par[2]; 
double ry=(x[1]-par[3])/par[4]; 
return par[0]*TMath::Exp(-(rx*rx+ry*ry)/2.); 
}
else return 0.;
}


Double_t FitMultiGauss(Double_t *x, Double_t *pars){
return Gaus2D(x,pars)+Gaus2D(x,&pars[5]) +Gaus2D(x,&pars[10]);
}

void fit2d(){
double xmin=-20.,xmax=50.,ymin=-20,ymax=50;
   //TH1F *hpx=new TH1F("Gauss","Gauss",1000, xmin,xmax);
   int bins=50;
   TH2F *hpx=new TH2F("Gauss","Gauss",bins, xmin,xmax,bins,ymin,ymax);
   TRandom t;
   double mean1=5, sigma1=3;
   double mean2=5, sigma2=3;
   for(int i = 0 ; i < 20000; i++){
	double val1=t.Gaus(mean1,sigma1);
	double val2=t.Gaus(mean2,sigma2);
 	if(val1 < xmax && val1 > xmin && val2 < ymax && val2 > ymin){
		hpx->Fill(val1,val2);
		hpx->Fill(val1+10,val2+10);
		hpx->Fill(val1,val2+10);
	}
  
   }
   hpx->Smooth(2);
//   hpx->Draw("SURF1");

//TF2 *g2D=new TF2("g2d",Gaus2D,-10,10,     -10,10,5); //Working
TF2 *g2D=new TF2("g2d",FitMultiGauss,-10,10,     -10,10,15);

//g2D->SetParameters(293,4.97,3.27,4.99,3.25);
//g2D->SetParameters(50,5,6,5,8, 50,15,6,15,8, 80,5,6,15,8);//,30,15,50,15,10);

hpx->Fit(g2D,"V");
hpx->Draw("surf1");

//g2D->Draw("surf, same");

}
