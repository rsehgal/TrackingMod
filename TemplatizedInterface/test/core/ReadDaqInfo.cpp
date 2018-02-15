/*
 * ReadDaqInfo.cpp
 *
 *  Created on: Feb 15, 2018
 *      Author: rsehgal
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(){
	std::string run_config_file = "6928.info";
	ifstream rff(run_config_file);
	bool verbose_openfile = true;
	if (rff.is_open()) {
		if (verbose_openfile)
			cout << "Run config file: " << run_config_file << " is opened."
					<< endl;

		// This is the header of the daqinfo file
		std::string variablestring1;
		double temperature = 0., pressure = 0., humidity = 0.;
		rff >> variablestring1 >> temperature >> humidity >> pressure;

		if (verbose_openfile)
			cout << "Temp: " << temperature << " humidity: " << humidity << " pressure: " << pressure << endl;

		std::string variablestring2, variablestring3, Trigger_layer;
		char etapartition;
		rff >> variablestring2 >> etapartition >> variablestring3
				>> Trigger_layer;
		if (verbose_openfile)
			cout << "Eta partition: " << etapartition << " trigger: " << Trigger_layer << endl;

		std::string rff_conf_line;
		getline(rff, rff_conf_line);
		if (verbose_openfile)
			cout << "reading this line: " << rff_conf_line << endl;

		getline(rff, rff_conf_line);
		if (verbose_openfile)
			cout << "reading this line: " << rff_conf_line << endl;

		std::string chamber_tn, chamber_tw, chamber_b;
		float curr1 = 0., vset1 = 0., vmon1 = 0., curr2 = 0., vset2 = 0., vmon2 = 0., curr3 = 0., vset3 = 0., vmon3 = 0.;
		int station = 0, feb[13];
		while (rff.good()) {
			getline(rff, rff_conf_line);

			rff >> chamber_tn >> curr1 >> vset1 >> vmon1 >> station;
			rff >> chamber_tw >> curr2 >> vset2 >> vmon2 >> station;
			rff >> chamber_b >> curr3 >> vset3 >> vmon3 >> station;
			rff >> variablestring1 >> feb[1] >> feb[2] >> feb[3] >> feb[4]
					>> feb[5] >> feb[6] >> feb[7] >> feb[8] >> feb[9] >> feb[10]
					>> feb[11] >> feb[12];

			if (verbose_openfile)
				cout << " " << endl << " " << endl;
			if (verbose_openfile)
				cout << "Gap: " << chamber_tn << " current: " << curr1
						<< " vset: " << vset1 << " vmon1: " << vmon1
						<< " station=" << station << endl;
			if (verbose_openfile)
				cout << "Gap: " << chamber_tw << " current: " << curr2
						<< " vset: " << vset2 << " vmon2: " << vmon2
						<< " station=" << station << endl;
			if (verbose_openfile)
				cout << "Gap: " << chamber_b << " current: " << curr3
						<< " vset: " << vset3 << " vmon3: " << vmon3
						<< " station=" << station << endl;
			if (verbose_openfile)
				cout << "FEB values: " << feb[1] << " " << feb[2] << " "
						<< feb[3] << " " << feb[4] << " " << feb[5] << " "
						<< feb[6] << " " << feb[7] << " " << feb[8] << " "
						<< feb[9] << " " << feb[10] << " " << feb[11] << " "
						<< feb[12] << endl;

		}
	}

}


