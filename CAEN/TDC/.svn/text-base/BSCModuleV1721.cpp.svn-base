#include "BSCModuleV1721.h"

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sstream>

using std::cout;
using std::cerr;
using std::endl;
using std::hex;
using std::dec;
using std::stringstream;
using std::flush;
using std::setw;
using std::setfill;

//unsigned V1721SamplesN = 98;

BSCModuleV1721::BSCModuleV1721(){
  fBaseAddr = -1;
  _vmeLink = 0;   // link 0 goes to ADC2 (BaseAddr 0x22220000)
  // link 1 goes to ADC3 (BaseAddr 0x33330000)
  _boardId = 0;
  fNRead = 0;
  fSamplesPerCh = 0;
  fChunkSize = 0;
  fHeader1 = fHeader2 = fHeader3 = fHeader4 = 0;
 
  //Set to defaults
  SetTriggerSource();
  SetBufferOrganization();
  SetBufferSize();
  SetPostTrigger();
  SetChannels();
  SetThresholdLevel();
}

int BSCModuleV1721::Initialize(){
  if( fBaseAddr > -1 ){
    int ret = 0;
    int counter = 0;
    cout << "Initializing Module " << fName << endl 
	 << "Type: V1721" << endl 
	 << "Base Address: 0x" << setw( 8 ) << setfill( '0' )<< hex <<  this->fBaseAddr << dec << endl 
	 << "VME Link: " << this->_vmeLink << endl
	 << "Board Id: " << this->_boardId << endl << endl;
    fADC = new ADC_V1721_CAEN( this->fBaseAddr, this->fName, this->_vmeLink, this->_boardId );
    //cout << "Software reset..." << endl;
    //fADC->SW_Reset(); //Perform Software Reset
   
    while( !fADC->Is_Board_Ready_For_Acquisition() ){
      sleep( 1 ); //sec
      cout << " Polling for ACQ ready  0x" << setw(2) << setfill( '0' ) << hex 
	   << fADC->Get_ACQ_Status() << dec << endl;
      ++counter;	
      if( counter > 1000 ){
	cout << "Could not reach ACQ ready in module " << this->fName << endl 
	     << "ABORT!" << endl;
	exit( -1 );
      }
    }
  
    fADC->Set_Count_ALL_Trg();          // count all triggers
    fADC->Set_FrontPanel_PatternMode(); // to see LVDS Pattern in header
    //Set the buffer organization => 32 blocks of 64Kb size each(64K samples/block)
    //fADC->Set_Buffer_Organisation( 5 );	
    fADC->Set_Buffer_Organisation( fBufOrg );
    //Set the size of the buffer; 12 * 8 = 96 samples => 192 ns
    fADC->Set_Custom_Size( fBufSize );
    // PostTrigger Samples 
    fADC->Set_Post_Trg( fPostTrig );                       //        "
    // One event per MBLT read
    fADC->Set_BLT_Event_Number( 1 );
    fADC->Enable_Ch( 8 );  //Enable all channels //TODO: Read from ini file
    usleep( SLEEP );
    //* * * Set Triggering Options for Self Triggering * * *
    if( fTrgSrc == 0 ){ //Self-trigger
      cout << "Disabling External Trigger Input... " << endl;
      ret = fADC->Disable_External_Trg_Src();
      if ( ret < 0 ) return ret;
      usleep( SLEEP );
      cout << "Enabling SW (Self) Trigger... " << endl;
      ret = fADC->Enable_SW_Trg_Src();
      if ( ret < 0 ) return ret;
      usleep( SLEEP );
      /*      
      cout << "Enable All Channels To Self Trigger..." << endl;
      ret = fADC->Enable_Ch_Trg_Src( 8 );
      */     
      cout << "Enable the First Channel as Trigger Source..." << endl;
      ret = fADC->Enable_Ch_Trg_Src( 1 );
      if ( ret < 0 ) return ret;
      usleep( SLEEP );
      cout << "Enabling Software Trigger Channel Sources... " << endl;
      ret = fADC->Enable_Ch_Trg_Src( fChannels );
      if ( ret < 0 ) return ret;
      usleep( SLEEP );
    } else if( fTrgSrc == 1 ){
      cout << "Disable Software Trigger Input" << endl;
      ret = fADC->Disable_SW_Trg_Src();
      if( ret < 0 ) return ret;
      usleep( SLEEP );
      cout << "Enabling External Trigger" << endl;
      ret = fADC->Enable_External_Trg_Src();
      if( ret < 0 ) return ret;
      usleep( SLEEP );
      ret = fADC->Enable_External_Trg_Out();
      if( ret < 0 ) return ret;
      usleep( SLEEP );
    }
    cout << "Setting Self Trigger Threshold Level To " << fThrLevel << " mV... " << endl;
    ret = fADC->Set_Ch_Trg_Threshold( 8, fThrLevel );
    if ( ret < 0 ) return ret;
    usleep( SLEEP );
    cout << "Local Coincedence Level is " << fADC->Get_LocalCoincidenceLevel() 
	 << flush << endl;
    //cout << "Setting Local Coincidence Level To 1... " << endl;
    //ret = fADC->Set_LocalCoincidenceLevel( 1 );
    if ( ret < 0 ) return ret;
    usleep( SLEEP );

    unsigned int FixCount = fADC->Get_BLT_Event_Number();
    unsigned CustomSize = fADC->Get_Custom_Size();

    if( CustomSize ){
      fNRead = 8 * 2 * CustomSize  + 4;
      fChunkSize = 1;
      while( fChunkSize * 2 < fNRead ) fChunkSize *= 2;
      cout << "Chunk Size = " << fChunkSize << endl;
    } else {
      unsigned int pot = 2 << ( fADC->Get_Buffer_Organisation() - 1 );
      fNRead =  FixCount * 4 * pot;
      fChunkSize = fNRead;
    }
    fSamplesPerCh =  ( ( fNRead - 4 ) / fChannels ) * 4;
    //fSamplesPerCh =  ( ( fNRead - 4 ) / fChannels );    
    cout << "Samples Per Channel : " << fSamplesPerCh << endl;
  } else { 
    cerr << "Base address is not set for module " << this->fName << endl; 
    return -100; 
  }
  // Calibrate
  //this->Calibrate();
  // Add branches to the main TTree
  if( fTree ){
    fData.resize( V1721Channels );// vector<int>( V1721SamplesN, 0 ) );
    unsigned ChNum = this->GetChannelNumber();
    stringstream ss;
    ss << fName << "_SAMPLES";
    //   TBranch * brSamples = 
    fTree->Branch( ss.str().c_str(), &fSamplesPerCh, "fSamplesPerCh/I" );
    ss.str("");
    ss << fName << "_HEADER1";
    fTree->Branch( ss.str().c_str(), &fHeader1, "fHeader1/I" );
    ss.str("");
    ss << fName << "_HEADER2";
    fTree->Branch( ss.str().c_str(), &fHeader2, "fHeader2/I" );
    ss.str("");
    ss << fName << "_HEADER3";
    fTree->Branch( ss.str().c_str(), &fHeader3, "fHeader3/I" );
    ss.str("");
    ss << fName << "_HEADER4";
    fTree->Branch( ss.str().c_str(), &fHeader4, "fHeader4/I" );
    //fBranches.push_back( brSamples );
    for( unsigned ch=0; ch < ChNum; ++ch ){
      ss.str( "" ); //Clear the stream
      ss << fName << "_CH" << ch;
      string chName = ss.str();
      ss.str( "" ); 
      ss << "fData["<< ch <<"]["<<fName<<"_SAMPLES]/I";
      string chVar = ss.str();
      //fData[ ch ] = new unsigned[ V1721SamplesN ];
      fData[ ch ].resize( fSamplesPerCh, 0 );
      //std::vector< unsigned > vec = &fData[ ch ];
      cout << "Creating Branch " << chName << " : " << chVar << endl;
      //TBranch *brChan = 
      fTree->Branch( chName.c_str(), &fData[ ch ] );
      //fBranches.push_back( brChan );
    }
  } else {
    cerr << "Tree is not set for module " << this->fName << endl;
    return -101;
  }
  return 0;
}

void BSCModuleV1721::Calibrate(){
  fADC->Do_ChCalibration();
}

void BSCModuleV1721::Start(){
  cout << " Start Acquisition Loop ... " << std::flush << endl;
  while( !fADC->Is_Board_Ready_For_Acquisition() ){
    fADC->SW_Clear();
  }
  fADC->Set_ACQ_RUN_START(); // Set start run
  usleep( 50000 );
  if( fADC->Get_Number_StoredEvents() > 0 ) fADC->SW_Clear();
}

bool BSCModuleV1721::Read(){
  bool STOPPING = false;
  bool flag = false;
  long ret = fADC->Wait_4_Event( &STOPPING, fReadoutTimeout );
  if( ret == 0 ){
    //flag = true;
  
    unsigned long StoredEvents = fADC->Get_Number_StoredEvents();
#ifdef DEBUG
    std::cout << "Found " << StoredEvents << " events stored in the buffer..." << std::endl;
#endif
    if( StoredEvents >= 1 ){
      flag = true;
      unsigned long data;
      fADC->Read_Event( &fHeader1 );
      //cout << "Header1 = " << fHeader1 << endl;
      unsigned long EventSize = ( 0xFFFFFFF & fHeader1 );
      //cout << fName << endl;
      if( EventSize > 4 ){
	fADC->Read_Event( &fHeader2 );
	fADC->Read_Event( &fHeader3 );
	fADC->Read_Event( &fHeader4 );
#ifdef DEBUG
	unsigned Geo = ( fHeader2 & 0xF8000000 ) >> 27;
	unsigned ZLE = ( fHeader2 & 0x01000000 ) >> 24;
	unsigned Pattern = ( fHeader2 & 0x00FFFF00 ) >> 8;	
        unsigned EventCounter = ( fHeader3 & 0x00FFFFFF );
	cout << "Geo = " << Geo << endl << "ZLE = " << ZLE << endl
	     << "Pattern = " << Pattern << endl << "Event counter = " << EventCounter << endl
	     << "Trigger Time Tag = " << fHeader4 << endl;
#endif

	EventSize -= 4;
	unsigned WordsPerChannel =  EventSize / fChannels; 
	fSamplesPerCh = WordsPerChannel * 4;
	const unsigned short SamplesPerWord = 4;
	fSamplesPerCh = WordsPerChannel * SamplesPerWord; 
	for( unsigned long i = 0; i < EventSize * SamplesPerWord; ++i ){
	  if( i % SamplesPerWord == 0 ) {
	    fADC->Read_Event( &data );
	  }
	  unsigned ch = i / fSamplesPerCh; 
	  unsigned chunk = i % SamplesPerWord;
	  unsigned short d = ( data >> chunk * 8 ) & 0xFF;                                                    
	  //cout << "ch = " << ch << " d = " << d << endl;
	  if( ch < 8 )fData[ ch ][ i - ch * fSamplesPerCh ] = d;
	}
      } else {
	cerr << "Event less then 4 words ( " << EventSize << " )" <<  endl;
      } 
    } else {
      cerr << "No events stored in " << fName << endl;
    } 

    fADC->SW_Clear(); 
  }
    /*  
    unsigned long * tmpData = new unsigned long[ fChunkSize ];
    vector< unsigned long > RawData( fNRead );
    memset( tmpData, fChunkSize * sizeof( unsigned long ), 0 );
    tmpData[ 0 ] = 5;

    unsigned long StoredEvents = fADC->Get_Number_StoredEvents();
    cout << "Stored Events = " << StoredEvents << endl;
  if( StoredEvents >= 1  ){_hitsPerChannel
      long ReadCount = fNRead;
      long ReadData = 0;
      while( ReadData < fNRead ){
	long Count = 0;
	Count = fADC->Read_MBLT64( tmpData, fChunkSize );
	cout << "Count = " << Count << endl;
	if( Count > 0 ){
	  unsigned N = ( ReadCount >= fChunkSize )? fChunkSize : ReadCount;
	  ReadCount -= fChunkSize;
	  cout << "N = " << N << " ReadCount = " << ReadCount << endl; 
	  for( unsigned i = 0; i < N; ++i ){
	    RawData[ ReadData + i ] = tmpData[ i ];
	  } 
	  ReadData += Count; 
	  
	  unsigned long Header1 = RawData[ 0 ];
	  unsigned long EventSize = ( 0xFFFFFFF & Header1 );
	  cout << "Event Size = " << EventSize << endl <<"Header1 = " << Header1 << endl;
	}
	cout << "ReadCount = " << ReadCount << endl << "ReadData = " << ReadData << endl;

      }
    }
    unsigned WordsPerChannel = ( fNRead - 4 ) / fChannels;
    for( unsigned words = 3; words < RawData.size(); ++words ){
      int ch = (words - 3) / WordsPerChannel;
      if( ch < 8 ){
	int localIndex = (words -3) - ch*WordsPerChannel;
	cout << "Channel = " << ch << ", words = " << words << ", localindex = " << localIndex <<  endl;
	unsigned long dt = RawData[ words ];
	cout << "[ " << words << " ]  = " << dt << endl; 
	//for( unsigned chunk = 0; chunk < 4; ++chunk ){
	//unsigned short d = ( dt >> chunk * 8 ) & 0xFF;
	//fData[ ch ][ localIndex + chunk ] = d;
	//cout << "[ " << localIndex + chunk << " ] = " << d << endl;
	fData[ ch ][ localIndex ] = dt;
	//}
      }
    }
    fADC->SW_Clear();
    cout << "Read event. " << endl << endl;
    delete tmpData;
    cout << "Deleted" << endl;
  } 
    */

 /* 
 //std::fill( &fData[ 0 ][ 0 ], 8 * V1721Samples, 0 ); 
  //cout << "waiting for event... " << endl;
  if( ret == 0 ){ // data available
    cout << " DATA AVAILABLE " << endl;
    //gettimeofday( &run_time, 0 );                   // Eventtime
    unsigned long EventSize = fADC->Get_Event_Size();
    unsigned long Nread = EventSize;
    cout << "EventSize = " << EventSize << endl;
//		int bytes = 0;
//		bool last = false;
    unsigned DataToRead = V1721SamplesN * 8;
    while( ( ret >= 0 ) && ( DataToRead > 0 )){ // read loop
      ret = fADC->Read_
      ret = fADC->Read_MBLT64( BLTData, Nread * sizeof( long ) );
      //if( ret <= 0 || last ) break;	
      // Copy Data
      DataToRead -= ret;
      cout << "Read " << ret << " words." << endl;
      if( ret > 0 ) //memcpy( &fData[ bytes ], BLTData, ret );
	{
	  for( unsigned ch = 0; ch < V1721Channels; ++ch )
	    for( unsigned s = 0; s < V1721SamplesN; ++s ){ 
	      fData[ ch ][ s ] = BLTData[ ch * V1721Channels + s ];
	    }	
	}
      fADC->SW_Clear();
    }	
  } else flag = false;
  */
  return flag;
}

bool BSCModuleV1721::Write(){
  return true;
}

void BSCModuleV1721::Stop(){
  cout << " Acquisition Loop stopped ... " << endl;
  fADC->Set_ACQ_RUN_STOP();
}
BSCModule * BSCModuleV1721::Clone(){ 
  std::cout << "V1721" << std::endl; 
  return new BSCModuleV1721();
}

