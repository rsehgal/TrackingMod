void Par()
{
//=========Macro generated from canvas: c1/c1
//=========  (Tue Nov  6 10:53:12 2018) by ROOT version 6.14/04
   TCanvas *c1 = new TCanvas("c1", "c1",930,187,795,627);
   c1->SetHighLightColor(2);
   c1->Range(-0.2250406,0.7825542,4.447299,0.8404015);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetLeftMargin(0.1979823);
   c1->SetRightMargin(0.03152585);
   c1->SetBottomMargin(0.1627907);
   c1->SetFrameBorderMode(0);
   c1->SetFrameLineWidth(2);
   c1->SetFrameBorderMode(0);
   
   Double_t Graph0_fx1[4] = {
   0.997851,
   1.990344,
   2.996418,
   3.995702};
   Double_t Graph0_fy1[4] = {
   0.7956171,
   0.8172434,
   0.831109,
   0.8308328};
   TGraph *graph = new TGraph(4,Graph0_fx1,Graph0_fy1);
   graph->SetName("Graph0");
   //graph->SetTitle("Graph");
   graph->SetFillStyle(1000);
   graph->SetLineColor(4);
   graph->SetMarkerColor(6);
   graph->SetMarkerStyle(20);
   graph->SetMarkerSize(1.6);
   
   TH1F *Graph_Graph_Graph011 = new TH1F("Graph_Graph_Graph011","",100,0.7,4.3);
   Graph_Graph_Graph011->SetMinimum(0.7919712);
   Graph_Graph_Graph011->SetMaximum(0.8346168);
   Graph_Graph_Graph011->SetDirectory(0);
   Graph_Graph_Graph011->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph_Graph011->SetLineColor(ci);
   Graph_Graph_Graph011->GetXaxis()->SetTitle("Materials");
   Graph_Graph_Graph011->GetXaxis()->SetBinLabel(9,"Al");
   Graph_Graph_Graph011->GetXaxis()->SetBinLabel(37,"Fe");
   Graph_Graph_Graph011->GetXaxis()->SetBinLabel(64,"Pb");
   Graph_Graph_Graph011->GetXaxis()->SetBinLabel(92,"U");
   Graph_Graph_Graph011->GetXaxis()->CenterTitle(true);
   Graph_Graph_Graph011->GetXaxis()->SetDecimals();
   Graph_Graph_Graph011->GetXaxis()->SetLabelFont(42);
   Graph_Graph_Graph011->GetXaxis()->SetLabelOffset(0.012);
   Graph_Graph_Graph011->GetXaxis()->SetLabelSize(0.08);
   Graph_Graph_Graph011->GetXaxis()->SetTitleSize(0.07);
   Graph_Graph_Graph011->GetXaxis()->SetTitleOffset(1.16);
   Graph_Graph_Graph011->GetXaxis()->SetTitleFont(42);
   Graph_Graph_Graph011->GetYaxis()->SetTitle("Poca Accuracy Ratio");
   Graph_Graph_Graph011->GetYaxis()->CenterTitle(true);
   Graph_Graph_Graph011->GetYaxis()->SetDecimals();
   Graph_Graph_Graph011->GetYaxis()->SetLabelFont(42);
   Graph_Graph_Graph011->GetYaxis()->SetLabelOffset(0.017);
   Graph_Graph_Graph011->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph_Graph011->GetYaxis()->SetTitleSize(0.07);
   Graph_Graph_Graph011->GetYaxis()->SetTitleOffset(0);
   Graph_Graph_Graph011->GetYaxis()->SetTitleFont(42);
   Graph_Graph_Graph011->GetZaxis()->SetLabelFont(42);
   Graph_Graph_Graph011->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph_Graph011->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph_Graph011->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph_Graph011);
   
   graph->Draw("acp");
   
   TPaveText *pt = new TPaveText(0.4398567,0.9343684,0.5601433,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *pt_LaTex = pt->AddText("Graph");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
