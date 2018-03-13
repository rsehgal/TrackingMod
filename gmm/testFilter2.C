void test2(){


	int nxbins = 1000;
	TH2F *hist2 = new TH2F("Data", "Data", nxbins, -10, 20,nxbins, -10.,20.);
	TH2F *hist3 = new TH2F("Data2", "Data2", nxbins, -10, 20,nxbins, -10.,20.);
	TH2F *hist4 = new TH2F("Data3", "Data3", nxbins, -10, 20,nxbins, -10.,20.);
	std::ifstream runfile ("filtered.txt");
	//std::ifstream runfile ("gauss2D.txt");
	double x = 0., y = 0., color = 0.;
	int numOfEvents = 70000; //std::atoi(argv[1]);

	int count2=0, count3=0, count4=0;
	while(numOfEvents){
		runfile >> x >> y >> color;
		if(color==2){
			count2++;
                        hist2->SetMarkerColor(color);
                        hist2->Fill(x,y);
                }
		if(color==3){
			count3++;
			hist3->SetMarkerColor(color);
			hist3->Fill(x,y);
		}
		if(color==4){
			count4++;
                        hist4->SetMarkerColor(color);
                        hist4->Fill(x,y);
                }
		numOfEvents--;
	}

	std::cout << "Count2 : "<< count2 << " : Count3 : "<< count3 <<" : Count4 : " << count4 << std::endl;
	std::cout << "Total : " << (count2+count3+count4) << std::endl;
	hist2->Draw();
	hist3->Draw("same");
	hist4->Draw("same");

}
