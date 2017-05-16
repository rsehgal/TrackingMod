#include "BSCDataReader.h"

#include <iostream>
#include <sstream>
#include "IniFile.h"
//#include "BSCModuleV1721.h"
//#include "ModuleV
#include <time.h>
#include <stdio.h>
#include <math.h>

using std::cout;
using std::endl;
using std::stringstream;

BSCDataReader::BSCDataReader() : fDataFile( NULL ),fDataTree( NULL ), fSpillNum( 0 ), fModuleCount( 0 ){
  BSCModule * mod;
  mod = new ModuleV2718;
  _modMap[ "V2718" ] = mod;
  mod = new ModuleV1190;
  _modMap[ "V1190" ] = mod;
}

BSCDataReader::~BSCDataReader(){
        fModules.clear();
        if( fDataFile ) delete fDataFile;
        //if( fDataTree ) delete fDataTree;
}

void BSCDataReader::Initialize( string aIniFileName ){

  //Added by Raman
  GetSkimmedData();

  IniFile * ini = new IniFile( aIniFileName );
  ini->Read();
  this->SetNumberOfSpills( ini->Int( "General", "MaxEvents", 10 ) );

  char bufName[ 100 ];
  char bufNameshort[ 16 ];
  char temp[20];
  char stack[20];
  char execstring[ 100 ];
  char trgeta[20];
  char layer[20];
  int slot_d, channel_d, stat, pwr, i, slot, channel, tdc,run,febtdc,verbosefeb=0;
  float hvset, hvmon, currset, curr;
  char chambername [25], temp1[25], temp2[25];
  int achip1,achip2,achip3,achip4,bchip1,bchip2,bchip3,bchip4,cchip1,cchip2,cchip3,cchip4;
  int dchip1,dchip2,dchip3,dchip4,echip1,echip2,echip3,echip4,fchip1,fchip2,fchip3,fchip4;
  int vachip1,vachip2,vachip3,vachip4,vbchip1,vbchip2,vbchip3,vbchip4,vcchip1,vcchip2,vcchip3,vcchip4;
  int vdchip1,vdchip2,vdchip3,vdchip4,vechip1,vechip2,vechip3,vechip4,vfchip1,vfchip2,vfchip3,vfchip4;
  float var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13, var14, var15, var16, var17;
  int pressurezero = 990;
   
  FILE *stream;

  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime ( bufNameshort, 16,"%Y%m%d%H%M%S", timeinfo );
  //strftime ( bufName, 100,"/home/user/re4data/TDC/%Y%m%d%H%M%S.root", timeinfo );
  // Incremental run number
  FILE *runnumberfile;
  runnumberfile = fopen("/home/user/re4data/TDC/run","r");

  while (!feof(runnumberfile)) {
          fscanf(runnumberfile, "%d\n", &run);
  }
  fclose(runnumberfile);
  run = run + 1;
  strcpy(bufName,"/home/user/re4data/TDC/");
  sprintf(temp, "%d",run);
  strcat(bufName,temp);
  strcat(bufName,".root");


  // Writing the current runname on a file
  if( (stream = fopen( "/home/user/re4data/TDC/run", "w" )) != NULL ) {
     //fwrite( bufNameshort, sizeof( char ), 14, stream );
     fprintf (stream, "%d\n",run);
     fclose( stream );
  }
  system("/home/user/workspace/rpcupgrade/CAEN1527_monitoring-1.00/CAEN1527_monitoring/htmlout");

  // Sending an e-log
  //strcpy(execstring,"elog -h cms904cr1.cern.ch -p 8080 -l 904lab -a Author=DAQ_USER -a Type=''Run'' -a Subject=''");
  //sprintf(bufNameshort, "%d", run);
  //strcat(execstring, bufNameshort);


  // Taking info from hardware to be put into elog
  i=0;
  FILE *streamconf;
  FILE *streamdump;
  FILE *streamlog;
  FILE *streamfeb;
  FILE *meteolog;
  FILE *meteolog2;
  FILE *streameta;
  FILE *streamstack;

  streamconf = fopen("/srv/www/htdocs/conf.txt","r");
  streamlog = fopen("/home/user/re4data/TDC/daqinfo","w");

  meteolog = fopen("/home/user/meteolog","r");
  meteolog2 = fopen("/home/user/meteolog2","r");
  fscanf (meteolog, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", &var1, &var2, &var3, &var4, &var5, &var6, &var7, &var8, &var9, &var10, &var11, &var12, &var13, &var14, &var15, &var16, &var17);
  fscanf (meteolog2, "%f %f %f %f %f %f %f\n", &var1, &var2, &var3, &var4, &var5, &var6, &var7);
  fclose(meteolog);
  fclose(meteolog2);
  fprintf(streamlog, "%s %f %f %f\n","Environment: ",var6,var7,var9);
  fprintf(streamlog, "%s\n","");


  streameta = fopen("/srv/www/htdocs/trg_eta.txt","r");
  fscanf (meteolog, "%s\n",trgeta);
  fclose(streameta);

  streamstack = fopen("/srv/www/htdocs/stack.txt","r");
  fscanf (streamstack, "%s\n",stack);
  fclose(streamstack);

  streameta = fopen("/srv/www/htdocs/trg_gap.txt","r");
  fscanf (meteolog, "%s\n",layer);
  fclose(streameta);

  fprintf(streamlog, "%s %s %s %s\n","Eta_partition: ", trgeta, "Trigger_layer", layer);
  fprintf(streamlog, "%s\n","Information from cosmic stand [gap] [uA] [Vset] [Vmon] [station]");
  fprintf(streamlog, "%s %s\n","Stack ID: ",stack);
  fprintf(streamlog, "%s\n","");

  fscanf (streamconf, "%s\n", &temp1);
  fscanf (streamconf, "%s\n", &temp1);
  i=3;
 
while (!feof(streamconf)) {

     if (i > 2) {
            fscanf (streamconf, "%s %d %d %d\n", &chambername, &slot, &channel, &tdc);

        streamdump = fopen("/tmp/dump","r");
        while (!feof(streamdump)) {

            fscanf (streamdump, "%d %d %f %f %f %f %d %d\n", &slot_d, &channel_d, &hvset, &hvmon, &currset, &curr, &stat, &pwr);            
            hvset = (hvset * pressurezero /var9 ) / 10;
            hvset = int(hvset);
            hvset = hvset / 10;
            hvset = floor(hvset + 0.5);
            hvset = hvset * 100;
           
            if (slot_d == slot) {
                  if (channel_d == channel) {
                     //printf("%s %f %f %d\n",chambername, curr, hvmon, tdc);

                     //reading febs values...
                     febtdc=0;
                     streamfeb = fopen("/srv/www/htdocs/feb/input.txt","r");
                     while (!feof(streamfeb)) {
                        febtdc=febtdc+1;
                        fscanf (streamfeb, "%d %d %d %d %d %d\n", &achip1, &bchip1, &cchip1, &dchip1, &echip1, &fchip1);
                        fscanf (streamfeb, "%d %d %d %d %d %d\n", &achip2, &bchip2, &cchip2, &dchip2, &echip2, &fchip2);
                        fscanf (streamfeb, "%d %d %d %d %d %d\n", &achip3, &bchip3, &cchip3, &dchip3, &echip3, &fchip3);
                        fscanf (streamfeb, "%d %d %d %d %d %d\n", &achip4, &bchip4, &cchip4, &dchip4, &echip4, &fchip4);

                        fscanf (streamfeb, "%d %d %d %d %d %d\n", &vachip1, &vachip1, &vcchip1, &vdchip1, &vechip1, &vfchip1);
                        fscanf (streamfeb, "%d %d %d %d %d %d\n", &vachip2, &vbchip2, &vcchip2, &vdchip2, &vechip2, &vfchip2);
                        fscanf (streamfeb, "%d %d %d %d %d %d\n", &vachip3, &vbchip3, &vcchip3, &vdchip3, &vechip3, &vfchip3);
                        fscanf (streamfeb, "%d %d %d %d %d %d\n", &vachip4, &vbchip4, &vcchip4, &vdchip4, &vechip4, &vfchip4);

                        if (febtdc==tdc) {
                          fprintf(streamlog, "%s %4.2f %4.0f %4.0f %d\n",chambername, curr, hvset, hvmon, tdc);
                          verbosefeb=verbosefeb+1;
                        }
                        if (verbosefeb==3) {
                          fprintf(streamlog, "%s %d %d %d %d %d %d %d %d %d %d %d %d\n", "FEBs: ", achip1, achip2, achip3, achip4, bchip1, bchip2, bchip3, bchip4, cchip1, cchip2, cchip3, cchip4);
                           fprintf(streamlog, "%s\n","");
                           verbosefeb=0;
                        }
                     }
                     fclose(streamfeb);
                  }
                }
        }
        fclose(streamdump);
     }
  }
  fclose(streamconf);
  fclose(streamlog);

  strcat(execstring," -m /home/user/re4data/TDC/daqinfo");
  system(execstring);


  strcpy(execstring,"cp /home/user/re4data/TDC/daqinfo /home/user/re4data/TDC/");
  strcat(execstring, bufNameshort);
  strcat(execstring,".info");
  system(execstring);


  std::string sModInt = ini->String( "General", "Interface", "NONE" );
  if( sModInt == "NONE" ) { std::cerr << "No interface specified. Please fill General.Interface section with the name of the interface module.\n"; exit( -1 ); }
  fDataFile = TFile::Open( bufName, "RECREATE" );
  fDataTree = new TTree( "BSC_DATA_TREE", "BSC RAW DATA" );
  fModuleCount = ini->Int( "General", "Modules", 0 );

  for( int module_index = 1; module_index < fModuleCount + 1; ++module_index ){
    std::stringstream  modName;
    modName << "Module" << module_index;
    //std::string sName = modName.str()
    std::string modType = ini->String( modName.str(), "Type", "NONE" );
    if( modType != "NONE" ){
      std::cout << "Found " << modName.str() << std::endl;
      BSCModule * mod = NULL;
      mod = _modMap[ modType ]->Clone();
      mod->SetName( modName.str() ); //Temporary set the name to the ini file section
      if( modName.str() != sModInt ){ mod->SetVmeInterface( _vmeInt ); }


      std::cout << "Module " << mod << std::endl;
      mod->SetIniFile( ini );
      mod->SetTree( fDataTree );
      mod->Initialize();

      //cout << "INDEX: " << module_index << endl;
      if (module_index == 1) {
          //cout << "Bunch ID reset... " << endl;
          //sleep(10);
          //mod->Stop();
       }      
      _modules.push_back( mod );
      if( modName.str() == sModInt ){ _vmeInt = dynamic_cast<ModuleVmeInterface*>( mod ); }
    } else std::cerr << "Cannot find description for " << modName.str() << std::endl;

  }



  //VmeInt = new ModuleV2718();
  //VmeInt->SetBaseAddress( 0 );
  //VmeInt->Initialize();
  //Add number of fADCs to the Tree
  //fDataTree->Branch( "FADC_Count", &fFAdcCount, "fFAdcCount/I" );
  /*
  for( int fadc = 0; fadc < fFAdcCount; ++fadc ){
    std::stringstream modName;
    modName << "V1721_" << fadc;
    string sName = modName.str();
    std::cout << "Creating fADC from section [" << sName << "]..." ;
    BSCModuleV1721 * fADC = new BSCModuleV1721();
    fADC->SetBaseAddress( ( long ) ini.Int( sName, "BaseAddress", -1 ) );
    fADC->SetName( ini.String( sName, "Name", "FADC" ) );
    fADC->SetReadoutTimeout( (unsigned long) ini.Int( sName, "Timeout", 1000 ) ); //Readout timeout. Units in ms. 1000 = 1 sec
    fADC->SetBufferOrganization( ( unsigned ) ini.Int( sName, "BufferOrg", 5 ) );
    fADC->SetBufferSize( ( unsigned ) ini.Int( sName, "BufferSize", 12 ) );
    fADC->SetPostTrigger( ( unsigned ) ini.Int( sName, "PostTrigger", 48 ) );
    fADC->SetTriggerSource( ( unsigned ) ini.Int( sName, "TriggerSrc", 1 ) );
    fADC->SetChannels( ( unsigned ) ini.Int( sName, "Channels", 8 ) );
    fADC->SetThresholdLevel( ( unsigned ) ini.Int( sName, "Threshold", 50 ) );
    fADC->SetBoardId( ini.Int( sName, "BoardId", 0 ) );
    fADC->SetVmeLink( ini.Int( sName, "VmeLink", 0 ) );
    this->AddModule( fADC );
    std::cout << " Done." << std::endl;
  }

  for( unsigned mod = 0; mod < fModules.size(); ++mod ){
    fModules[ mod ]->SetTree( fDataTree );
    std::cout << "Going to initialize " << fModules[ mod ]->GetName() << std::endl;
    fModules[ mod ]->Initialize();
  }
  */
}

void BSCDataReader::SetNumberOfSpills( unsigned aSpills ){
  fSpillNum = aSpills;
  //int Spill10Percent = fSpillNum / 10;
}

void BSCDataReader::AddModule( BSCModule * aModule ){
  fModules.push_back( aModule );
}

void BSCDataReader::Run(){
  char execstring[ 100 ], temp[20];
  int run;

  cout << "Starting data acquisition. " << endl;
  for( unsigned mod = 0; mod < _modules.size(); ++mod ) _modules[ mod ]->Start();
  unsigned SpillCount = 0;
  fStopFlag = false;
  int Spill10Percent = fSpillNum / 10;
  while( !fStopFlag ){
    for( unsigned mod = 0; mod < _modules.size(); ++mod ) fStopFlag = fStopFlag || !_modules[ mod ]->Read();
        fDataTree->Fill();
        ++SpillCount;
        //if( SpillCount % Spill10Percent == 0 ) cout << "#";
        std::cout << "Event " << SpillCount << std::endl;
        if( fSpillNum <= SpillCount ) fStopFlag = true;
  }

  cout << endl << "Acquired " << SpillCount << " events. Stopping data acquisition." << endl;
  for( unsigned mod = 0; mod < _modules.size(); ++mod ) _modules[ mod ]->Stop();
  fDataTree->Write();
  fDataFile->Close();

  FILE *runnumberfile;
  runnumberfile = fopen("/home/user/re4data/TDC/run","r");

  while (!feof(runnumberfile)) {
       fscanf(runnumberfile, "%d\n", &run);
  }
  fclose(runnumberfile);
  sprintf(temp, "%d",run);
  strcpy(execstring,"/home/user/workspace/rpcupgrade/Offline/rpc_offline /home/user/re4data/TDC/");
  strcat(execstring, temp);
  strcat(execstring,".root > /dev/null 2>&1");
  //printf("%s\n", execstring);
  system(execstring);
}

void BSCDataReader::GetSkimmedData() {

	int pressurezero = 990;

	FILE *runnumberfile;
	runnumberfile = fopen("/home/user/re4data/TDC/run", "r");
	int run=0;
	while (!feof(runnumberfile)) {
		fscanf(runnumberfile, "%d\n", &run);
	}
	fclose(runnumberfile);

	FILE *meteolog;
	FILE *meteolog2;
	meteolog = fopen("/home/user/meteolog", "r");
	meteolog2 = fopen("/home/user/meteolog2", "r");

	float var1, var2, var3, var4, var5, var6, var7, var8, var9;
	float var10,var11,var12,var13,var14,var15,var16,var17;
	fscanf(meteolog, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			&var1, &var2, &var3, &var4, &var5, &var6, &var7, &var8, &var9,
			&var10, &var11, &var12, &var13, &var14, &var15, &var16, &var17);

	fscanf(meteolog2, "%f %f %f %f %f %f %f\n", &var1, &var2, &var3, &var4,
			&var5, &var6, &var7);
	fclose(meteolog);
	fclose(meteolog2);

	FILE *streamconf;
	char temp1[25];
	streamconf = fopen("conf.txt", "r");
	fscanf(streamconf, "%s\n", &temp1);
	fscanf(streamconf, "%s\n", &temp1);
	int i = 3;

//	char chambername[25];
//	int slot, channel, tdc;
	int slot_d, channel_d, stat, pwr, slot, channel, tdc, febtdc,
			verbosefeb = 0;
	float hvset, hvmon, currset, curr;
	char chambername[25], temp2[25];

	FILE *streamlog;
	streamlog = fopen("daqinfoRaman","w");

	FILE *streamdump;

	while (!feof(streamconf)) {

		if (i > 2) {
			fscanf(streamconf, "%s %d %d %d\n", &chambername, &slot, &channel,
					&tdc);

			streamdump = fopen("/tmp/dump", "r");
			while (!feof(streamdump)) {

				fscanf(streamdump, "%d %d %f %f %f %f %d %d\n", &slot_d,
						&channel_d, &hvset, &hvmon, &currset, &curr, &stat,
						&pwr);
				hvset = (hvset * pressurezero / var9) / 10;
				hvset = int(hvset);
				hvset = hvset / 10;
				hvset = floor(hvset + 0.5);
				hvset = hvset * 100;

				if (slot_d == slot) {
					if (channel_d == channel) {
						//fprintf(streamlog, "%d %s %4.2f %4.0f %4.0f %d\n",run,chambername, curr, hvset, hvmon, tdc);
						fprintf(streamlog, "%d %f %f %f %f %f\n",run, hvset, curr ,var6,var7, var9);
					}
				}
			}
			fclose(streamdump);


		}

	}
	fclose(streamconf);
	fclose(streamlog);

//	char *mvcommand="mv daqinforRaman ";
//	char *runInfo;
//	sprintf(runInfo,"%i%s",run,".txt");
//	char *finalMvCommand;
//	strcpy(finalMvCommand,mvCommand);
//	strcat(finalMvCommand,runInfo);
//	system(mvcommand);

}
