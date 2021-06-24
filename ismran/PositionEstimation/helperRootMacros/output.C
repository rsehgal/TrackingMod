void output(){
TTree *t = new TTree("t", "tree from ytt_yt.csv");
t->ReadFile("output.csv","q/D:delt/D:ypred/D");
new TCanvas("Q","Q");
t->Draw("q");
new TCanvas("delT","delT");
t->Draw("delt");
new TCanvas("yPred","yPred");
t->Draw("ypred");
}
