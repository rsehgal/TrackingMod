{
   TCanvas *c1 = new TCanvas("c1","ABC",10,10,900,500);
   const Int_t n = 20;
   Int_t i;
   char *ABC[n] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T"};
   Double_t x[n], y[n];
   for (i=0;i<n;i++) {
      x[i] = i*0.1;
      y[i] = 10*sin(x[i]+0.2);
   }
   TH1F *h = new TH1F("h","test",n,x[0],x[n-1]);
   for (i=1;i<=n;i++) h->GetXaxis()->SetBinLabel(i,ABC[i-1]);
   h->SetMaximum(12);
   h->Draw();
   TGraph *gr = new TGraph(n,x,y);
   gr->Draw("CP");
}
