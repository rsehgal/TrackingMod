void test2(){


	int nxbins = 1000;
	TH2F *hist = new TH2F("Data", "Data", nxbins, -10, 20,nxbins, -10.,20.);
	std::ifstream runfile ("gauss2D.txt");
	double x = 0., y = 0.;
	int numOfEvents = 50000;//std::atoi(argv[1]);

	while(numOfEvents){
		runfile >> x >> y;
		hist->Fill(x,y);
		numOfEvents--;
	}

	hist->Draw();

}
