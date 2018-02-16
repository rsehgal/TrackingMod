#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "DetectorMapping.h"

using namespace std;

int main()
{
	char fileout[50], fileout2[50], num[10];
	ofstream stream3,stream4;
        FILE *runname;

	int numd = 0;
        // reading run number from file
        //runname = fopen("/home/user/re4data/TDC/run","r");
	runname = fopen("/var/www/html/Tomo/run","r");
        fscanf (runname, "%d\n", &numd);
        fclose(runname);

	
	std::cout << "Run : "<< (numd-1) << std::endl;

        strcpy(fileout,"/var/www/html/Tomo/dqm/");
	char numstring[20];
	sprintf(numstring,"%d",numd-1);
        std::cout << "NumString : "<< numstring << std::endl;   
        strcat(fileout,numstring);
        //strcpy(fileout,numstring);
	strcat(fileout,"f.html");
	//cout << fileout << endl;
	sprintf(num,"%d",numd-1);
	stream3.open(fileout);

	//Writing into the file

	stream3 << " <html> " << endl;
	stream3 << " <head> " << endl;
	stream3 << " <meta http-equiv=""Content-Type""  " << endl;
	stream3 << " content=""text/html; charset=iso-8859-1"">  " << endl;
	stream3 << " <title>Run " << num << " </title> " << endl;
	stream3 << " </head>  " << endl;

	stream3 << "  <frameset rows=""15%,85%"" frameborder=""no""> " << endl;
	stream3 << " <frame src=""http://cms904cr1/cgi-bin/dqm""> " << endl;
	stream3 << " <frame src=""http://cms904cr1/dqm/"<< num <<".html""> " << endl;
	stream3 << "</frameset> " << endl;
	stream3 << "</html>"  << endl;
	stream3.close();


        //strcpy(fileout2,"/srv/www/htdocs/dqm/");
	strcpy(fileout2,"/var/www/html/Tomo/dqm/");
        strcat(fileout2,numstring);
        //strcpy(fileout2,numstring);
	strcat(fileout2,".html");
        //cout << fileout2 << endl;

	stream4.open(fileout2);

	stream4 << "<html> " << endl;

	stream4 << "<head> " << endl;
	stream4 << "<meta http-equiv=""Content-Type"" content=""text/html; charset=iso-8859-1""> " << endl;
	stream4 << "<title>Run " << num << " </title> " << endl;
	stream4 << "</head> " << endl;

	stream4 << "<body> " << endl;
	stream4 << "<p align=""center"">&nbsp;</p> " << endl;
	stream4 << "<div align=""center""><center> " << endl;
	stream4 << "<table border=""0""> " << endl;


	std::string pathOnServer = "../plot/";
	std::string ipaddress = "31.10.2.192";
//    sprintf(num,"%d",numd-1);
    Tomography::DetectorMapping *detectorMap = Tomography::DetectorMapping::create("testMapping.txt");
    //std::string pathOnServer = detectorMap->GetPlotsLocation(); //"../plot/";
    std::vector<std::string> detectorNamesVector = detectorMap->GetDetectorNamesVector();
    for(int i = 0 ; i < detectorNamesVector.size(); i++){
    	stream4 << "<tr> " << endl;
    	stream4 << " <td><img src=" << pathOnServer << num << "-"<< detectorNamesVector[i] << "-StripProfile.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    	stream4 << " <td><img src=" << pathOnServer << num << "-"<< detectorNamesVector[i] << "-XPlane-ClusterSize.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    	stream4 << " <td><img src=" << pathOnServer << num << "-"<< detectorNamesVector[i] << "-YPlane-ClusterSize.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    	stream4 << " <td><img src=" << pathOnServer << num << "-"<< detectorNamesVector[i] << "-Pixel-ClusterSize.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    	stream4 << " <td><img src=" << pathOnServer << num << "-"<< detectorNamesVector[i] << "-HitPlot.gif"" alt=""run_not_yet_analyzed"" width=""450"" height=""450"">" << endl;
    	stream4 << " <td><img src=" << pathOnServer << num << "-"<< detectorNamesVector[i] << "-HitPlot3D.gif"" alt=""run_not_yet_analyzed"" width=""450"" height=""450"">" << endl;
    	stream4 << " <td><img src=" << pathOnServer << num << "-"<< detectorNamesVector[i] << "-Radiograph.gif"" alt=""run_not_yet_analyzed"" width=""450"" height=""450"">" << endl;
    	stream4 << " <td><img src=" << pathOnServer << num  << "-ScintillatorPattern.gif"" alt=""run_not_yet_analyzed"" width=""450"" height=""450"">" << endl;

/*
    	stream4 << " <td><img src=""http://"<< ipaddress << pathOnServer << num << "-"<< detectorNamesVector[i] << "-StripProfile.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    	stream4 << " <td><img src=""http://"<< ipaddress << pathOnServer << num << "-"<< detectorNamesVector[i] << "-XPlane-ClusterSize.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    	stream4 << " <td><img src=""http://"<< ipaddress << pathOnServer << num << "-"<< detectorNamesVector[i] << "-YPlane-ClusterSize.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    	stream4 << " <td><img src=""http://"<< ipaddress << pathOnServer << num << "-"<< detectorNamesVector[i] << "-Pixel-ClusterSize.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
*/
    	//stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile1.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    	//stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize1.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    	stream4 << "</tr> " << endl;
    }

/*
    stream4 << "<tr> " << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "stripprofile1.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile1.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize1.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << "</tr> " << endl;

    stream4 << "<tr> " << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "stripprofile2.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile2.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize2.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << "</tr> " << endl;

    stream4 << "<tr> " << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "stripprofile3.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile3.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize3.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << "</tr> " << endl;

    stream4 << "<tr> " << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "stripprofile4.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile4.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize4.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << "</tr> " << endl;

    stream4 << "<tr> " << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "stripprofile5.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile5.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize5.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << "</tr> " << endl;

    stream4 << "<tr> " << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "stripprofile6.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile6.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize6.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << "</tr> " << endl;

    stream4 << "<tr> " << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "stripprofile7.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile7.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize7.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << "</tr> " << endl;

    stream4 << "<tr> " << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "stripprofile8.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile8.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize8.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << "</tr> " << endl;
    
    stream4 << "<tr> " << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "stripprofile9.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile9.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize9.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << "</tr> " << endl;
    
    stream4 << "<tr> " << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "stripprofile10.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "noisestripprofile10.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << " <td><img src=""http://31.10.3.241/srv/www/htdocs/dqm/plot/" << num << "clustersize10.gif"" alt=""run_not_yet_analyzed"" width=""600"" height=""360"">" << endl;
    stream4 << "</tr> " << endl;
    
    
    
*/
    
    
    stream4.close();

    return 0;
}
