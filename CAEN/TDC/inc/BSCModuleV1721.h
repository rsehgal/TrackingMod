#ifndef _BSCMODULEV1721_H_
#define _BSCMODULEV1721_H_
#include <iostream>
#include <vector>

#include "BSCModule.h"
#include "ModuleVmeInterface.h"

#include "adc_v1721_caen.hh"

using std::vector;

const unsigned V1721Channels = 8;
const unsigned int SLEEP = 5000;

class BSCModuleV1721: public BSCModule, public ModuleVmeInterface {
 public:
  BSCModuleV1721();
  virtual ~BSCModuleV1721(){};
		
  unsigned GetChannelNumber(){ return V1721Channels; };
  void Start();	
  int Initialize();
  void Calibrate();
  bool Read();
  bool Write();
  void Stop();
  
  void SetTriggerSource( unsigned aTrigger = 0 ){ fTrgSrc = aTrigger; };
  void SetBufferOrganization( unsigned aBufOrg = 5 ){ fBufOrg = aBufOrg; };
  void SetBufferSize( unsigned aBufSize = 12 ){ fBufSize = aBufSize; };
  void SetPostTrigger( unsigned aPostTrig = 48 ){ fPostTrig = aPostTrig; };
  void SetChannels( unsigned aChannels = 8 ){ fChannels = aChannels; };
  void SetThresholdLevel( unsigned aThrLevel = 50 ){ fThrLevel = aThrLevel; };
  BSCModule * Clone();
  
  //TODO: Fill the following
  unsigned char ReadC( unsigned long aAddress ){ return 0; };
  unsigned short ReadW( unsigned long aAddress ){ return 0; };
  unsigned long ReadL( unsigned long aAddress ){ return 0; };

  void WriteC( unsigned long aAddress, unsigned char aData ){};
  void WriteW( unsigned long aAddress, unsigned short aData ){};
  void WriteL( unsigned long aAddress, unsigned long aData ){};
  long GetBoardId(){ return _boardId; };
 private:
  ADC_V1721_CAEN * fADC;
  vector< vector< unsigned > > fData;
  vector< TBranch * > fBranches;
  unsigned int TotalData[0x1000010];
  unsigned long BLTData[ 5000 ];    // long statt int !!
  unsigned fTrgSrc;
  unsigned fBufOrg;
  unsigned fBufSize;
  unsigned fPostTrig;
  unsigned fChannels;
  unsigned fThrLevel;
  unsigned fSamplesPerCh;
  unsigned fNRead;
  unsigned fChunkSize; 
  unsigned long fHeader1;
  unsigned long fHeader2;
  unsigned long fHeader3;
  unsigned long fHeader4;
};

#endif

