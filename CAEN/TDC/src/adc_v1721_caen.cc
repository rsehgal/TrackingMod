#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cmath>

#include "adc_v1721_caen.hh"

#define lred ""//"\033[1;49;31m"
#define norm ""//"\033[0m"

using namespace std;

/*  Help function 1 for constructor  */
void my_exit(int val){
  switch(val){
  case 0 :
    cerr << " [ERR:ADC_V1721_CAEN] Wrong board0 type!" << endl;
    cerr << lred << " Check also base address of the module or restart the VME crate!" << norm << endl;
    break;
  case 1 : 
    cerr << "\n Board Type Unknown! (non of CAEN V1721, VX1721, V1721B, VX1721B, V1731, VX1731, V1731B, VX1731B) " << endl;
    cerr << lred << " Check also base address of the module or restart the VME crate!" << norm << endl;
    break;
  case 2 :
    cerr << " [ERR:ADC_V1721_CAEN] Wrong board0 type!" << endl;
    cerr << lred << " Check also base address of the module or restart the VME crate!" << norm << endl;
    break;
  }
  exit(0);
}

/*  Help function 2 for constructor  */
void output(string bval){
  cout << "\n ==>  Found board type CAEN " << lred << bval << norm << endl;
  if (bval!="V1721")
    cout << lred << "\n  *** This software is tested for CAEN V1721 only *** " << norm << endl;
}


/* Help function for Print_Reg_Status()*/
void Print_Hex(unsigned long val, unsigned short bitmask){ // bitmask prints red 
  unsigned long dummy;
  cout << "0x";
  for(int i=7; i>=0; i--){
    dummy=(0xF << i*4);
    if( ((1 << i) & bitmask) != 0)
      cout << lred;
    cout << hex << ((dummy & val) >> 4*i) << dec << norm ;
  }
}




/*******************/
/*   Konstruktor   */
/*******************/
ADC_V1721_CAEN::ADC_V1721_CAEN(const unsigned long Base_Addr, string ADC_Name, short vme_link, short board_no){
  BaseAddr = Base_Addr;
  ADCName  = ADC_Name;

  cout << "\n---------------------------------------------------------------\n"<< endl;

 
  CVErrorCodes cv_error_code;
  cout << "vme_link " << vme_link << endl;
  cout << "board_no " << board_no << endl;
  cout << "BaseAddr " << Base_Addr << endl;
  if( ( cv_error_code= CAENVME_Init( cvV2718, vme_link, board_no, &this->m_handle.m_vme_handle))!= cvSuccess){
    cerr <<" [ERR:ADC_V1721_CAEN] No Caen VME controller found!\n " << CAENVME_DecodeError(cv_error_code) << endl;
    exit(-1);
  }

  cout << "\n---------------------------------------------------------------\n"<< endl;
  cout << " -> Initialization of " << ADC_Name << endl;
  cout << " -> Looking for " << ADC_Name << " at 0x" << hex << BaseAddr << dec << " ..." << endl;
  

  cout << " -> Reading the Configuration ROM ... " << endl;

  unsigned long Data=0;
  int ret=0;

  // vers
  ret = CAENVME_ReadCycle( this->m_handle.m_vme_handle, BaseAddr + CONFIGURATION_ROM + 0x30, &Data, cvA32_U_DATA, cvD32 ); // vers
  if(ret<0) {
    cerr << " [ERR:ADC_V1721_CAEN] No access to ADC at address 0x" << hex << BaseAddr << dec << endl;
    exit(0);
  }
  long vers = Data;
  ret = CAENVME_ReadCycle( this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+0x34,&Data, cvA32_U_DATA, cvD32); // board
  long board2 = Data;
  ret = CAENVME_ReadCycle( this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+0x3C,&Data, cvA32_U_DATA, cvD32); // board
  long board0 = Data;
  /*
  if(vers==0x20){       // type   (no B)
    if(board2==0x00){          // no X
      if(board0==0xb9){               // 21
	BoardType="V1721";
      }else if(board0==0xc3){         // 31
	BoardType="V1731";
      }else
	my_exit(0);
    }else if(board2==0x01){    // X
      if(board0==0xb9){               // 21
	BoardType="VX1721";
      }else if(board0==0xc3){         // 31
	BoardType="VX1731";
      }else
	my_exit(0);
    }else
      my_exit(2);
  }else if(vers==0x21){ // type B
    if(board2==0x00){          // no X
      if(board0==0xb9){               // 21
	BoardType="V1721B";
      }else if(board0==0xc3){         // 31
	BoardType="V1731B";
      }else
	my_exit(0);
    }else if(board2==0x01){    // X
      if(board0==0xb9){               // 21
	BoardType="VX1721B";
      }else if(board0==0xc3){         // 31
	BoardType="VX1731B";
      }else
	my_exit(0);
    }else
      my_exit(2);
  }else
    my_exit(1);

  output(BoardType);

  cout << "\n -> Reading further register entries ... " << endl;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+0x80,&Data, cvA32_U_DATA, cvD32); // serial number
  ConfigRom[20]=Data;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+0x84,&Data, cvA32_U_DATA, cvD32); // serial number
  ConfigRom[21]=Data;
  Data=0;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FIRMWARE_UPGRADE, &Data, cvA32_U_DATA, cvD32); // FW Upgrade number (2 for LHC Clock Upgrade)
  cout << "    Board Serial No!!! :   " << setw(8) << ConfigRom[20]*0x100+ConfigRom[21] << endl;
  cout << "    FW_Upgrade: (2 = Latest) " << Data << endl;
  for(int i=0; i<20; i++){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+i*0x4,&Data, cvA32_U_DATA, cvD32);
    ConfigRom[i]=Data;
  }
  
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ROC_FPGA_FIRMMWARE_REVISION,&Data, cvA32_U_DATA, cvD32);
  string mon[12] = {" Jan "," Feb "," Mar "," Apr "," May "," Jun "," Jul "," Aug "," Sep "," Oct "," Nov "," Dec"};
  ROC_FPGA_FW.revDate_year = ((Data & 0xF0000000)>>28);
  ROC_FPGA_FW.revDate_month = ((Data & 0xF000000)>>24);
  ROC_FPGA_FW.revDate_day = ((Data & 0xF00000)>>20)*10 + ((Data & 0xF0000)>>16);
  ROC_FPGA_FW.rev_X = ((Data & 0xFF00)>>8);
  ROC_FPGA_FW.rev_Y = (Data & 0xFF);
  cout << "    Firmware Rev.   :   " << ROC_FPGA_FW.revDate_day << "." << mon[ROC_FPGA_FW.revDate_month-1] ;
  cout << "2" << setfill('0') << setw(3) << ROC_FPGA_FW.revDate_year << setfill(' ');
  cout << ", v " << ROC_FPGA_FW.rev_X << "." << ROC_FPGA_FW.rev_Y << endl;

  for(int i=0; i<NCH; i++){
    CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_AMC_FPGA_FIRMWARE_REVISION+i*0x100,&Data, cvA32_U_DATA, cvD32);
    AMC_FPGA_FW[i].revDate_year = ((Data & 0xF0000000)>>28);
    AMC_FPGA_FW[i].revDate_month = ((Data & 0xF000000)>>24);
    AMC_FPGA_FW[i].revDate_day = ((Data & 0xF00000)>>20)*10 + ((Data & 0xF0000)>>16);
    AMC_FPGA_FW[i].rev_X = ((Data & 0xFF00)>>8);
    AMC_FPGA_FW[i].rev_Y = (Data & 0xFF);
  }
*/
  this->SW_Reset();
/*
  int counter = 0;
  while(!this->Is_Board_Ready_For_Acquisition()){
    sleep(1);
    cout << " Polling for ACQ ready" << endl;
    counter++;
    if(counter>1000){
      cerr << " Could not reach ACQ ready" << endl;
      exit(-1);
    }
      
  }
*/
  cout << "\n  ADC reset and ready to work! " << endl;
  cout << "\n---------------------------------------------------------------\n"<< endl;
}



/*******************/
/*   Konstruktor   */
/*******************/
ADC_V1721_CAEN::ADC_V1721_CAEN(const unsigned long Base_Addr, string ADC_Name, int caen_device_handle){
  BaseAddr = Base_Addr;
  ADCName  = ADC_Name;

  cout << "\n---------------------------------------------------------------\n"<< endl;

  this->m_handle.m_vme_handle = (int32_t)caen_device_handle;


  cout << "\n---------------------------------------------------------------\n"<< endl;
  cout << " -> Initialization of " << ADC_Name << endl;
  cout << " -> Looking for " << ADC_Name << " at 0x" << hex << BaseAddr << dec << " ..." << endl;
  

  cout << " -> Reading the Configuration ROM ... " << endl;

  unsigned long Data=0;
  int ret=0;

  // vers
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+0x30,&Data, cvA32_U_DATA, cvD32); // vers
  if(ret<0) {
    cout << " [ERR:ADC_V1721_CAEN] No access to ADC at address 0x" << hex << BaseAddr << dec << endl;
    exit(0);
  }
  long vers = Data;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+0x34,&Data, cvA32_U_DATA, cvD32); // board
  long board2 = Data;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+0x3C,&Data, cvA32_U_DATA, cvD32); // board
  long board0 = Data;

  if(vers==0x20){       // type   (no B)
    if(board2==0x00){          // no X
      if(board0==0xb9){               // 21
	BoardType="V1721";
      }else if(board0==0xc3){         // 31
	BoardType="V1731";
      }else
	my_exit(0);
    }else if(board2==0x01){    // X
      if(board0==0xb9){               // 21
	BoardType="VX1721";
      }else if(board0==0xc3){         // 31
	BoardType="VX1731";
      }else
	my_exit(0);
    }else
      my_exit(2);
  }else if(vers==0x21){ // type B
    if(board2==0x00){          // no X
      if(board0==0xb9){               // 21
	BoardType="V1721B";
      }else if(board0==0xc3){         // 31
	BoardType="V1731B";
      }else
	my_exit(0);
    }else if(board2==0x01){    // X
      if(board0==0xb9){               // 21
	BoardType="VX1721B";
      }else if(board0==0xc3){         // 31
	BoardType="VX1731B";
      }else
	my_exit(0);
    }else
      my_exit(2);
  }else
    my_exit(1);

  output(BoardType);

  cout << "\n -> Reading further register entries ... " << endl;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+0x80,&Data, cvA32_U_DATA, cvD32); // serial number
  ConfigRom[20]=Data;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+0x84,&Data, cvA32_U_DATA, cvD32); // serial number
  ConfigRom[21]=Data;
  cout << "    Board Serial No :   " << setw(8) << ConfigRom[20]*0x100+ConfigRom[21] << endl;
  
  for(int i=0; i<20; i++){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CONFIGURATION_ROM+i*0x4,&Data, cvA32_U_DATA, cvD32);
    ConfigRom[i]=Data;
  }
  
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ROC_FPGA_FIRMMWARE_REVISION,&Data, cvA32_U_DATA, cvD32);
  string mon[12] = {" Jan "," Feb "," Mar "," Apr "," May "," Jun "," Jul "," Aug "," Sep "," Oct "," Nov "," Dec"};
  ROC_FPGA_FW.revDate_year = ((Data & 0xF0000000)>>28);
  ROC_FPGA_FW.revDate_month = ((Data & 0xF000000)>>24);
  ROC_FPGA_FW.revDate_day = ((Data & 0xF00000)>>20)*10 + ((Data & 0xF0000)>>16);
  ROC_FPGA_FW.rev_X = ((Data & 0xFF00)>>8);
  ROC_FPGA_FW.rev_Y = (Data & 0xFF);
  cout << "    Firmware Rev.   :   " << ROC_FPGA_FW.revDate_day << "." << mon[ROC_FPGA_FW.revDate_month-1] ;
  cout << "2" << setfill('0') << setw(3) << ROC_FPGA_FW.revDate_year << setfill(' ');
  cout << ", v " << ROC_FPGA_FW.rev_X << "." << ROC_FPGA_FW.rev_Y << endl;

  for(int i=0; i<NCH; i++){
    CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_AMC_FPGA_FIRMWARE_REVISION+i*0x100,&Data, cvA32_U_DATA, cvD32);
    AMC_FPGA_FW[i].revDate_year = ((Data & 0xF0000000)>>28);
    AMC_FPGA_FW[i].revDate_month = ((Data & 0xF000000)>>24);
    AMC_FPGA_FW[i].revDate_day = ((Data & 0xF00000)>>20)*10 + ((Data & 0xF0000)>>16);
    AMC_FPGA_FW[i].rev_X = ((Data & 0xFF00)>>8);
    AMC_FPGA_FW[i].rev_Y = (Data & 0xFF);
  }

  this->SW_Reset();
/*
  int counter = 0;
  while(!this->Is_Board_Ready_For_Acquisition()){
    sleep(1);
    cout << " Polling for ACQ ready" << endl;
    counter++;
    if(counter>1000){
      cerr << " Could not reach ACQ ready" << endl;
      exit(-1);
    }
      
  }
*/
  cout << "\n  ADC reset and ready to work! " << endl;
  cout << "\n---------------------------------------------------------------\n"<< endl;
}


/********************/
/* Destrcutor       */
/********************/
ADC_V1721_CAEN::~ADC_V1721_CAEN(){
  CAENVME_End(this->m_handle.m_vme_handle);
}



// ---------------- Set Functions --------------------------




/********************************************************************/
/*    int ADC_V1721_CAEN::SW_Reset()                                */
/********************************************************************/
int ADC_V1721_CAEN::SW_Reset(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] SW_Reset" << endl;
#endif
  unsigned long Data=0;
  int ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+SW_RESET, (void*)&Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    cerr << " [ERR:ADC_V1721_CAEN] Software Reset failed!! " << endl;
  return ret;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::SW_Clear()                                */
/********************************************************************/
int ADC_V1721_CAEN::SW_Clear(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] SW_Clear" << endl;
#endif
  unsigned long Data=0;
  int ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+SW_CLEAR, (void*)&Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    cerr << " [ERR:ADC_V1721_CAEN] Software Clear failed!! " << endl;
  return 0;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Config_Reload()                           */
/********************************************************************/
int ADC_V1721_CAEN::Config_Reload(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Config_Reload" << endl;
#endif
  unsigned long Data=0;
  int ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+CONFIGURATION_RELOAD, (void*)&Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN] Configuration Reload failed!! " << endl;
    return ret;
  }

  usleep(100000); // wait a bit for the flash memories

  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ROC_FPGA_FIRMMWARE_REVISION,&Data, cvA32_U_DATA, cvD32);
  ROC_FPGA_FW.revDate_year = ((Data & 0xF0000000)>>28);
  ROC_FPGA_FW.revDate_month = ((Data & 0xF000000)>>24);
  ROC_FPGA_FW.revDate_day = ((Data & 0xF00000)>>20)*10 + ((Data & 0xF0000)>>16);
  ROC_FPGA_FW.rev_X = ((Data & 0xFF00)>>8);
  ROC_FPGA_FW.rev_Y = (Data & 0xFF);

  for(int i=0; i<NCH; i++){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_AMC_FPGA_FIRMWARE_REVISION+i*0x100,&Data, cvA32_U_DATA, cvD32);
    AMC_FPGA_FW[i].revDate_year = ((Data & 0xF0000000)>>28);
    AMC_FPGA_FW[i].revDate_month = ((Data & 0xF000000)>>24);
    AMC_FPGA_FW[i].revDate_day = ((Data & 0xF00000)>>20)*10 + ((Data & 0xF0000)>>16);
    AMC_FPGA_FW[i].rev_X = ((Data & 0xFF00)>>8);
    AMC_FPGA_FW[i].rev_Y = (Data & 0xFF);
  }

  return 0;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Do_ChCalibration()                        */
/********************************************************************/
int ADC_V1721_CAEN::Do_ChCalibration(){
  cout << " Channel Calibration" << endl;
  unsigned long Data=0x2;
  int ret;

  for(int i=0; i<NCH; i++){
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_ADC_CONFIGURATION+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
    if(ret < 0){
      cerr << " [ERR:ADC_V1721_CAEN] Channel Calibration failed" << endl;
      return ret;
    }
  }

  Data&=0x0;
  for(int i=0; i<NCH; i++){
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_ADC_CONFIGURATION+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
    if(ret < 0){
      cerr << " [ERR:ADC_V1721_CAEN] Channel Calibration failed" << endl;
      return ret;
    }
  }

  int bla=0, dummy, cnt=0;
  while(bla==0){
    bla=1;
    cout << setw(7) << cnt++ << "   " ;
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+i*0x100,&Data, cvA32_U_DATA, cvD32);
      if(ret < 0){
	cerr << " [ERR:ADC_V1721_CAEN] Channel Calibration failed" << endl;
	return ret;
      }
      dummy = ((Data & 0x40)>>6);
      cout << " Ch " << i << " : " << dummy << "   ";
      bla*=dummy;
    }
    cout << endl;
  }
  cout << " Channel Calibration was successful" << endl;
  
  return 0;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Ch_Trg_Threshold()  "self-trigger"    */
/********************************************************************/
int ADC_V1721_CAEN::Set_Ch_Trg_Threshold(int channel, unsigned int val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Ch_Trg_Threshold" << endl;
#endif
  if(channel<0 || channel>NCH)
    return -1;

  unsigned long Data=(unsigned long)val;
  int ret;
  if(channel<NCH){   // set threshold for the specified channel only
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_THRESHOLD+channel*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
  }else{    // set threshold for all the channels (channel==8)
    for(int i=0; i<NCH; i++){
      ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_THRESHOLD+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
    }
  }
#ifdef DEBUG_
  cerr << "                      Ch_" << channel << "_THRESHOLD : 0x" << hex << Data << dec << endl;
#endif
  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_DATANUMBER_OvrUndr_Thres()  "self-trigger" */
/********************************************************************/
int ADC_V1721_CAEN::Set_DATANUMBER_OvrUndr_Thres(int channel, unsigned int val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_DATANUMBER_OvrUndr_Thres" << endl;
#endif
  if(channel<0 || channel>NCH)
    return -1;

  unsigned long Data=(unsigned long)val;
  int ret;
  if(channel<NCH){   // set time over/under threshold for the specified channel only
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_TIME_OVERUNDER_THRESHOLD+channel*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
  }else{    // set time over/under threshold for all the channels (channel==8)
    for(int i=0; i<NCH; i++){
      ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_TIME_OVERUNDER_THRESHOLD+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
    }
  }
#ifdef DEBUG_
  cerr << "                      Ch_" << channel << "_TIME_OVERUNDER_THRESHOLD : 0x" << hex << Data << dec << endl;
#endif
  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Ch_DAC()                              */
/********************************************************************/
int ADC_V1721_CAEN::Set_Ch_DAC(int channel, unsigned int val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Ch_DAC" << endl;
#endif
  if(channel<0 || channel>NCH)
    return -1;

  unsigned long Data=(unsigned long)val;
  int ret;
  if(channel<NCH){
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_DAC+channel*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_DAC+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
    }
  }
  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_Ch_Trg_Src()                       */
/********************************************************************/
int ADC_V1721_CAEN::Enable_Ch_Trg_Src(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_Ch_Trg_Src" << endl;
#endif
  if(channel<0 || channel>NCH)
    return -1;
  unsigned long Data;
 
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  if(channel<NCH){
    Data |= (1 << channel);
  }else{
    Data |= 0xFF;
  }
#ifdef DEBUG_
  cerr << "                      Trigger Source Enable Mask : 0x" << hex << Data << dec << endl;
#endif
  ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_Ch_Trg_Src()                      */
/********************************************************************/
int ADC_V1721_CAEN::Disable_Ch_Trg_Src(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_Ch_Trg_Src" << endl;
#endif
  if(channel<0 || channel>NCH)
    return -1;
  unsigned long Data;
 
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  if(channel<NCH){
    Data &= ~(1 << channel);
  }else{
    Data &= ~0xFF;
  }
  ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_Ch_Trg_Out()                       */
/********************************************************************/
int ADC_V1721_CAEN::Enable_Ch_Trg_Out(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_Ch_Trg_Out" << endl;
#endif
  if(channel<0 || channel>NCH)
    return -1;
  unsigned long Data;
 
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  if(channel<NCH){
    Data |= (1 << channel);
  }else{
    Data |= 0xFF;
  }
  ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_Ch_Trg_Out()                      */
/********************************************************************/
int ADC_V1721_CAEN::Disable_Ch_Trg_Out(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_Ch_Trg_Out" << endl;
#endif
  if(channel<0 || channel>NCH)
    return -1;
  unsigned long Data;
 
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  if(channel<NCH){
    Data &= ~(1 << channel);
  }else{
    Data &= ~0xFF;
  }
  ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_Ch()                               */
/********************************************************************/
int ADC_V1721_CAEN::Enable_Ch(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_Ch" << endl;
#endif
  if(channel<0 || channel>NCH)
    return -1;
  unsigned long Data;
 
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CHANNEL_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  if(channel<NCH){
    Data |= (1 << channel);
  }else{
    Data |= 0xFF;
  }
#ifdef DEBUG_
  cerr << "                      Channel Mask : 0x" << hex << Data << dec << endl;
#endif
  ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+CHANNEL_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_Ch()                              */
/********************************************************************/
int ADC_V1721_CAEN::Disable_Ch(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_Ch" << endl;
#endif
  if(channel<0 || channel>NCH)
    return -1;
  unsigned long Data;
 
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CHANNEL_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;

  if(channel<NCH){
    Data &= ~(1 << channel);
  }else{
    Data &= ~0xFF;
    cerr << " [Warning:ADC_V1721_CAEN-Disable_Ch] All Channels are disabled!!" << endl;
  }
  ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+CHANNEL_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;
#ifdef DEBUG_
  cerr << "                      Channel Mask : 0x" << hex << Data << dec << endl;
#endif

  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Ch_ZS_Logik_OVER_Threshold()          */
/********************************************************************/
int ADC_V1721_CAEN::Set_Ch_ZS_Logik_OVER_Threshold(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Ch_ZS_Logik_OVER_Threshold" << endl;
#endif
  if(this->Is_ZS_Switched_Off())
    cerr << " [Warning:ADC_V1721_CAEN] Setting Logik has no action without ZS Mode! " << endl;

  if(channel<0 || channel>NCH)
    return -1;
  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_ZS_THRES+channel*0x100,&Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
    Data &= 0x7FFFFFFF;
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_ZS_THRES+channel*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_ZS_THRES+i*0x100,&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
      Data &= 0x7FFFFFFF;
      ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_ZS_THRES+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
    }
  }
  return 0;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Ch_ZS_Logik_UNDER_Threshold()        */
/********************************************************************/
int ADC_V1721_CAEN::Set_Ch_ZS_Logik_UNDER_Threshold(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Ch_ZS_Logik_UNDER_Threshold" << endl;
#endif
  if(this->Is_ZS_Switched_Off())
    cerr << " [Warning:ADC_V1721_CAEN] Setting Logik has no action without ZS Mode! " << endl;

  if(channel<0 || channel>NCH)
    return -1;
  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_ZS_THRES+channel*0x100,&Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
    Data |= 0x80000000;
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_ZS_THRES+channel*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_ZS_THRES+i*0x100,&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
      Data |= 0x80000000;
      ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_ZS_THRES+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
    }
  }
  return 0;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Ch_ZS_Compare_Value()                 */
/********************************************************************/
int ADC_V1721_CAEN::Set_Ch_ZS_Compare_Value(int channel, unsigned int val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Ch_ZS_Compare_Value" << endl;
#endif
  if(this->Is_ZS_Switched_Off())
    cerr << " [Warning:ADC_V1721_CAEN] Setting compare value has no action without ZS Mode! " << endl;

  if(channel<0 || channel>NCH)
    return -1;
  if(val<0 || val>0xFF)
    return -2;

  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_ZS_THRES+channel*0x100,&Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
    Data &= 0x80000000;
    Data |= (unsigned long)val;
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_ZS_THRES+channel*0x100, (void*)&Data, cvA32_U_DATA, cvD32);    
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_ZS_THRES+i*0x100,&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
      Data &= 0x80000000;
      Data |= (unsigned long)val;
      ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_ZS_THRES+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
    }
  }
  return 0;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Ch_ZSAMP_DataNumber()                 */
/********************************************************************/
int ADC_V1721_CAEN::Set_Ch_ZSAMP_DataNumber(int channel, unsigned long val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Ch_ZSAMP_DataNumber" << endl;
#endif
  if(!this->Is_ZS_AMP_Mode()){
    cerr << " [ERR:ADC_V1721_CAEN] Zero Suppression Mode is not ZS_AMP! " << endl;
    return -3;
  }

  if(channel<0 || channel>NCH)
    return -1;

  unsigned long Data=val;
  int ret;
  if(channel<NCH){
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_NS_AMP+channel*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_NS_AMP+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
    }
  }
  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Ch_ZLE_N_LBK()                        */
/********************************************************************/
int ADC_V1721_CAEN::Set_Ch_ZLE_N_LBK(int channel, unsigned long val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Ch_ZLE_N_LBK" << endl;
#endif
  if(!this->Is_ZS_ZLE_Mode()){
    cerr << " [ERR:ADC_V1721_CAEN] Zero Suppression Mode is not ZLE! " << endl;
    return -3;
  }

  if(channel<0 || channel>NCH)
    return -1;
  if(val<0 || val>0xFFFF)
    return -2;

  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_NS_AMP+channel*0x100, &Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
    Data &= ~0xFFFF;
    Data |= val;
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_NS_AMP+channel*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_NS_AMP+i*0x100, &Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
      Data &= ~0xFFFF;
      Data |= val;
      ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_NS_AMP+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
    }
  }
  return 0;  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Ch_ZLE_N_LFWD()                       */
/********************************************************************/
int ADC_V1721_CAEN::Set_Ch_ZLE_N_LFWD(int channel, unsigned long val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Ch_ZLE_N_LFWD" << endl;
#endif
  if(!this->Is_ZS_ZLE_Mode()){
    cerr << " [ERR:ADC_V1721_CAEN] Zero Suppression Mode is not ZLE! " << endl;
    return -3;
  }

  if(channel<0 || channel>NCH)
    return -1;
  if(val<0 || val>0xFFFF)
    return -2;

  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_NS_AMP+channel*0x100, &Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
    Data &= 0xFFFF;
    Data |= (val << 16);
    ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_NS_AMP+channel*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
    if (ret<0)
      return ret;
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_NS_AMP+i*0x100, &Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
      Data &= 0xFFFF;
      Data |= (val << 16);
      ret = CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_0_NS_AMP+i*0x100, (void*)&Data, cvA32_U_DATA, cvD32);
      if (ret<0)
	return ret;
    }
  }
  return 0;  
}



/* General Settings */
/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_ZeroSuppression_Mode()            */
/********************************************************************/
int ADC_V1721_CAEN::Disable_ZeroSuppression_Mode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_ZeroSuppression_Mode" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_CONFIGURATION, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;
  Data &= ~0xF0000;
#ifdef DEBUG_
  cerr << "                      Ch_CONFIGURATION : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_CONFIGURATION, (void*)&Data, cvA32_U_DATA, cvD32);
}




/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_ZeroLengthEncoding_Mode()          */
/********************************************************************/
int ADC_V1721_CAEN::Enable_ZeroLengthEncoding_Mode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_ZeroLengthEncoding_Mode" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_CONFIGURATION, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;
  Data &= ~0xF0000;
  Data |= 0x20000;
#ifdef DEBUG_
  cerr << "                      Ch_CONFIGURATION : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_CONFIGURATION, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_ZeroSuppressionAmplitude_Mode()    */
/********************************************************************/
int ADC_V1721_CAEN::Enable_ZeroSuppressionAmplitude_Mode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_ZeroSuppressionAmplitude_Mode" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_CONFIGURATION, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return ret;
  Data &= ~0xF0000;
  Data |= 0x30000;
#ifdef DEBUG_
  cerr << "                      Ch_CONFIGURATION : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_CONFIGURATION, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_TRG_OUT_OVER_Threshold()              */
/********************************************************************/
int ADC_V1721_CAEN::Set_TRG_OUT_OVER_Threshold(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_TRG_OUT_OVER_Threshold" << endl;
#endif

  unsigned long Data=0x40;
#ifdef DEBUG_
  cerr << "                      Ch_CONFIGURATION_BIT_CLEAR : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_CONFIGURATION_BIT_CLEAR, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_TRG_OUT_UNDER_Threshold()             */
/********************************************************************/
int ADC_V1721_CAEN::Set_TRG_OUT_UNDER_Threshold(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_TRG_OUT_UNDER_Threshold" << endl;
#endif

  unsigned long Data=0x40;
#ifdef DEBUG_
  cerr << "                      Ch_CONFIGURATION_BIT_SET : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_CONFIGURATION_BIT_SET, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_MemoryAccess_SEQUENTIAL()             */
/********************************************************************/
int ADC_V1721_CAEN::Set_MemoryAccess_SEQUENTIAL(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_MemoryAccess_SEQUENTIAL" << endl;
#endif

  unsigned long Data=0x10;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_CONFIGURATION_BIT_SET, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_TestPattern_Generator()            */
/********************************************************************/
int ADC_V1721_CAEN::Enable_TestPattern_Generator(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_TestPattern_Generator" << endl;
#endif

  unsigned long Data=0x8;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_CONFIGURATION_BIT_SET, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_TestPattern_Generator()           */
/********************************************************************/
int ADC_V1721_CAEN::Disable_TestPattern_Generator(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_TestPattern_Generator" << endl;
#endif

  unsigned long Data=0x8;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_CONFIGURATION_BIT_CLEAR, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_TrgOverlapp()                      */
/********************************************************************/
int ADC_V1721_CAEN::Enable_TrgOverlapp(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_TrgOverlapp" << endl;
#endif

  unsigned long Data=0x2;
#ifdef DEBUG_
  cerr << "                      Ch_CONFIGURATION_BIT_SET : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_CONFIGURATION_BIT_SET, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_TrgOverlapp()                     */
/********************************************************************/
int ADC_V1721_CAEN::Disable_TrgOverlapp(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_TrgOverlapp" << endl;
#endif

  unsigned long Data=0x2;
#ifdef DEBUG_
  cerr << "                      Ch_CONFIGURATION_BIT_CLEAR : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+Ch_CONFIGURATION_BIT_CLEAR, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Buffer_Organisation()                 */
/********************************************************************/
int ADC_V1721_CAEN::Set_Buffer_Organisation(unsigned int code){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Buffer_Organisation" << endl;
#endif
  if(code<0 || code>0xa){
    cerr << " [ERR:ADC_V1721_CAEN-Set_Buffer_Organisation] Specified value is outside Buffer Code range [0:10]!" << endl;
    return -1;
  }

  if(this->Is_ACQ_Running()){
    cerr << " [ERR:ADC_V1721_CAEN-Set_Buffer_Organisation] Buffer Code must not be changed in Acquisition Run Mode!" << endl;
    return -1;
  }
#ifdef DEBUG_
  cerr << "                      BUFFER_ORGANISATION : 0x" << hex << code << dec << endl;
#endif

  unsigned long Data = (unsigned long)code;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+BUFFER_ORGANISATION, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Buffer_Free()                         */
/********************************************************************/
int ADC_V1721_CAEN::Set_Buffer_Free(unsigned int val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Buffer_Free" << endl;
#endif
  unsigned long Data = (unsigned long)val;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+BUFFER_FREE, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Custom_Size()                         */
/********************************************************************/
int ADC_V1721_CAEN::Set_Custom_Size(unsigned long val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Custom_Size" << endl;
#endif
  if(this->Is_ACQ_Running()){
    cerr << " [ERR:ADC_V1721_CAEN-Set_Custom_Size] Custom Size must not be changed in Acquisition Run Mode!" << endl;
    return -1;
  }

#ifdef DEBUG_
  cerr << "                      CUSTOM_SIZE : 0x" << hex << val << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+CUSTOM_SIZE, (void*)&val, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Count_ALL_Trg()                       */
/********************************************************************/
int ADC_V1721_CAEN::Set_Count_ALL_Trg(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Count_ALL_Trg" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if(ret<0)
    return ret;
  Data |= 0x8;
#ifdef DEBUG_
  cerr << "                      ACQUISITION_CONTROL : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+ACQUISITION_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Count_ACCEPTED_Trg()                  */
/********************************************************************/
int ADC_V1721_CAEN::Set_Count_ACCEPTED_Trg(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Count_ACCEPTED_Trg" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if(ret<0)
    return ret;
  Data &= ~0x8;
#ifdef DEBUG_
  cerr << "                      ACQUISITION_CONTROL : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+ACQUISITION_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_ACQ_RUN_START()                       */
/********************************************************************/
int ADC_V1721_CAEN::Set_ACQ_RUN_START(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_ACQ_RUN_START" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data |= 0x4;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+ACQUISITION_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}




/********************************************************************/
/*    int ADC_V1721_CAEN::Set_ACQ_RUN_STOP()                        */
/********************************************************************/
int ADC_V1721_CAEN::Set_ACQ_RUN_STOP(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_ACQ_RUN_STOP" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data &= ~0x4;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+ACQUISITION_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Trg_Mode_RegisterControlled()         */
/********************************************************************/
int ADC_V1721_CAEN::Set_Trg_Mode_RegisterControlled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Trg_Mode_RegisterControlled" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data &= ~0x3;
#ifdef DEBUG_
  cerr << "                      Acquisition Control : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+ACQUISITION_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Trg_Mode_SIN_Controlled()              */
/********************************************************************/
int ADC_V1721_CAEN::Set_Trg_Mode_SIN_Controlled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Trg_Mode_SIN_Controlled" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data &= ~0x3;
  Data |= 0x1;
#ifdef DEBUG_
  cerr << "                      Acquisition Control : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+ACQUISITION_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Trg_Mode_SIN_Gate()                    */
/********************************************************************/
int ADC_V1721_CAEN::Set_Trg_Mode_SIN_Gate(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Trg_Mode_SIN_Gate" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data &= ~0x3;
  Data |= 0x2;
#ifdef DEBUG_
  cerr << "                      Acquisition Control : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+ACQUISITION_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Trg_Mode_MultiboardSync()             */
/********************************************************************/
int ADC_V1721_CAEN::Set_Trg_Mode_MultiboardSync(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Trg_Mode_MultiboardSync" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data |= 0x3;
#ifdef DEBUG_
  cerr << "                      Acquisition Control : 0x" << hex << Data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+ACQUISITION_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_SW_Trg()                              */
/********************************************************************/
int ADC_V1721_CAEN::Set_SW_Trg(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_SW_Trg" << endl;
#endif
  unsigned long Data = 0x1;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+SOFTWARE_TRIGGER, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_SW_Trg_Src()                       */
/********************************************************************/
int ADC_V1721_CAEN::Enable_SW_Trg_Src(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_SW_Trg_Src" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data |= 0x80000000;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_SW_Trg_Src()                      */
/********************************************************************/
int ADC_V1721_CAEN::Disable_SW_Trg_Src(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_SW_Trg_Src" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data &= ~0x80000000;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_External_Trg_Src()                 */
/********************************************************************/
int ADC_V1721_CAEN::Enable_External_Trg_Src(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_External_Trg_Src" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data |= 0x40000000;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_External_Trg_Src()                */
/********************************************************************/
int ADC_V1721_CAEN::Disable_External_Trg_Src(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_External_Trg_Src" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data &= ~0x40000000;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);  
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_LocalCoincidenceLevel()                      */
/********************************************************************/
int ADC_V1721_CAEN::Set_LocalCoincidenceLevel(unsigned int val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_LocalCoincidenceLevel" << endl;
#endif

  if(val<0 || val>7)
    return -1;

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data &= ~0x7000000;
  Data |= ((unsigned long)val << 24);
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_SW_Trg_Out()                       */
/********************************************************************/
int ADC_V1721_CAEN::Enable_SW_Trg_Out(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_SW_Trg_Out" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data |= 0x80000000;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_SW_Trg_Out()                      */
/********************************************************************/
int ADC_V1721_CAEN::Disable_SW_Trg_Out(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_SW_Trg_Out" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data &= ~0x80000000;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_External_Trg_Out()                 */
/********************************************************************/
int ADC_V1721_CAEN::Enable_External_Trg_Out(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_External_Trg_Out" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data |= 0x40000000;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_External_Trg_Out()                */
/********************************************************************/
int ADC_V1721_CAEN::Disable_External_Trg_Out(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_External_Trg_Out" << endl;
#endif

  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK,&Data, cvA32_U_DATA, cvD32);
  if(ret<0) 
    return ret;
  Data &= ~0x40000000;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Post_Trg()                            */
/********************************************************************/
int ADC_V1721_CAEN::Set_Post_Trg(unsigned long val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Post_Trg" << endl;
#endif

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+POST_TRIGGER_SETTING, (void*)&val, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_FrontPanel_IO_Data()                  */
/********************************************************************/
int ADC_V1721_CAEN::Set_FrontPanel_IO_Data(unsigned int val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_FrontPanel_IO_Data" << endl;
#endif
  unsigned long Data = (unsigned long)val;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_DATA, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_FrontPanel_IO_Normal_OP()             */
/********************************************************************/
int ADC_V1721_CAEN::Set_FrontPanel_IO_Normal_OP(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_FrontPanel_IO_Normal_OP" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0x8000;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_FrontPanel_IO_TestMode()              */
/********************************************************************/
int ADC_V1721_CAEN::Set_FrontPanel_IO_TestMode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_FrontPanel_IO_TestMode" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data |= 0x8000;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_FrontPanel_TestMode_TrgOut_One()      */
/********************************************************************/
int ADC_V1721_CAEN::Set_FrontPanel_TestMode_TrgOut_One(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_FrontPanel_TestMode_TrgOut_One" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data |= 0x4000;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_FrontPanel_TestMode_TrgOut_Zero()     */
/********************************************************************/
int ADC_V1721_CAEN::Set_FrontPanel_TestMode_TrgOut_Zero(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_FrontPanel_TestMode_TrgOut_Zero" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0x4000;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_FrontPanel_GeneralPurposeIO()         */
/********************************************************************/
int ADC_V1721_CAEN::Set_FrontPanel_GeneralPurposeIO(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_FrontPanel_GeneralPurposeIO" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0xC0;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_FrontPanel_ProgrammedIO()             */
/********************************************************************/
int ADC_V1721_CAEN::Set_FrontPanel_ProgrammedIO(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_FrontPanel_ProgrammedIO" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0xC0;
  Data |= 0x40;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_FrontPanel_PatternMode()              */
/********************************************************************/
int ADC_V1721_CAEN::Set_FrontPanel_PatternMode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_FrontPanel_PatternMode" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0xC0;
  Data |= 0x80;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);

  cerr << "0x"<< hex << Data << dec << endl;
  Data=0;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  cerr << "0x"<< hex << Data << dec << endl;


  return ret;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_LVDS_IO_15_12_Input()                 */
/********************************************************************/
int ADC_V1721_CAEN::Set_LVDS_IO_15_12_Input(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_LVDS_IO_15_12_Input" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0x20;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_LVDS_IO_15_12_Output()                 */
/********************************************************************/
int ADC_V1721_CAEN::Set_LVDS_IO_15_12_Output(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_LVDS_IO_15_12_Output" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data |= 0x20;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_LVDS_IO_11_8_Input()                  */
/********************************************************************/
int ADC_V1721_CAEN::Set_LVDS_IO_11_8_Input(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_LVDS_IO_11_8_Input" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0x10;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_LVDS_IO_11_8_Output()                 */
/********************************************************************/
int ADC_V1721_CAEN::Set_LVDS_IO_11_8_Output(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_LVDS_IO_11_8_Output" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data |= 0x10;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_LVDS_IO_7_4_Input()                   */
/********************************************************************/
int ADC_V1721_CAEN::Set_LVDS_IO_7_4_Input(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_LVDS_IO_7_4_Input" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0x8;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_LVDS_IO_7_4_Output()                  */
/********************************************************************/
int ADC_V1721_CAEN::Set_LVDS_IO_7_4_Output(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_LVDS_IO_7_4_Output" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data |= 0x8;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_LVDS_IO_3_0_Input()                   */
/********************************************************************/
int ADC_V1721_CAEN::Set_LVDS_IO_3_0_Input(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_LVDS_IO_3_0_Input" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0x4;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_LVDS_IO_3_0_Output()                  */
/********************************************************************/
int ADC_V1721_CAEN::Set_LVDS_IO_3_0_Output(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_LVDS_IO_3_0_Output" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data |= 0x4;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_PanelOutSignals()                  */
/********************************************************************/
int ADC_V1721_CAEN::Enable_PanelOutSignals(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_PanelOutSignals" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0x2;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::EnableHighImpedance_PanelOutSignals()     */
/********************************************************************/
int ADC_V1721_CAEN::EnableHighImpedance_PanelOutSignals(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] EnableHighImpedance_PanelOutSignals" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data |= 0x2;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_TRG_CLK_NIM()                         */
/********************************************************************/
int ADC_V1721_CAEN::Set_TRG_CLK_NIM(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_TRG_CLK_NIM" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data &= ~0x1;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_TRG_CLK_TTL()                         */
/********************************************************************/
int ADC_V1721_CAEN::Set_TRG_CLK_TTL(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_TRG_CLK_TTL" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if( ret < 0)
    return ret;
  Data |= 0x1;

  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FRONT_PANEL_IO_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Monitor_DAC()                         */
/********************************************************************/
int ADC_V1721_CAEN::Set_Monitor_DAC(unsigned int val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Monitor_DAC" << endl;
#endif
  unsigned long Data = (unsigned long)val;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+SET_MONITOR_DAC, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_MonitorMode_Majority()                */
/********************************************************************/
int ADC_V1721_CAEN::Set_MonitorMode_Majority(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_MonitorMode_Majority" << endl;
#endif
  unsigned long Data = 0x0;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+MONITOR_MODE, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_MonitorMode_WaveformGenerator()       */
/********************************************************************/
int ADC_V1721_CAEN::Set_MonitorMode_WaveformGenerator(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_MonitorMode_WaveformGenerator" << endl;
#endif
  unsigned long Data = 0x1;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+MONITOR_MODE, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_MonitorMode_BufferOccupancy()         */
/********************************************************************/
int ADC_V1721_CAEN::Set_MonitorMode_BufferOccupancy(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_MonitorMode_BufferOccupancy" << endl;
#endif
  unsigned long Data = 0x3;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+MONITOR_MODE, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_MonitorMode_VoltageLevel()            */
/********************************************************************/
int ADC_V1721_CAEN::Set_MonitorMode_VoltageLevel(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_MonitorMode_VoltageLevel" << endl;
#endif
  unsigned long Data=0x4;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+MONITOR_MODE, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_IRQMode_RORA()                        */
/********************************************************************/
int ADC_V1721_CAEN::Set_IRQMode_RORA(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_IRQMode_RORA" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data &= ~0x80;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_IRQMode_ROAK()                        */
/********************************************************************/
int ADC_V1721_CAEN::Set_IRQMode_ROAK(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_IRQMode_ROAK" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data |= 0x80;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_RELOC()                            */
/********************************************************************/
int ADC_V1721_CAEN::Enable_RELOC(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_RELOC" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data |= 0x40;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_RELOC()                           */
/********************************************************************/
int ADC_V1721_CAEN::Disable_RELOC(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_RELOC" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data &= ~0x40;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_ALIGN64()                          */
/********************************************************************/
int ADC_V1721_CAEN::Enable_ALIGN64(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_ALIGN64" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data |= 0x20;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_ALIGN64()                         */
/********************************************************************/
int ADC_V1721_CAEN::Disable_ALIGN64(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_ALIGN64" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data &= ~0x20;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_BERR()                             */
/********************************************************************/
int ADC_V1721_CAEN::Enable_BERR(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_BERR" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data |= 0x10;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_BERR()                            */
/********************************************************************/
int ADC_V1721_CAEN::Disable_BERR(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_BERR" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data &= ~0x10;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_OpticalLinkIRQ()                   */
/********************************************************************/
int ADC_V1721_CAEN::Enable_OpticalLinkIRQ(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_OpticalLinkIRQ" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data |= 0x8;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_OpticalLinkIRQ()                  */
/********************************************************************/
int ADC_V1721_CAEN::Disable_OpticalLinkIRQ(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_OpticalLinkIRQ" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data &= ~0x8;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_IRQ_Level()                           */
/********************************************************************/
int ADC_V1721_CAEN::Set_IRQ_Level(unsigned int level){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_IRQ_Level" << endl;
#endif
  if( level>7 )
    return -1;
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret < 0)
    return ret;
  Data &= ~0x7;
  Data |= (unsigned long)level;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+VME_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_BoardID()                             */
/********************************************************************/
int ADC_V1721_CAEN::Set_BoardID(unsigned int GEOAddr){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_BoardID" << endl;
#endif
  unsigned long Data = (unsigned long) GEOAddr;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+BOARD_ID, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_MCST_CBLT_Addr()                      */
/********************************************************************/
int ADC_V1721_CAEN::Set_MCST_CBLT_Addr(unsigned int Addr){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_MCST_CBLT_Addr" << endl;
#endif
  if(Addr > 0xFF)
    return -1;
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MULTICAST_BASEADDR_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0)
    return ret;
  Data &= ~0xFF;
  Data |= (unsigned long)Addr;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+MULTICAST_BASEADDR_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Board_disabled()                      */
/********************************************************************/
int ADC_V1721_CAEN::Set_Board_disabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Board_disabled" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MULTICAST_BASEADDR_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0)
    return ret;
  Data &= ~0x300;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+MULTICAST_BASEADDR_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Board_last()                          */
/********************************************************************/
int ADC_V1721_CAEN::Set_Board_last(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Board_last" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MULTICAST_BASEADDR_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0)
    return ret;
  Data &= ~0x300;
  Data |= 0x100;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+MULTICAST_BASEADDR_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Board_first()                         */
/********************************************************************/
int ADC_V1721_CAEN::Set_Board_first(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Board_first" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MULTICAST_BASEADDR_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0)
    return ret;
  Data &= ~0x300;
  Data |= 0x200;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+MULTICAST_BASEADDR_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Board_intermediate()                  */
/********************************************************************/
int ADC_V1721_CAEN::Set_Board_intermediate(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Board_intermediate" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MULTICAST_BASEADDR_CONTROL,&Data, cvA32_U_DATA, cvD32);
  if(ret<0)
    return ret;
  Data |= 0x300;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+MULTICAST_BASEADDR_CONTROL, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_Reloc_Address()                       */
/********************************************************************/
int ADC_V1721_CAEN::Set_Reloc_Address(unsigned int Addr){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_Reloc_Address" << endl;
#endif
  unsigned long Data = (unsigned long)Addr;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+RELOCATION_ADDRESS, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_IRQ_StatusID()                        */
/********************************************************************/
int ADC_V1721_CAEN::Set_IRQ_StatusID(unsigned long StatusID){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_IRQ_StatusID" << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+INTERRUPT_STATUS_ID, (void*)&StatusID, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_IRQ_Event_Number()                    */
/********************************************************************/
int ADC_V1721_CAEN::Set_IRQ_Event_Number(unsigned int val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_IRQ_Event_Number" << endl;
#endif
  unsigned long Data = (unsigned long)val &0x3FF;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+INTERRUPT_EVENT_NUMBER, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Set_BLT_Event_Number()                    */
/********************************************************************/
int ADC_V1721_CAEN::Set_BLT_Event_Number(unsigned int val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Set_BLT_Event_Number" << endl;
  cerr << "                      BLT_EVENT_NUMBER : 0x" << hex << val << dec << endl;
#endif
  unsigned long Data = (unsigned long)val & 0xFF;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+BLT_EVENT_NUMBER, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Write_to_Scratch()                        */
/********************************************************************/
int ADC_V1721_CAEN::Write_to_Scratch(unsigned long val){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Write_to_Scratch" << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+SCRATCH, (void*)&val, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_Flash()                            */
/********************************************************************/
int ADC_V1721_CAEN::Enable_Flash(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Enable_Flash" << endl;
#endif
  unsigned long Data = 0;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FLASH_ENABLE, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_Flash()                           */
/********************************************************************/
int ADC_V1721_CAEN::Disable_Flash(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Disable_Flash" << endl;
#endif
  unsigned long Data = 1;
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FLASH_ENABLE, (void*)&Data, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Write_to_FlashData()                      */
/********************************************************************/
int ADC_V1721_CAEN::Write_to_FlashData(unsigned long data){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Write_to_FlashData  0x" << hex << data << dec << endl;
#endif
  return CAENVME_WriteCycle(this->m_handle.m_vme_handle,BaseAddr+FLASH_DATA, (void*)&data, cvA32_U_DATA, cvD32);
}



//-----------------------  Get and Status Functions ------------------------


/********************************************************************/
/*    unsigned long ADC_V1721_CAEN::Get_ACQ_Status()                */
/********************************************************************/
unsigned long ADC_V1721_CAEN::Get_ACQ_Status(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_ACQ_Status" << endl;
#endif
#ifdef DUMMY_
  return 0;
#else
  unsigned long Data=0;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_STATUS, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return 0;
  return Data;
#endif
}



/********************************************************************/
/*    unsigned long ADC_V1721_CAEN::Get_VME_Status()                */
/********************************************************************/
unsigned long ADC_V1721_CAEN::Get_VME_Status(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_VME_Status" << endl;
#endif
#ifdef DUMMY_
  return 0;
#else
  unsigned long Data=0;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_STATUS, &Data, cvA32_U_DATA, cvD32);
  if (ret<0)
    return 0;
  return Data;
#endif
}




/********************************************************************/
/*    void ADC_V1721_CAEN::Print_Reg_Status()                       */
/********************************************************************/
void ADC_V1721_CAEN::Print_Reg_Status(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Print_Reg_Status" << endl;
#endif
  unsigned long nbr=0;
  int NN=3;


  // Zero Suppression
  if(Is_ZS_Switched_Off()){
    cout << setw(NN) << nbr++ << ". Zero Suppression switched off" << endl;
  }else{
    if(this->Is_Ch_Logik_OverThreshold(8)){
      cout << setw(NN)<< nbr++ << ". ZS: all channels over threshold" << endl;
    }else{
      for(unsigned int i=0; i<NCH; i++)
	if(this->Is_Ch_Logik_OverThreshold(i)){
	  cout << setw(NN)<< nbr++ << ". ZS: ch " << i << " over threshold" << endl;
	}else{
	  cout << setw(NN)<< nbr++ << ". ZS: ch " << i << " under threshold" << endl;
	}
    }
    for(unsigned int i=0; i<NCH; i++)
      cout << setw(NN) << nbr++ << ". ZS: ch " << i << " compare value : " << this->Get_Ch_ZS_Compare_Value(i) << endl;
    if(this->Is_ZS_ZLE_Mode()){
      cout << setw(NN) << nbr++ << ". Zero Suppression in ZLE Mode" << endl;
      for(unsigned int i=0; i<NCH; i++){
	cout << setw(NN) << nbr++ << ". ZS_ZLE: ch " << i << " N_LBK : " << this->Get_Ch_ZLE_N_LBK(i);
	cout << "     N_LFWD : " << this->Get_Ch_ZLE_N_LFWD(i) << endl;
      }
    }
    if(this->Is_ZS_AMP_Mode()){
      cout << setw(NN) << nbr++ << ". Zero Suppression in ZS AMP Mode" << endl;
      for(unsigned int i=0; i<NCH; i++)
	cout << setw(NN) << nbr++ << ". ZS_AMP: ch " << i << " Data Number (Nth) : " << this->Get_Ch_ZSAMP_DataNumber(i) << endl;
    }
  }

  // Trigger Stuff
  if(this->Is_Trg_Overlapp_Enabled()){
    cout << setw(NN) << nbr++ << ". Trg: Trigger overlapp enabled\n";    
  }else{
    cout << setw(NN) << nbr++ << ". Trg: Trigger overlapp disabled\n";    
  }
  if(this->Is_Count_All_Trg()){
    cout << setw(NN) << nbr++ << ". Trg: Count all Trigger\n";    
  }else{
    cout << setw(NN) << nbr++ << ". Trg: Count only accepted Trigger\n";    
  }
  for(unsigned int i=0; i<NCH; i++){
    cout << setw(NN) << nbr++ << ". Trg: ch " << i << " Trg Thres : " << this->Get_Ch_Trg_Threshold(i) << endl;
    cout << setw(NN) << nbr++ << ". Trg: ch " << i << " DataNumber o/u Thres : " << this->Get_DATANUMBER_OvrUndr_Thres(i) << endl;
  }

  if(this->Is_SW_Trg_Src_Enabled())
    cout << setw(NN) << nbr++ << ". Trg: Software Trigger as Trigger Source enabled\n";    
  else
    cout << setw(NN) << nbr++ << ". Trg: Software Trigger as Trigger Source disabled\n";    

  if(this->Is_Ext_Trg_Src_Enabled())
    cout << setw(NN) << nbr++ << ". Trg: External Trigger as Trigger Source enabled\n";   
  else
    cout << setw(NN) << nbr++ << ". Trg: External Trigger as Trigger Source disabled\n";

  for(unsigned int i=0; i<NCH; i++)
    if(this->Is_Ch_Trg_Src_Enabled(i))
      cout << setw(NN) << nbr++ << ". Trg: ch " << i << " as Trigger Source enabled\n";
    else
      cout << setw(NN) << nbr++ << ". Trg: ch " << i << " as Trigger Source disabled\n";

  cout << setw(NN) << nbr++ << ". Trg: Local Coincidence Level : " << Get_LocalCoincidenceLevel() << endl;

  if(this->Is_SW_Trg_Out_Enabled())
    cout << setw(NN) << nbr++ << ". Trg: Software Trigger for Front Panel output enabled\n";    
  else
    cout << setw(NN) << nbr++ << ". Trg: Software Trigger for Front Panel output disabled\n";    

  if(this->Is_Ext_Trg_Out_Enabled())
    cout << setw(NN) << nbr++ << ". Trg: External Trigger for Front Panel output enabled\n";   
  else
    cout << setw(NN) << nbr++ << ". Trg: External Trigger for Front Panel output disabled\n";

  for(unsigned int i=0; i<NCH; i++)
    if(this->Is_Ch_Trg_Out_Enabled(i))
      cout << setw(NN) << nbr++ << ". Trg: ch " << i << " for Front Panel output enabled\n";
    else
      cout << setw(NN) << nbr++ << ". Trg: ch " << i << " for Front Panel output disabled\n";
  
  


  // further Channel Settings
  for(unsigned int i=0; i<NCH; i++)
    cout << setw(NN) << nbr++ << ". Channel : Get_Ch_DAC ch " << i << " DC-Offset : " << this->Get_Ch_DAC(i) << endl;
  for(unsigned int i=0; i<NCH; i++)
    if(this->Is_Ch_Enabled(i))
      cout << setw(NN) << nbr++ << ". Channel : ch " << i << " enabled" << endl;
    else
      cout << setw(NN) << nbr++ << ". Channel : ch " << i << " disabled" << endl;
       
   



  // General stuff
  if(this->Is_RegisterControlled_Mode())
    cout << setw(NN) << nbr++ << ". ACQ : Acquisition Mode is Register Controlled" << endl;
  if(this->Is_SIN_Controlled_Mode())
    cout << setw(NN) << nbr++ << ". ACQ : Acquisition Mode is S-IN Controlled" << endl;
  if(this->Is_SIN_Gate_Mode())
    cout << setw(NN) << nbr++ << ". ACQ : Acquisition Mode is S-IN Gate" << endl;
  if(this->Is_MultiboardSync_Mode())
    cout << setw(NN) << nbr++ << ". ACQ : Acquisition Mode is Multiboard Synchronization" << endl;

  if(this->Is_Bypass_Mode())
    cout << setw(NN) << nbr++ << ". ACQ : Clock in Bypass Mode\n";
  else
    cout << setw(NN) << nbr++ << ". ACQ : Clock in PLL Mode\n";
  if(this->Is_ClockSrc_Internal())
    cout << setw(NN) << nbr++ << ". ACQ : Clock source is internal\n";
  else
    cout << setw(NN) << nbr++ << ". ACQ : Clock source is external\n";

  if(this->Is_TestPatternGenerator_Enabled()){
    cout << setw(NN) << nbr++ << ". Test Pattern Generator enabled\n";
  }else{
    cout << setw(NN) << nbr++ << ". Test Pattern Generator disabled\n";
  }
  cout << setw(NN) << nbr++ << ". Buffer : Buffer code : " << this->Get_Buffer_Organisation() << endl;
  cout << setw(NN) << nbr++ << ". Buffer : Buffer Free : " << this->Get_Buffer_Free() << endl;
  cout << setw(NN) << nbr++ << ". Buffer : Custom Size : " << this->Get_Custom_Size() << endl;
  cout << setw(NN) << nbr++ << ". Buffer : Post Trigger Value : " << this->Get_Post_Trg() << endl;
  cout << setw(NN) << nbr++ << ". Buffer : BLT Event Number : " << this->Get_BLT_Event_Number() << endl;

  cout << setw(NN) << nbr++ << ". Front Panel : Front Panel IO Data : " << this->Get_FrontPanel_IO_Data() << endl;
  
  if(this->Is_IO_Operation_TestMode()){
    cout << setw(NN) << nbr++ << ". Front Panel : IO Operation in Test Mode\n";
    if(this->Is_TrgOut_TestMode_Set_to_One())
      cout << setw(NN) << nbr++ << ". Front Panel : Trg Out Set to One\n";
    else
      cout << setw(NN) << nbr++ << ". Front Panel : Trg Out Set to Zero\n";
  }else{
    cout << setw(NN) << nbr++ << ". Front Panel : IO Operation in Normal Mode\n";
  }
  if(this->Is_IO_GeneralPurpose())
    cout << setw(NN) << nbr++ << ". Front Panel : IO General Purpose" << endl;
  if(this->Is_IO_Programmed())
    cout << setw(NN) << nbr++ << ". Front Panel : IO Programmed" << endl;
  if(this->Is_IO_PatternMode())
    cout << setw(NN) << nbr++ << ". Front Panel : IO Pattern Mode" << endl;

  if(this->Is_LVDS_IO_15_12_Input())
    cout << setw(NN) << nbr++ << ". Front Panel : LVDS IO 15..12 are Inputs" << endl;
  else
    cout << setw(NN) << nbr++ << ". Front Panel : LVDS IO 15..12 are Outputs" << endl;

  if(this->Is_LVDS_IO_11_8_Input())
    cout << setw(NN) << nbr++ << ". Front Panel : LVDS IO 11..8 are Inputs" << endl;
  else
    cout << setw(NN) << nbr++ << ". Front Panel : LVDS IO 11..8 are Outputs" << endl;

  if(this->Is_LVDS_IO_7_4_Input())
    cout << setw(NN) << nbr++ << ". Front Panel : LVDS IO 7..4 are Inputs" << endl;
  else
    cout << setw(NN) << nbr++ << ". Front Panel : LVDS IO 7..4 are Outputs" << endl;

  if(this->Is_LVDS_IO_3_0_Input())
    cout << setw(NN) << nbr++ << ". Front Panel : LVDS IO 3..0 are Inputs" << endl;
  else
    cout << setw(NN) << nbr++ << ". Front Panel : LVDS IO 3..0 are Outputs" << endl;

  if(this->Is_PanelOutSignal_in_high_Impedance())
    cout << setw(NN) << nbr++ << ". Front Panel : Panel Output Signals in High Impedance" << endl;

  if(this->Is_TRG_CLK_NIM())
    cout << setw(NN) << nbr++ << ". Front Panel : Clock and Trigger are NIM" << endl;
  else
    cout << setw(NN) << nbr++ << ". Front Panel : Clock and Trigger are TTL" << endl;


  // Monitor stuff
  cout << setw(NN) << nbr++ << ". Monitor : Monitor DAC : " << this->Get_Monitor_DAC() << endl;
  if(this->Is_MonitorMode_Majority())
    cout << setw(NN) << nbr++ << ". Monitor : Monitor Mode Majority " << endl;
  if(this->Is_MonitorMode_WaveformGenerator())
    cout << setw(NN) << nbr++ << ". Monitor : Monitor Mode Waveform Cenerator " << endl;
  if(this->Is_MonitorMode_BufferOccupancy())
    cout << setw(NN) << nbr++ << ". Monitor : Monitor Mode Buffer Occupancy " << endl;
  if(this->Is_MonitorMode_VoltageLevel())
    cout << setw(NN) << nbr++ << ". Monitor : Monitor Mode Voltage Level " << endl;

  // VME Stuff
  if(this->Is_IRQMode_RORA())
    cout << setw(NN) << nbr++ << ". VME : IRQ Mode : RORA " << endl;
  else
    cout << setw(NN) << nbr++ << ". VME : IRQ Mode : ROAK " << endl;

  if(this->Is_RELOC_Enabled()){
    cout << setw(NN) << nbr++ << ". VME : Relocation enabled " << endl;
    cout << setw(NN) << nbr++ << ". VME : Relocation Address : " << this->Get_Reloc_Address() << endl;
  }else
    cout << setw(NN) << nbr++ << ". VME : Relocation disabled " << endl;

  if(this->Is_ALIGN64_Enabled())
    cout << setw(NN) << nbr++ << ". VME : ALIGN64 enabled " << endl;
  else
    cout << setw(NN) << nbr++ << ". VME : ALIGN64 disabled " << endl;

  if(this->Is_BERR_Enabled())
    cout << setw(NN) << nbr++ << ". VME : BERR enabled " << endl;
  else
    cout << setw(NN) << nbr++ << ". VME : BERR disabled " << endl;

  if(this->Is_OpticalLinkIRQ_Enabled())
    cout << setw(NN) << nbr++ << ". VME : IRQ via optical Link enabled " << endl;
  else
    cout << setw(NN) << nbr++ << ". VME : IRQ via optical Link disabled " << endl;
  
  if(this->Get_IRQ_Level()==0)
    cout << setw(NN) << nbr++ << ". VME : IRQ disabled " << endl;
  else{
    cout << setw(NN) << nbr++ << ". VME : IRQ Level : " << this->Get_IRQ_Level() << endl;
    cout << setw(NN) << nbr++ << ". VME : IRQ Status ID : " << this->Get_IRQ_StatusID() << endl;
    cout << setw(NN) << nbr++ << ". VME : IRQ Event Number : " << this->Get_IRQ_Event_Number() << endl;
  }

  cout << setw(NN) << nbr++ << ". Board : BoardID : " << this->Get_BoardID() << endl;
  cout << setw(NN) << nbr++ << ". Board : MCST/CBLT Address : " << this->Get_MCST_CBLT_Addr() << endl;
  if(this->Is_Board_disabled())
    cout << setw(NN) << nbr++ << ". Board : board is disabled" << endl;
  if(this->Is_Board_last())
    cout << setw(NN) << nbr++ << ". Board : board is last board" << endl;
  if(this->Is_Board_first())
    cout << setw(NN) << nbr++ << ". Board : board is first board" << endl;
  if(this->Is_Board_intermediate())
    cout << setw(NN) << nbr++ << ". Board : board is intermediate board" << endl;
 
  if(this->Is_Flash_Enabled())
    cout << setw(NN) << nbr++ << ". Flash Memory : Flash Access is enabled" << endl;
  else
    cout << setw(NN) << nbr++ << ". Flash Memory : Flash Access is disabled" << endl;
}



/********************************************************************/
/*    void ADC_V1721_CAEN::Print_ROC_FPGA_Firmware_Rev()            */
/********************************************************************/
void ADC_V1721_CAEN::Print_ROC_FPGA_Firmware_Rev(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Print_ROC_FPGA_Firmware_Rev" << endl;
#endif
  string mon[12] = {" Jan "," Feb "," Mar "," Apr "," May "," Jun "," Jul "," Aug "," Sep "," Oct "," Nov "," Dec"};
  cout << "  ROC FPGA Firmware Rev.   :   " << ROC_FPGA_FW.revDate_day << "." << mon[ROC_FPGA_FW.revDate_month-1] ;
  cout << "2" << setfill('0') << setw(3) << ROC_FPGA_FW.revDate_year << setfill(' ');
  cout << ", v " << ROC_FPGA_FW.rev_X << "." << ROC_FPGA_FW.rev_Y << endl;
}



/********************************************************************/
/*    void ADC_V1721_CAEN::Print_AMC_FPGA_Firmware_Rev()            */
/********************************************************************/
void ADC_V1721_CAEN::Print_AMC_FPGA_Firmware_Rev(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Print_AMC_FPGA_Firmware_Rev" << endl;
#endif
  string mon[12] = {" Jan "," Feb "," Mar "," Apr "," May "," Jun "," Jul "," Aug "," Sep "," Oct "," Nov "," Dec"};
  for(int i=0; i<NCH; i++){
    cout << "  AMC FPGA Firmware Rev.(Ch " << i << ") :   " << AMC_FPGA_FW[i].revDate_day << "." << mon[AMC_FPGA_FW[i].revDate_month-1] ;
    cout << "2" << setfill('0') << setw(3) << AMC_FPGA_FW[i].revDate_year << setfill(' ');
    cout << ", v " << AMC_FPGA_FW[i].rev_X << "." << AMC_FPGA_FW[i].rev_Y << endl;
  }
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Ch_Logik_OverThreshold()              */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Ch_Logik_OverThreshold(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Ch_Logik_OverThreshold" << endl;
#endif
  if(channel<0 || channel>NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Ch_Logik_OverThreshold] Channel out of range [0:8]!" << endl;
    return false;
  }
  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_ZS_THRES+channel*0x100, &Data, cvA32_U_DATA, cvD32);
    if (ret<0){
      cerr << " [ERR:ADC_V1721_CAEN-Is_Ch_Logik_OverThreshold] Read Error!" << endl;
      return false;
    }
    if((Data & 0x80000000) != 0 )
      return false;
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_ZS_THRES+i*0x100, &Data, cvA32_U_DATA, cvD32);
      if (ret<0){
	cerr << " [ERR:ADC_V1721_CAEN-Is_Ch_Logik_OverThreshold] Read Error!" << endl;
	return false;
      }
      if((Data & 0x80000000) != 0 )
	return false;
    }
  }
  return true;
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_Ch_ZS_Compare_Value()      */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_Ch_ZS_Compare_Value(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Ch_ZS_Compare_Value" << endl;
#endif
  if(channel<0 || channel>7){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_ZS_Compare_Value] Channel out of range [0:7]!" << endl;
    return 0;
  }
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_ZS_THRES+channel*0x100, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_ZS_Compare_Value] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(0xFF & Data);
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_Ch_Trg_Threshold()         */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_Ch_Trg_Threshold(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Ch_Trg_Threshold" << endl;
#endif
  if(channel<0 || channel>=NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_Trg_Threshold] Channel out of range [0:7]!" << endl;
    return 0;
  }
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_THRESHOLD+channel*0x100, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_Trg_Threshold] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(0xFF & Data);
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_DATANUMBER_OvrUndr_Thres()   */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_DATANUMBER_OvrUndr_Thres(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_DATANUMBER_OvrUndr_Thres" << endl;
#endif
  if(channel<0 || channel>=NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Get_DATANUMBER_OvrUndr_Thres] Channel out of range [0:7]!" << endl;
    return 0;
  }
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_TIME_OVERUNDER_THRESHOLD+channel*0x100, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_DATANUMBER_OvrUndr_Thres] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(0xFFF & Data);
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_Ch_DAC()                     */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_Ch_DAC(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Ch_DAC" << endl;
#endif
  if(channel<0 || channel>7){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_DAC] Channel out of range [0:7]!" << endl;
    return 0;
  }
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_DAC+channel*0x100, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_DAC] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(0xFFFF & Data);
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_ZS_ZLE_Mode()                         */
/********************************************************************/
bool ADC_V1721_CAEN::Is_ZS_ZLE_Mode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_ZS_ZLE_Mode" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_CONFIGURATION, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_ZS_ZLE_Mode] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x30000) != 0x20000 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_ZS_AMP_Mode()                         */
/********************************************************************/
bool ADC_V1721_CAEN::Is_ZS_AMP_Mode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_ZS_AMP_Mode" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_CONFIGURATION, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_ZS_AMP_Mode] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x30000) != 0x30000 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_ZS_Switched_Off()                     */
/********************************************************************/
bool ADC_V1721_CAEN::Is_ZS_Switched_Off(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_ZS_Switched_Off" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_CONFIGURATION, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_ZS_Switched_Off] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x30000) != 0 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_TRG_OUT_OVER_Threshold()              */
/********************************************************************/
bool ADC_V1721_CAEN::Is_TRG_OUT_OVER_Threshold(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_TRG_OUT_OVER_Threshold" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_CONFIGURATION, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_TRG_OUT_OVER_Threshold] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x40) != 0 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_TestPatternGenerator_Enabled()        */
/********************************************************************/
bool ADC_V1721_CAEN::Is_TestPatternGenerator_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_TestPatternGenerator_Enabled" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_CONFIGURATION, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_TestPatternGenerator_Enabled] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x8) == 0 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Trg_Overlapp_Enabled()                */
/********************************************************************/
  bool ADC_V1721_CAEN::Is_Trg_Overlapp_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Trg_Overlapp_Enabled" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_CONFIGURATION, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Trg_Overlapp_Enabled] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x2) == 0 )
    return false;
  return true;
}



/********************************************************************/
/*    unsigned long ADC_V1721_CAEN::Get_Ch_ZSAMP_DataNumber()       */
/********************************************************************/
unsigned long ADC_V1721_CAEN::Get_Ch_ZSAMP_DataNumber(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Ch_ZSAMP_DataNumber" << endl;
#endif
  if(channel<0 || channel>7){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_ZSAMP_DataNumber] Channel out of range [0:7]!" << endl;
    return 0;
  }
  unsigned long Data;
  if(!Is_ZS_AMP_Mode())
    cerr << " [Warning:ADC_V1721_CAEN-Get_Ch_ZSAMP_DataNumber] Module not in ZS AMP Mode! Return value has no sense!" << endl;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_NS_AMP+channel*0x100, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_ZSAMP_DataNumber] Read Error!" << endl;
    return 0;
  }
  return (0x1FFFFF & Data);
}



/********************************************************************/
/*    unsigned long ADC_V1721_CAEN::Get_Ch_ZLE_N_LBK()              */
/********************************************************************/
unsigned long ADC_V1721_CAEN::Get_Ch_ZLE_N_LBK(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Ch_ZLE_N_LBK" << endl;
#endif
  if(channel<0 || channel>7){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_ZLE_N_LBK] Channel out of range [0:7]!" << endl;
    return 0;
  }
  unsigned long Data;
  if(!Is_ZS_ZLE_Mode())
    cerr << " [Warning:ADC_V1721_CAEN-Get_Ch_ZLE_N_LBK] Module not in ZS ZLE Mode! Return value has no sense!" << endl;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_NS_AMP+channel*0x100, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_ZLE_N_LBK] Read Error!" << endl;
    return 0;
  }
  return (0xFFFF & Data);
}



/********************************************************************/
/*    unsigned long ADC_V1721_CAEN::Get_Ch_ZLE_N_LFWD()             */
/********************************************************************/
unsigned long ADC_V1721_CAEN::Get_Ch_ZLE_N_LFWD(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Ch_ZLE_N_LFWD" << endl;
#endif
  if(channel<0 || channel>7){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_ZLE_N_LFWD] Channel out of range [0:7]!" << endl;
    return 0;
  }
  unsigned long Data;
  if(!Is_ZS_ZLE_Mode())
    cerr << " [Warning:ADC_V1721_CAEN-Get_Ch_ZLE_N_LFWD] Module not in ZS ZLE Mode! Return value has no sense!" << endl;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_NS_AMP+channel*0x100, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Ch_ZLE_N_LFWD] Read Error!" << endl;
    return 0;
  }
  return ((~0xFFFF & Data) >> 16);
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_Buffer_Organisation()      */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_Buffer_Organisation(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Buffer_Organisation" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+BUFFER_ORGANISATION, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Buffer_Organisation] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(0xF & Data);
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_Buffer_Free()                */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_Buffer_Free(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Buffer_Free" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+BUFFER_FREE, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Buffer_Free] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(0xFFF & Data);
}



/********************************************************************/
/*    unsigned long ADC_V1721_CAEN::Get_Custom_Size()               */
/********************************************************************/
unsigned long ADC_V1721_CAEN::Get_Custom_Size(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Custom_Size" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CUSTOM_SIZE, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Custom_Size] Read Error!" << endl;
    return 0;
  }
  return Data;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Count_All_Trg()                       */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Count_All_Trg(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Count_All_Trg" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Count_All_Trg] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x8) == 0 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_RegisterControlled_Mode()             */
/********************************************************************/
bool ADC_V1721_CAEN::Is_RegisterControlled_Mode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_RegisterControlled_Mode" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_RegisterControlled_Mode] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x3) != 0 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_SIN_Controlled_Mode()                 */
/********************************************************************/
bool ADC_V1721_CAEN::Is_SIN_Controlled_Mode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_SIN_Controlled_Mode" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_SIN_Controlled_Mode] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x3) != 1 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_SIN_Gate_Mode()                       */
/********************************************************************/
bool ADC_V1721_CAEN::Is_SIN_Gate_Mode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_SIN_Gate_Mode" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_SIN_Gate_Mode] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x3) != 2 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_MultiboardSync_Mode()                 */
/********************************************************************/
bool ADC_V1721_CAEN::Is_MultiboardSync_Mode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_MultiboardSync_Mode" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_MultiboardSync_Mode] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x3) != 3 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Bypass_Mode()                         */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Bypass_Mode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Bypass_Mode" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_STATUS, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Bypass_Mode] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x40) == 0 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_ClockSrc_Internal()                   */
/********************************************************************/
bool ADC_V1721_CAEN::Is_ClockSrc_Internal(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_ClockSrc_Internal" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_STATUS, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_ClockSrc_Internal] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x20) != 0 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_SW_Trg_Src_Enabled()                  */
/********************************************************************/
bool ADC_V1721_CAEN::Is_SW_Trg_Src_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_SW_Trg_Src_Enabled" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_SW_Trg_Src_Enabled] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x80000000) == 0 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Ext_Trg_Src_Enabled()                  */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Ext_Trg_Src_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Ext_Trg_Src_Enabled" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Ext_Trg_Src_Enabled] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x40000000) == 0 )
    return false;
  return true;
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_LocalCoincidenceLevel()    */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_LocalCoincidenceLevel(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_LocalCoincidenceLevel" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_LocalCoincidenceLevel] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)((Data >> 24) & 0x7);
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Ch_Trg_Src_Enabled()                  */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Ch_Trg_Src_Enabled(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Ch_Trg_Src_Enabled" << endl;
#endif
  if(channel<0 || channel>NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Ch_Trg_Src_Enabled] Channel out of range [0:7]!" << endl;
    return false;
  }
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+TRIGGER_SOURCE_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Ch_Trg_Src_Enabled] Read Error!" << endl;
    return false;
  }
  if(channel<NCH){
    if( (Data & (1 << channel)) == 0)
      return false;
  }else{
    if( (Data & 0xFF) != 0xFF)
      return false;
  }
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_SW_Trg_Out_Enabled()                  */
/********************************************************************/
  bool ADC_V1721_CAEN::Is_SW_Trg_Out_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_SW_Trg_Out_Enabled" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_SW_Trg_Out_Enabled] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x80000000) == 0 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Ext_Trg_Out_Enabled()                 */
/********************************************************************/
  bool ADC_V1721_CAEN::Is_Ext_Trg_Out_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Ext_Trg_Out_Enabled" << endl;
#endif
  unsigned long Data;
  int ret;
  ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Ext_Trg_Out_Enabled] Read Error!" << endl;
    return false;
  }
  if( (Data & 0x40000000) == 0 )
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Ch_Trg_Out_Enabled()                  */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Ch_Trg_Out_Enabled(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Ch_Trg_Out_Enabled" << endl;
#endif
  if(channel<0 || channel>NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Ch_Trg_Out_Enabled] Channel out of range [0:7]!" << endl;
    return false;
  }
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Ch_Trg_Out_Enabled] Read Error!" << endl;
    return false;
  }
  if(channel<NCH){
    if( (Data & (1 << channel)) == 0)
      return false;
  }else{
    if( (Data & 0xFF) != 0xFF)
      return false;
  }
  return true;
}



/********************************************************************/
/*    unsigned long ADC_V1721_CAEN::Get_Post_Trg()                  */
/********************************************************************/
unsigned long ADC_V1721_CAEN::Get_Post_Trg(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Post_Trg" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+POST_TRIGGER_SETTING, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Post_Trg] Read Error!" << endl;
    return 0;
  }
  return Data;
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_FrontPanel_IO_Data()         */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_FrontPanel_IO_Data(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_FrontPanel_IO_Data" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_DATA, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_FrontPanel_IO_Data] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(Data & 0xFFFF);
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_IO_Operation_TestMode()               */
/********************************************************************/
bool ADC_V1721_CAEN::Is_IO_Operation_TestMode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_IO_Operation_TestMode" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_IO_Operation_TestMode] Read Error!" << endl;
    return false;
  }
  if((Data & 0x8000) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_TrgOut_TestMode_Set_to_One()          */
/********************************************************************/
bool ADC_V1721_CAEN::Is_TrgOut_TestMode_Set_to_One(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_TrgOut_TestMode_Set_to_One" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_TrgOut_TestMode_Set_to_One] Read Error!" << endl;
    return false;
  }
  if((Data & 0x4000) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_IO_GeneralPurpose()                  */
/********************************************************************/
bool ADC_V1721_CAEN::Is_IO_GeneralPurpose(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_IO_GeneralPurpose" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_IO_GeneralPurpose] Read Error!" << endl;
    return false;
  }
  if((Data & 0xC0) != 0x0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_IO_Programmed()                       */
/********************************************************************/
bool ADC_V1721_CAEN::Is_IO_Programmed(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_IO_Programmed" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_IO_Programmed] Read Error!" << endl;
    return false;
  }
  if((Data & 0xC0) != 0x40)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_IO_PatternMode()                      */
/********************************************************************/
bool ADC_V1721_CAEN::Is_IO_PatternMode(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_IO_PatternMode" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  //  cerr << "0x" << hex << Data << dec << endl;

  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_IO_PatternMode] Read Error!" << endl;
    return false;
  }
  if((Data & 0xC0) != 0x80)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_LVDS_IO_15_12_Input()                 */
/********************************************************************/
bool ADC_V1721_CAEN::Is_LVDS_IO_15_12_Input(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_LVDS_IO_15_12_Input" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_LVDS_IO_15_12_Input] Read Error!" << endl;
    return false;
  }
  if((Data & 0x20) != 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_LVDS_IO_11_8_Input()                  */
/********************************************************************/
bool ADC_V1721_CAEN::Is_LVDS_IO_11_8_Input(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_LVDS_IO_11_8_Input" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_LVDS_IO_11_8_Input] Read Error!" << endl;
    return false;
  }
  if((Data & 0x10) != 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_LVDS_IO_7_4_Input()                   */
/********************************************************************/
bool ADC_V1721_CAEN::Is_LVDS_IO_7_4_Input(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_LVDS_IO_7_4_Input" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_LVDS_IO_7_4_Input] Read Error!" << endl;
    return false;
  }
  if((Data & 0x8) != 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_LVDS_IO_3_0_Input()                   */
/********************************************************************/
bool ADC_V1721_CAEN::Is_LVDS_IO_3_0_Input(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_LVDS_IO_3_0_Input" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_LVDS_IO_3_0_Input] Read Error!" << endl;
    return false;
  }
  if((Data & 0x4) != 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_PanelOutSignal_in_high_Impedance()    */
/********************************************************************/
bool ADC_V1721_CAEN::Is_PanelOutSignal_in_high_Impedance(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_PanelOutSignal_in_high_Impedance" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_PanelOutSignal_in_high_Impedance] Read Error!" << endl;
    return false;
  }
  if((Data & 0x2) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_TRG_CLK_NIM()                         */
/********************************************************************/
bool ADC_V1721_CAEN::Is_TRG_CLK_NIM(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_TRG_CLK_NIM" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FRONT_PANEL_IO_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_TRG_CLK_NIM] Read Error!" << endl;
    return false;
  }
  if((Data & 0x1) != 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Ch_Enabled()                          */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Ch_Enabled(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Ch_Enabled" << endl;
#endif
  if(channel<0 || channel>NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Ch_Enabled] Channel out of range [0:7]!" << endl;
    return false;
  }
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+CHANNEL_ENABLE_MASK, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Ch_Enabled] Read Error!" << endl;
    return false;
  }
  if(channel<NCH){
    if( (Data & (1 << channel)) == 0)
      return false;
  }else{
    if( (Data & 0xFF) != 0xFF)
      return false;
  }
  return true;
}


/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_Monitor_DAC()                */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_Monitor_DAC(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Monitor_DAC" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+SET_MONITOR_DAC, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Monitor_DAC] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(0xFFF & Data);
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_MonitorMode_Majority()                */
/********************************************************************/
bool ADC_V1721_CAEN::Is_MonitorMode_Majority(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_MonitorMode_Majority" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MONITOR_MODE, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_MonitorMode_Majority] Read Error!" << endl;
    return false;
  }
  if((Data & 0x7) != 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_MonitorMode_WaveformGenerator()       */
/********************************************************************/
bool ADC_V1721_CAEN::Is_MonitorMode_WaveformGenerator(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_MonitorMode_WaveformGenerator" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MONITOR_MODE, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_MonitorMode_WaveformGenerator] Read Error!" << endl;
    return false;
  }
  if((Data & 0x7) != 0x1)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_MonitorMode_BufferOccupancy()         */
/********************************************************************/
bool ADC_V1721_CAEN::Is_MonitorMode_BufferOccupancy(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_MonitorMode_BufferOccupancy" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MONITOR_MODE, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_MonitorMode_BufferOccupancy] Read Error!" << endl;
    return false;
  }
  if((Data & 0x7) != 0x3)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_MonitorMode_VoltageLevel()            */
/********************************************************************/
bool ADC_V1721_CAEN::Is_MonitorMode_VoltageLevel(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_MonitorMode_VoltageLevel" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MONITOR_MODE, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_MonitorMode_VoltageLevel] Read Error!" << endl;
    return false;
  }
  if((Data & 0x7) != 0x4)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_IRQMode_RORA()                        */
/********************************************************************/
bool ADC_V1721_CAEN::Is_IRQMode_RORA(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_IRQMode_RORA" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_IRQMode_RORA] Read Error!" << endl;
    return false;
  }
  if((Data & 0x80) != 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_RELOC_Enabled()                       */
/********************************************************************/
bool ADC_V1721_CAEN::Is_RELOC_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_RELOC_Enabled" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_RELOC_Enabled] Read Error!" << endl;
    return false;
  }
  if((Data & 0x40) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_ALIGN64_Enabled()                     */
/********************************************************************/
bool ADC_V1721_CAEN::Is_ALIGN64_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_ALIGN64_Enabled" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_ALIGN64_Enabled] Read Error!" << endl;
    return false;
  }
  if((Data & 0x20) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_BERR_Enabled()                        */
/********************************************************************/
bool ADC_V1721_CAEN::Is_BERR_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_BERR_Enabled" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_BERR_Enabled] Read Error!" << endl;
    return false;
  }
  if((Data & 0x10) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_OpticalLinkIRQ_Enabled()              */
/********************************************************************/
bool ADC_V1721_CAEN::Is_OpticalLinkIRQ_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_OpticalLinkIRQ_Enabled" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_OpticalLinkIRQ_Enabled] Read Error!" << endl;
    return false;
  }
  if((Data & 0x8) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_IRQ_Level()                */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_IRQ_Level(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_IRQ_Level" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_IRQ_Level] Read Error!" << endl;
    return false;
  }
  return (unsigned int)(0x7 & Data);
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_BoardID()                    */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_BoardID(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_BoardID" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+BOARD_ID, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_BoardID] Read Error!" << endl;
    return false;
  }
  return (unsigned int)(0x1F & Data);
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_MCST_CBLT_Addr()             */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_MCST_CBLT_Addr(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_MCST_CBLT_Addr" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MULTICAST_BASEADDR_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_MCST_CBLT_Addr] Read Error!" << endl;
    return false;
  }
  return (unsigned int)(0xFF & Data);
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Board_disabled()                      */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Board_disabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Board_disabled" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MULTICAST_BASEADDR_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Board_disabled] Read Error!" << endl;
    return false;
  }
  if((Data & 0x300) != 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Board_last()                          */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Board_last(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Board_last" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MULTICAST_BASEADDR_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Board_last] Read Error!" << endl;
    return false;
  }
  if((Data & 0x300) != 0x100)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Board_first()                         */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Board_first(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Board_first" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MULTICAST_BASEADDR_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Board_first] Read Error!" << endl;
    return false;
  }
  if((Data & 0x300) != 0x200)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Board_intermediate()                  */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Board_intermediate(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Board_intermediate" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+MULTICAST_BASEADDR_CONTROL, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Board_intermediate] Read Error!" << endl;
    return false;
  }
  if((Data & 0x300) != 0x300)
    return false;
  return true;
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_Reloc_Address()              */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_Reloc_Address(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Reloc_Address" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+RELOCATION_ADDRESS, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Reloc_Address] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(0xFFFF & Data);
}



/********************************************************************/
/*    unsigned long ADC_V1721_CAEN::Get_IRQ_StatusID()              */
/********************************************************************/
unsigned long ADC_V1721_CAEN::Get_IRQ_StatusID(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_IRQ_StatusID" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+INTERRUPT_STATUS_ID, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_IRQ_StatusID] Read Error!" << endl;
    return 0;
  }
  return Data;
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_IRQ_Event_Number()           */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_IRQ_Event_Number(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_IRQ_Event_Number" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+INTERRUPT_EVENT_NUMBER, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_IRQ_Event_Number] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(0x3FF & Data);
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_BLT_Event_Number()           */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_BLT_Event_Number(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_BLT_Event_Number" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+BLT_EVENT_NUMBER, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Get_BLT_Event_Number] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(0xFF & Data);
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Flash_Enabled()                       */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Flash_Enabled(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Flash_Enabled" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+FLASH_ENABLE, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Flash_Enabled] Read Error!" << endl;
    return 0;
  }
  if(Data != 0)
    return false;
  return true;
}
  


//---------------------------- Current Status ---------------------------
/********************************************************************/
/*    void ADC_V1721_CAEN::Print_VME_Status()                       */
/********************************************************************/
void ADC_V1721_CAEN::Print_VME_Status(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Print_VME_Status" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_STATUS, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Print_VME_Status] Read Error!" << endl;
  }else{
    cerr << "\n VME BERR               : " ;
    (Data & 0x4)!=0 ? cerr << "yes\n" : cerr << "no\n" ;
    cerr << " VME Output Buffer full : " ;
    (Data & 0x2)!=0 ? cerr << "yes\n" : cerr << "no\n" ;
    cerr << " VME Event ready        : " ;
    (Data & 0x1)!=0 ? cerr << "yes\n" : cerr << "no\n" ;
  }
}



/********************************************************************/
/*    void ADC_V1721_CAEN::Print_Acquisition_Status()               */
/********************************************************************/
void ADC_V1721_CAEN::Print_Acquisition_Status(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Print_Acquisition_Status" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_STATUS, &Data, cvA32_U_DATA, cvD32);
  if (ret<0){
    cerr << " [ERR:ADC_V1721_CAEN-Print_Acquisition_Status] Read Error!" << endl;
  }else{
    cerr << "\n ACQUISITION_STATUS 0x" << hex << Data << dec << endl;
    cerr << "\n Board ready for Acquisition : " ;
    (Data & 0x100)!=0 ? cerr << "yes\n" : cerr << "no\n" ;
    cerr << " PLL loss of clock           : " ;
    (Data & 0x80)!=0 ? cerr << "no\n" : cerr << "yes\n" ;
    cerr << " Board in Bypass Mode        : " ;
    (Data & 0x40)!=0 ? cerr << "yes\n" : cerr << "no\n" ;
    cerr << " Clock Source                : " ;
    (Data & 0x20)!=0 ? cerr << "External\n" : cerr << "Internal\n" ;
    cerr << " EVENT FULL                  : " ;
    (Data & 0x10)!=0 ? cerr << "yes\n" : cerr << "no\n" ;
    cerr << " EVENT READY                 : " ;
    (Data & 0x8)!=0 ? cerr << "yes\n" : cerr << "no\n" ;
    cerr << " Acquisition Running         : " ;
    (Data & 0x4)!=0 ? cerr << "yes\n" : cerr << "no\n" ;
  }
}



/********************************************************************/
/*    void ADC_V1721_CAEN::Print_Ch_n_Status()                      */
/********************************************************************/
void ADC_V1721_CAEN::Print_Ch_n_Status(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Print_Ch_n_Status" << endl;
#endif
  unsigned long Data;
  int ret ;
  for(int i=0; i<NCH; i++){
    cerr << endl << " Channel " << i << " : " << endl;
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+i*0x100, &Data, cvA32_U_DATA, cvD32);
    if (ret<0){
      cerr << " [ERR:ADC_V1721_CAEN-Print_Ch_n_Status] Read Error!" << endl;
    }else{
      cerr << " Calibration              : " ;
      (Data & 0x40)!=0 ? cerr << "done\n" : cerr << "in progress\n" ;
      cerr << " Buff free error          : " ;
      (Data & 0x20)!=0 ? cerr << "occured\n" : cerr << "no\n" ;
      cerr << " Channel n DAC            : " ;
      (Data & 0x4)!=0 ? cerr << "Busy\n" : cerr << "DC Offset updated\n" ;
      cerr << " Memory empty             : " ;
      (Data & 0x2)!=0 ? cerr << "yes\n" : cerr << "no\n" ;
      cerr << " Memory full              : " ;
      (Data & 0x1)!=0 ? cerr << "yes\n" : cerr << "no\n" ;
    }
  }
}



/********************************************************************/
/*    unsigned int ADC_V1721_CAEN::Get_BufferOccupancy()            */
/********************************************************************/
unsigned int ADC_V1721_CAEN::Get_BufferOccupancy(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_BufferOccupancy" << endl;
#endif
  if(channel<0 || channel>7){
    cerr << " [ERR:ADC_V1721_CAEN-Get_BufferOccupancy] Channel out of range [0:7]!" << endl;
    return 0;
  }
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_BUFFER_OCCUPANCY+channel*0x100, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Get_BufferOccupancy] Read Error!" << endl;
    return 0;
  }
  return (unsigned int)(Data & 0x7FF);
}



/********************************************************************/
/*    unsigned long ADC_V1721_CAEN::Get_Number_StoredEvents()       */
/********************************************************************/
unsigned long ADC_V1721_CAEN::Get_Number_StoredEvents(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Number_StoredEvents" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+EVENT_STORED, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Number_StoredEvents] Read Error!" << endl;
    return 0;
  }
  return Data;
}



/********************************************************************/
/*    unsigned long ADC_V1721_CAEN::Get_Event_Size()                */
/********************************************************************/
unsigned long ADC_V1721_CAEN::Get_Event_Size(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_Event_Size" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+EVENT_SIZE, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Get_Event_Size] Read Error!" << endl;
    return 0;
  }
  return Data;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Get_BufferOccupancy()                    */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Calibration_done(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Get_BufferOccupancy" << endl;
#endif
  if(channel<0 || channel>NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Get_BufferOccupancy] Channel out of range [0:8]!" << endl;
    return 0;
  }
  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+channel*0x100, &Data, cvA32_U_DATA, cvD32);
    if( ret<0 ){
      cerr << " [ERR:ADC_V1721_CAEN-Get_BufferOccupancy] Read Error!" << endl;
      return false;
    }
    if((Data & 0x40) == 0)
      return false;
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+i*0x100, &Data, cvA32_U_DATA, cvD32);
      if( ret<0 ){
	cerr << " [ERR:ADC_V1721_CAEN-Get_BufferOccupancy] Read Error!" << endl;
	return false;
      }
      if((Data & 0x40) == 0)
	return false;
    }
  }
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_BufferFreeError()                     */
/********************************************************************/
bool ADC_V1721_CAEN::Is_BufferFreeError(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_BufferFreeError" << endl;
#endif
  if(channel<0 || channel>NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Is_BufferFreeError] Channel out of range [0:8]!" << endl;
    return 0;
  }
  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+channel*0x100, &Data, cvA32_U_DATA, cvD32);
    if( ret<0 ){
      cerr << " [ERR:ADC_V1721_CAEN-Is_BufferFreeError] Read Error!" << endl;
      return false;
    }
    if((Data & 0x20) == 0)
      return false;
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+i*0x100, &Data, cvA32_U_DATA, cvD32);
      if( ret<0 ){
	cerr << " [ERR:ADC_V1721_CAEN-Is_BufferFreeError] Read Error!" << endl;
	return false;
      }
      if((Data & 0x20) == 0)
	return false;
    }
  }
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_DC_Offset_Updated()                   */
/********************************************************************/
bool ADC_V1721_CAEN::Is_DC_Offset_Updated(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_DC_Offset_Updated" << endl;
#endif
  if(channel<0 || channel>NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Is_DC_Offset_Updated] Channel out of range [0:8]!" << endl;
    return 0;
  }
  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+channel*0x100, &Data, cvA32_U_DATA, cvD32);
    if( ret<0 ){
      cerr << " [ERR:ADC_V1721_CAEN-Is_DC_Offset_Updated] Read Error!" << endl;
      return false;
    }
    if((Data & 0x4) != 0)
      return false;
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+i*0x100, &Data, cvA32_U_DATA, cvD32);
      if( ret<0 ){
	cerr << " [ERR:ADC_V1721_CAEN-Is_DC_Offset_Updated] Read Error!" << endl;
	return false;
      }
      if((Data & 0x4) != 0)
	return false;
    }
  }
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_MemoryEmpty()                         */
/********************************************************************/
bool ADC_V1721_CAEN::Is_MemoryEmpty(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_MemoryEmpty" << endl;
#endif
  if(channel<0 || channel>NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Is_MemoryEmpty] Channel out of range [0:8]!" << endl;
    return 0;
  }
  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+channel*0x100, &Data, cvA32_U_DATA, cvD32);
    if( ret<0 ){
      cerr << " [ERR:ADC_V1721_CAEN-Is_MemoryEmpty] Read Error!" << endl;
      return false;
    }
    if((Data & 0x2) != 0)
      return false;
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+i*0x100, &Data, cvA32_U_DATA, cvD32);
      if( ret<0 ){
	cerr << " [ERR:ADC_V1721_CAEN-Is_MemoryEmpty] Read Error!" << endl;
	return false;
      }
      if((Data & 0x2) != 0)
	return false;
    }
  }
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_MemoryFull()                          */
/********************************************************************/
bool ADC_V1721_CAEN::Is_MemoryFull(int channel){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_MemoryFull" << endl;
#endif
  if(channel<0 || channel>NCH){
    cerr << " [ERR:ADC_V1721_CAEN-Is_MemoryFull] Channel out of range [0:8]!" << endl;
    return 0;
  }
  unsigned long Data;
  int ret;
  if(channel<NCH){
    ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+channel*0x100, &Data, cvA32_U_DATA, cvD32);
    if( ret<0 ){
      cerr << " [ERR:ADC_V1721_CAEN-Is_MemoryFull] Read Error!" << endl;
      return false;
    }
    if((Data & 0x1) != 0)
      return false;
  }else{
    for(int i=0; i<NCH; i++){
      ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+Ch_0_STATUS+i*0x100, &Data, cvA32_U_DATA, cvD32);
      if( ret<0 ){
	cerr << " [ERR:ADC_V1721_CAEN-Is_MemoryFull] Read Error!" << endl;
	return false;
      }
      if((Data & 0x1) != 0)
	return false;
    }
  }
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_Board_Ready_For_Acquisition()         */
/********************************************************************/
bool ADC_V1721_CAEN::Is_Board_Ready_For_Acquisition(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_Board_Ready_For_Acquisition" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_STATUS, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Is_Board_Ready_For_Acquisition] Read Error!" << endl;
    return false;
  }
  if((Data & 0x100) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_PLL_Locked()                          */
/********************************************************************/
inline bool ADC_V1721_CAEN::Is_PLL_Locked(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_PLL_Locked" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_STATUS, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Is_PLL_Locked] Read Error!" << endl;
    return false;
  }
  if((Data & 0x80) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_EventFULL()                           */
/********************************************************************/
inline bool ADC_V1721_CAEN::Is_EventFULL(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_EventFULL" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_STATUS, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Is_EventFULL] Read Error!" << endl;
    return false;
  }
  if((Data & 0x10) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_EventREADY()                          */
/********************************************************************/
inline bool ADC_V1721_CAEN::Is_EventREADY(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_EventREADY" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_STATUS, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Is_EventREADY] Read Error!" << endl;
    return false;
  }
  if((Data & 0x8) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_ACQ_Running()                         */
/********************************************************************/
inline bool ADC_V1721_CAEN::Is_ACQ_Running(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_ACQ_Running" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+ACQUISITION_STATUS, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Is_ACQ_Running] Read Error!" << endl;
    return false;
  }
  if((Data & 0x4) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_VME_BERR()                            */
/********************************************************************/
inline bool ADC_V1721_CAEN::Is_VME_BERR(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] " << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_STATUS, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Is_VME_BERR] Read Error!" << endl;
    return false;
  }
  if((Data & 0x4) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_VME_OutBuffer_Full()                  */
/********************************************************************/
  bool ADC_V1721_CAEN::Is_VME_OutBuffer_Full(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_VME_OutBuffer_Full" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_STATUS, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Is_VME_OutBuffer_Full] Read Error!" << endl;
    return false;
  }
  if((Data & 0x2) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    bool ADC_V1721_CAEN::Is_VME_EventReady()                      */
/********************************************************************/
  bool ADC_V1721_CAEN::Is_VME_EventReady(){
#ifdef DEBUG_
  cerr << " [DBG:ADC_V1721_CAEN] Is_VME_EventReady" << endl;
#endif
  unsigned long Data;
  int ret = CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+VME_STATUS, &Data, cvA32_U_DATA, cvD32);
  if( ret<0 ){
    cerr << " [ERR:ADC_V1721_CAEN-Is_VME_EventReady] Read Error!" << endl;
    return false;
  }
  if((Data & 0x1) == 0)
    return false;
  return true;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Read_Event()                              */
/********************************************************************/
int ADC_V1721_CAEN::Read_Event(unsigned long * rdata){
  return CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr, rdata, cvA32_U_DATA, cvD32);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::DAQ_To_File()                             */
/********************************************************************/
int ADC_V1721_CAEN::DAQ_To_File(string filename, int NbrOfEvt){
  unsigned long Data, DSize=0;
  int nbr=0;

  ofstream of(filename.c_str());

  this->Set_ACQ_RUN_START();
  while(nbr<NbrOfEvt){
    unsigned long bla=this->Get_Number_StoredEvents();
    if(bla>1){
      //      cerr << "bla " << bla << endl;
      CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr,&Data, cvA32_U_DATA, cvD32); // Header : Event Size
      DSize=(0xFFFFFFF & Data);
      //      cerr << DSize << endl;
      of << hex << Data << dec << endl;  
      CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+0x0,&Data, cvA32_U_DATA, cvD32); // Header : Ch Mask & More
      of << hex << Data << dec << endl;  
      CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+0x0,&Data, cvA32_U_DATA, cvD32); // Header : Event Counter
      of << hex << Data << dec << endl;  
      CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+0x0,&Data, cvA32_U_DATA, cvD32); // Header : Trigger Time Tag
      of << hex << Data << dec << endl;  
      for(unsigned int i=0; i<DSize-4; i++){
	CAENVME_ReadCycle(this->m_handle.m_vme_handle, BaseAddr+0x0,&Data, cvA32_U_DATA, cvD32);
	of << hex << Data << dec << endl;
      }
      nbr++;
      if(nbr%1000==0) cout << nbr << endl;
    }
  }

  this->Set_ACQ_RUN_STOP();
  of.close();
  return 0;
}


  
/********************************************************************/
/*    int ADC_V1721_CAEN::DAQ_To_File_MBLT64()                         */
/********************************************************************/
int ADC_V1721_CAEN::DAQ_To_File_MBLT64(string filename, int NbrOfEvt){
  unsigned int FixCount=this->Get_BLT_Event_Number();
  int pot=(int)pow(2.,(double)this->Get_Buffer_Organisation());
  const unsigned int FixNumber=FixCount*4*pot;
  unsigned long Nread=FixNumber;
  unsigned long *BLTData = new unsigned long[FixNumber];
  int nbr=0, ret=Nread;
  for(unsigned int i=0; i<Nread;i++) BLTData[i]=0;
  BLTData[0]=5;

  unsigned int EvtStord=0;
  int count;
//   cout << " FixCount  " << FixCount << endl;
//   cout << " FixNumber " << FixNumber << endl;

  ofstream of(filename.c_str());

  while(!this->Is_Board_Ready_For_Acquisition()){
    this->SW_Clear();
  }
  this->Set_ACQ_RUN_START();
  usleep(50000);
  EvtStord=this->Get_Number_StoredEvents();
  if(EvtStord>0)
    this->SW_Clear();
  while(nbr<NbrOfEvt){
    EvtStord=this->Get_Number_StoredEvents();
    if( EvtStord>=FixCount ){
      ret=100;
      cerr << "  Events Stored:  " << EvtStord << endl;
      cerr << "  Event Size:  " << this->Get_Event_Size() << endl;

      while(ret>=0 && nbr<NbrOfEvt){
	ret = CAENVME_BLTReadCycle(this->m_handle.m_vme_handle, (uint32_t) BaseAddr, (void*) BLTData, Nread, cvA32_U_MBLT, cvD64, &count);
	cerr << " count " << count << endl;
	//	of << "---- Next Loop ----" << int(Nread/FixCount) << endl;
	for(unsigned int i=0; i<count/FixCount; i++)
	  of << hex << BLTData[i] << dec << endl;
	nbr+=FixCount;
	if(nbr%1000==0) cout << nbr << endl;
      }
    }
  }
  this->Set_ACQ_RUN_STOP();

  of.close();

  delete BLTData;

  cout << " Ready ;)" << endl;

  return 0;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Read_MBLT64()                             */
/********************************************************************/
int ADC_V1721_CAEN::Read_MBLT64(unsigned long * BLTData, unsigned long Nread){
                                      // long statt int !!
  int count;

  int ret = CAENVME_BLTReadCycle(this->m_handle.m_vme_handle, (uint32_t) BaseAddr, (void*) BLTData, Nread, cvA32_U_MBLT, cvD64, &count); 

  //  cout << " Ret in MBLT64 Read = " << ret << endl;
  if(ret<0)
    return ret;

  return count;
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Enable_IRQ_PCI()                          */
/********************************************************************/
int ADC_V1721_CAEN::Enable_IRQ_PCI(unsigned long Mask){
  return CAENVME_IRQEnable(this->m_handle.m_vme_handle, Mask);
}



/********************************************************************/
/*    int ADC_V1721_CAEN::Disable_IRQ_PCI()                         */
/********************************************************************/
int ADC_V1721_CAEN::Disable_IRQ_PCI(unsigned long Mask){
  return CAENVME_IRQDisable(this->m_handle.m_vme_handle, Mask);
}



#include <sys/time.h>
#include <time.h>
/********************************************************************/
/*    int ADC_V1721_CAEN::Wait_IRQ_PCI()   "interrupt request"      */
/********************************************************************/
int ADC_V1721_CAEN::Wait_IRQ_PCI(unsigned long Mask, unsigned long Timeout){
  /*
  timeval tot_time, tot_time1;
  gettimeofday(&tot_time, 0);                   // Starttime
  double time = tot_time.tv_sec + 1.e-6*tot_time.tv_usec;    // and usec
 
  while(true){
    if(Is_VME_EventReady())
      return 0;
    //    cerr << " Wait" << endl;
    usleep(10);
    gettimeofday(&tot_time1, 0);
    if((tot_time1.tv_sec + 1.e-6*tot_time1.tv_usec-time) > 1.e-3*Timeout)
      break;
  }
  return -1;
  */
    
  CVErrorCodes cv_error_code;

  if( (cv_error_code=CAENVME_IRQWait(this->m_handle.m_vme_handle, Mask, Timeout))!= cvSuccess )
    return (int)cv_error_code;
  return 0;
  
  //  return CAENVME_IRQWait(this->m_handle.m_vme_handle, Mask, Timeout);
}



#include <sys/time.h>
#include <time.h>
/********************************************************************/
/*    int ADC_V1721_CAEN::Wait_4_Event()  "poll for rare triggers"  */
/********************************************************************/
int ADC_V1721_CAEN::Wait_4_Event( bool * stop, unsigned long Timeout ){

  timeval tot_time, tot_time1;
  gettimeofday( &tot_time, 0 );                   // Starttime
  double time = tot_time.tv_sec + 1.e-6 * tot_time.tv_usec;    // and usec

  // Interrupted readout - for reading more than one event!
  /*
  CVErrorCodes cv_error_code;
  */   

  while( true ){
    if( this->Is_VME_EventReady() ){
      // std::cout << "Event timeout: " << ( tot_time1.tv_sec + 1.e-6 * tot_time1.tv_usec ) - time << " [sec]" << std::endl; 
      return 0;
    }
    usleep( 1000 ); // 1 msec
    
    // Interrupted readout -  for reading more than one event!
    /*
    if( (cv_error_code=CAENVME_IRQWait(this->m_handle.m_vme_handle, 0xFFFFFFFF, 1000))!= cvSuccess ){
      cerr << (int)cv_error_code << " ";
    }else{
      return 0;
    }
    */

    gettimeofday( &tot_time1, 0 );
    
    if( ( ( tot_time1.tv_sec + ( 1.e-6 * tot_time1.tv_usec ) ) - time ) > 1.e-3 * Timeout ){
      std::cerr << "ADC_V1721_CAEN::Wait_4_Event(): Timeout ( " 
		<< Timeout << " ms ) expired. No event found. Try with longer timeout or check if the module recieves data." << std::endl << std::endl;
      break;
    }
    if( *stop ) return -2;
  }
  return -1;
}



// /********************************************************************/
// /*    int ADC_V1721_CAEN::Self_Trigger()  "triggers on channel threshold"  */
// /********************************************************************/
// int ADC_V1721_CAEN::Self_Trigger(int channel){

//   timeval tot_time, tot_time1;
//   gettimeofday(&tot_time, 0);                   // Starttime
//   double time = tot_time.tv_sec + 1.e-6*tot_time.tv_usec;    // and usec

//   // Interrupted readout - for reading more than one event!
//   /*
//   CVErrorCodes cv_error_code;
//   */   

//   while(true){

//     if(this->Is_VME_EventReady())
//       return 0;
//     usleep(1000); // 1 msec

//     // Interrupted readout -  for reading more than one event!
//     /*
//     if( (cv_error_code=CAENVME_IRQWait(this->m_handle.m_vme_handle, 0xFFFFFFFF, 1000))!= cvSuccess ){
//       cerr << (int)cv_error_code << " ";
//     }else{
//       return 0;
//     }
//     */

//     gettimeofday(&tot_time1, 0);
//     if((tot_time1.tv_sec + 1.e-6*tot_time1.tv_usec-time) > 1.e-3*Timeout)
//       break;
//     if(*stop)
//       return -2;
//   }
//   return -1;

// }
