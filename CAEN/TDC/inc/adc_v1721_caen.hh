#ifndef ADC_V1721_CAEN_H_
#define ADC_V1721_CAEN_H_ 1

#define NCH                             8               // number of channels

#define EVENT_READOUT_BUFFER            0x0000     // -0x0FFC      R
// all registers are A24/A32 D32
// channel registers (each 8)
#define Ch_0_ZS_THRES                   0x1024     // Ch0; Ch1=Ch0+0x100 .. RW 
#define Ch_0_NS_AMP                     0x1028     //     - " -    RW
#define Ch_0_THRESHOLD                  0x1080     //     - " -    RW
#define Ch_0_TIME_OVERUNDER_THRESHOLD   0x1084     //     - " -    RW
#define Ch_0_STATUS                     0x1088     //     - " -    R
#define Ch_0_AMC_FPGA_FIRMWARE_REVISION 0x108C     //     - " -    R
#define Ch_0_BUFFER_OCCUPANCY           0x1094     //     - " -    R
#define Ch_0_DAC                        0x1098     //     - " -    RW
#define Ch_0_ADC_CONFIGURATION          0x109C     //     - " -    RW

// single registers
#define Ch_CONFIGURATION                0x8000     //              R
#define Ch_CONFIGURATION_BIT_SET        0x8004     //              W
#define Ch_CONFIGURATION_BIT_CLEAR      0x8008     //              W
#define BUFFER_ORGANISATION             0x800C     //              RW
#define BUFFER_FREE                     0x8010     //              RW
#define CUSTOM_SIZE                     0x8020     //              RW
#define ACQUISITION_CONTROL             0x8100     //              RW
#define ACQUISITION_STATUS              0x8104     //              R
#define SOFTWARE_TRIGGER                0x8108     //              W
#define TRIGGER_SOURCE_ENABLE_MASK      0x810C     //              RW
#define FRONT_PANEL_TRIGGER_OUT_ENABLE_MASK 0x8110     //          RW
#define POST_TRIGGER_SETTING            0x8114     //              RW
#define FRONT_PANEL_IO_DATA             0x8118     //              RW
#define FRONT_PANEL_IO_CONTROL          0x811C     //              RW
#define CHANNEL_ENABLE_MASK             0x8120     //              RW
#define ROC_FPGA_FIRMMWARE_REVISION     0x8124     //              R
#define EVENT_STORED                    0x812C     //              R
#define SET_MONITOR_DAC                 0x8138     //              RW
#define BOARD_INFO                      0x8140     //              R
#define MONITOR_MODE                    0x8144     //              RW
#define EVENT_SIZE                      0x814C     //              R
#define VME_CONTROL                     0xEF00     //              RW
#define VME_STATUS                      0xEF04     //              R
#define BOARD_ID                        0xEF08     //              RW
#define MULTICAST_BASEADDR_CONTROL      0xEF0C     //              RW
#define RELOCATION_ADDRESS              0xEF10     //              RW
#define INTERRUPT_STATUS_ID             0xEF14     //              RW
#define INTERRUPT_EVENT_NUMBER          0xEF18     //              RW
#define BLT_EVENT_NUMBER                0xEF1C     //              RW
#define SCRATCH                         0xEF20     //              RW
#define SW_RESET                        0xEF24     //              W
#define SW_CLEAR                        0xEF28     //              W
#define FLASH_ENABLE                    0xEF2C     //              RW
#define FLASH_DATA                      0xEF30     //              RW
#define CONFIGURATION_RELOAD            0xEF34     //              W
#define CONFIGURATION_ROM               0xF000     // -0xF3FC      R
#define FIRMWARE_UPGRADE                0xF088     // 0 or 1 Original: 2 = Xtal upgrade 
#define LINUX 1
#include "CAENVMElib.h"
#include "CAENVMEoslib.h"
#include "CAENVMEtypes.h"
typedef unsigned long UINT32;

typedef struct{
  int  m_vme_link;                        /*!< \brief The link number */
  int  m_vme_board_num;           /*!< \brief The board number */
  UINT32 m_board_base_add;        /*!< \brief The board base address (32 bit) */
  char m_board_type[20];          /*!< \brief The board type */
} InputData;

typedef struct{
  int32_t m_vme_handle;           /*!< \brief  The VME library comunication handle */
} Handle;


#include <string>

struct Firmware{
  int revDate_year;
  int revDate_month;
  int revDate_day;
  int rev_X;
  int rev_Y;
};
typedef struct Firmware FW;

class ADC_V1721_CAEN{
public:
  // constructor for standalone device
  ADC_V1721_CAEN(const unsigned long Base_Addr, std::string ADC_Name, short vme_link=0, short board_no=0);
  // constructor for existing device handle/ Multiboard
  ADC_V1721_CAEN(const unsigned long Base_Addr, std::string ADC_Name, int caen_device_handle);
  ~ADC_V1721_CAEN();

  // ---- Setting methods

  /* Software Reset */
  int Config_Reload();    // SW_Reset, Config ROM reload, PLL reconfiguration
  int SW_Reset();         // SW_Reset, SW_Clear
  int SW_Clear();         // clears 0x0000, 0x1n94, 0x812C, 0x814C


  /* Channel Calibration */
  int Do_ChCalibration(); // Channel Calibration Procedure

  /* Channel Settings - channel == 8 <=> all channels */
  int Set_Ch_Trg_Threshold(int channel, unsigned int val); // 1n80 [7:0]

  int Set_DATANUMBER_OvrUndr_Thres(int channel, unsigned int val); // 1n84 [11:0]

  int Set_Ch_DAC(int channel, unsigned int val); // 1n98 [15:0]

  int Enable_Ch_Trg_Src(int channel);          // 810C [7:0]
  int Disable_Ch_Trg_Src(int channel);         // 810C [7:0]

  int Enable_Ch_Trg_Out(int channel);          // 8110 [7:0]
  int Disable_Ch_Trg_Out(int channel);         // 8110 [7:0]

  int Enable_Ch(int channel);                  // 8120 [7:0]
  int Disable_Ch(int channel);                 // 8120 [7:0]

  // Set Zero Supression Mode before!
  int Set_Ch_ZS_Logik_OVER_Threshold(int channel);  // 1n24 [31]
  int Set_Ch_ZS_Logik_UNDER_Threshold(int channel); // 1n24 [31]
  int Set_Ch_ZS_Compare_Value(int channel, unsigned int val); // 1n24 [7:0]
  // in Zero Suppression based on Amplitude: Number Data over/under Threshold
  int Set_Ch_ZSAMP_DataNumber(int channel, unsigned long val);  // 1n28 [20:0]
  // in Zero Length Encoding  
  int Set_Ch_ZLE_N_LBK(int channel, unsigned long val);  // 1n28 [15:0]
  int Set_Ch_ZLE_N_LFWD(int channel, unsigned long val); // 1n28 [31:16]


  /* General Settings */
  int Disable_ZeroSuppression_Mode();          // 8000 [17:16]
  int Enable_ZeroLengthEncoding_Mode();        // 8000 [17:16]
  int Enable_ZeroSuppressionAmplitude_Mode();  // 8000 [17:16]
  int Set_TRG_OUT_OVER_Threshold();            // 8000 [6]
  int Set_TRG_OUT_UNDER_Threshold();           // 8000 [6]
  //  int Set_MemoryAccess_RANDOM();           // 8000 [4] -> not implemented yet
  int Set_MemoryAccess_SEQUENTIAL();           // 8000 [4] Not necessary now, see above
  int Enable_TestPattern_Generator();          // 8000 [3] 
  int Disable_TestPattern_Generator();         // 8000 [3]
  int Enable_TrgOverlapp();                    // 8000 [1]
  int Disable_TrgOverlapp();                   // 8000 [1]

  int Set_Buffer_Organisation(unsigned int code); // 800C [3:0]

  int Set_Buffer_Free(unsigned int val);       // 8010 [11:0]

  int Set_Custom_Size(unsigned long val);      // 8020 [31:0]

  int Set_Count_ALL_Trg();                     // 8100 [3]
  int Set_Count_ACCEPTED_Trg();                // 8100 [3]
  int Set_ACQ_RUN_START();                     // 8100 [2]
  int Set_ACQ_RUN_STOP();                      // 8100 [2]
  int Set_Trg_Mode_RegisterControlled();       // 8100 [1:0]
  int Set_Trg_Mode_SIN_Controlled();           // 8100 [1:0]
  int Set_Trg_Mode_SIN_Gate();                 // 8100 [1:0]
  int Set_Trg_Mode_MultiboardSync();           // 8100 [1:0]

  int Set_SW_Trg();                            // 8108 [31:0]

  int Enable_SW_Trg_Src();                     // 810C [31]
  int Disable_SW_Trg_Src();                    // 810C [31]
  int Enable_External_Trg_Src();               // 810C [30]
  int Disable_External_Trg_Src();              // 810C [30]
  // Enable Channels before: val < # enabled channels!!! 
  int Set_LocalCoincidenceLevel(unsigned int val); // 810C [26:24]

  int Enable_SW_Trg_Out();                     // 8110 [31]
  int Disable_SW_Trg_Out();                    // 8110 [31]
  int Enable_External_Trg_Out();               // 8110 [30]
  int Disable_External_Trg_Out();              // 8110 [30]

  int Set_Post_Trg(unsigned long val);         // 8114 [31:0]

  int Set_FrontPanel_IO_Data(unsigned int val);// 8118 [15:0]

  int Set_FrontPanel_IO_Normal_OP();           // 811C [15]
  int Set_FrontPanel_IO_TestMode();            // 811C [15]
  int Set_FrontPanel_TestMode_TrgOut_One();    // 811C [14]
  int Set_FrontPanel_TestMode_TrgOut_Zero();   // 811C [14]
  int Set_FrontPanel_GeneralPurposeIO();       // 811C [7:6]
  int Set_FrontPanel_ProgrammedIO();           // 811C [7:6]
  int Set_FrontPanel_PatternMode();            // 811C [7:6]
  int Set_LVDS_IO_15_12_Input();               // 811C [5]
  int Set_LVDS_IO_15_12_Output();              // 811C [5]
  int Set_LVDS_IO_11_8_Input();                // 811C [4]
  int Set_LVDS_IO_11_8_Output();               // 811C [4]
  int Set_LVDS_IO_7_4_Input();                 // 811C [3]
  int Set_LVDS_IO_7_4_Output();                // 811C [3]
  int Set_LVDS_IO_3_0_Input();                 // 811C [2]
  int Set_LVDS_IO_3_0_Output();                // 811C [2]
  int Enable_PanelOutSignals();                // 811C [1]
  int EnableHighImpedance_PanelOutSignals();   // 811C [1]
  int Set_TRG_CLK_NIM();                       // 811C [0]
  int Set_TRG_CLK_TTL();                       // 811C [0]

  int Set_Monitor_DAC(unsigned int val);       // 8138 [11:0]

  int Set_MonitorMode_Majority();              // 8144 [2:0]
  int Set_MonitorMode_WaveformGenerator();     // 8144 [2:0]
  int Set_MonitorMode_BufferOccupancy();       // 8144 [2:0]
  int Set_MonitorMode_VoltageLevel();          // 8144 [2:0]

  int Set_IRQMode_RORA();                      // EF00 [7]
  int Set_IRQMode_ROAK();                      // EF00 [7]
  int Enable_RELOC();                          // EF00 [6]
  int Disable_RELOC();                         // EF00 [6]
  int Enable_ALIGN64();                        // EF00 [5]
  int Disable_ALIGN64();                       // EF00 [5]
  int Enable_BERR();                           // EF00 [4]
  int Disable_BERR();                          // EF00 [4]
  int Enable_OpticalLinkIRQ();                 // EF00 [3]
  int Disable_OpticalLinkIRQ();                // EF00 [3]
  int Set_IRQ_Level(unsigned int level);       // EF00 [2:0]

  int Set_BoardID(unsigned int GEOAddr);       // EF08 [4:0]

  int Set_MCST_CBLT_Addr(unsigned int Addr);   // EF0C [7:0]
  int Set_Board_disabled();                    // EF0C [9:8]
  int Set_Board_last();                        // EF0C [9:8]
  int Set_Board_first();                       // EF0C [9:8]
  int Set_Board_intermediate();                // EF0C [9:8]

  int Set_Reloc_Address(unsigned int Addr);    // EF10 [15:0]

  int Set_IRQ_StatusID(unsigned long StatusID);// EF14 [31:0]

  int Set_IRQ_Event_Number(unsigned int val);  // EF18 [9:0]

  int Set_BLT_Event_Number(unsigned int val);  // EF1C [7:0]

  int Write_to_Scratch(unsigned long val);     // EF20 [31:0]

  int Enable_Flash();                          // EF2C [0]
  int Disable_Flash();                         // EF2C [0]

  int Write_to_FlashData(unsigned long data);  // EF30 [7:0]


  // ---- Getting and Print methods

  unsigned long Get_ACQ_Status() ;             // ACQ Status
  unsigned long Get_Firmware_Upgrade();
  unsigned long Get_VME_Status() ;             // ACQ Status

  /* General Information Printing */
  void Print_Reg_Status();     // !! Massive Register access!
  void Print_ROC_FPGA_Firmware_Rev();
  void Print_AMC_FPGA_Firmware_Rev();

  /* not permanently changing Status */
  // Channel Is_... Functions: channel = 8 <=> check all channels
  // Channel Get_... Funktions: channl in [0:7] !!!
  bool Is_Ch_Logik_OverThreshold(int channel); // 1n24 [31]   else under Thres
  unsigned int Get_Ch_ZS_Compare_Value(int channel); // 1n24 [7:0]

  unsigned int Get_Ch_Trg_Threshold(int channel); // 1n80 [7:0]

  unsigned int Get_DATANUMBER_OvrUndr_Thres(int channel); // 1n84 [11:0]

  unsigned int Get_Ch_DAC(int channel);        // 1n98 [15:0]

  bool Is_ZS_ZLE_Mode();                       // 8000 [19:16]
  bool Is_ZS_AMP_Mode();                       // 8000 [19:16]
  bool Is_ZS_Switched_Off();                   // 8000 [19:16]
  bool Is_TRG_OUT_OVER_Threshold();            // 8000 [6]
  //  bool Is_MemoryAccess_SEQUENTIAL();       // 8000 [4] Not necessary now, see above
  bool Is_TestPatternGenerator_Enabled();      // 8000 [3]
  bool Is_Trg_Overlapp_Enabled();              // 8000 [1]

  // depending on the ZS Mode : only ZS_AMP
  unsigned long Get_Ch_ZSAMP_DataNumber(int channel); // 1n28 [20:0]
  // only ZLE
  unsigned long Get_Ch_ZLE_N_LBK(int channel); // 1n28 [15:0]
  unsigned long Get_Ch_ZLE_N_LFWD(int channel);// 1n28 [31:16]

  unsigned int Get_Buffer_Organisation();      // 800C [3:0]

  unsigned int Get_Buffer_Free();              // 8010 [11:0]

  unsigned long Get_Custom_Size();             // 8020 [31:0]

  bool Is_Count_All_Trg();                     // 8100 [3]  else count accepted Trg
  bool Is_RegisterControlled_Mode();           // 8100 [1:0]
  bool Is_SIN_Controlled_Mode();               // 8100 [1:0]
  bool Is_SIN_Gate_Mode();                     // 8100 [1:0]
  bool Is_MultiboardSync_Mode();               // 8100 [1:0]

  bool Is_Bypass_Mode();                       // 8104 [6]   else PLL
  bool Is_ClockSrc_Internal();                 // 8104 [5]   else External

  bool Is_SW_Trg_Src_Enabled();                // 810C [31]
  bool Is_Ext_Trg_Src_Enabled();               // 810C [30]
  unsigned int Get_LocalCoincidenceLevel();    // 810C [26:24]
  bool Is_Ch_Trg_Src_Enabled(int channel);     // 810C [7:0]

  bool Is_SW_Trg_Out_Enabled();                // 8110 [31]
  bool Is_Ext_Trg_Out_Enabled();               // 8110 [30]
  bool Is_Ch_Trg_Out_Enabled(int channel);     // 8110 [7:0]

  unsigned long Get_Post_Trg();                // 8114 [31:0]

  unsigned int Get_FrontPanel_IO_Data();       // 8118 [15:0]

  bool Is_IO_Operation_TestMode();             // 811C [15] else Normal
  bool Is_TrgOut_TestMode_Set_to_One();        // 811C [14]  else set to zero
  bool Is_IO_GeneralPurpose();                 // 811C [7:6]
  bool Is_IO_Programmed();                     // 811C [7:6]
  bool Is_IO_PatternMode();                    // 811C [7:6]
  bool Is_LVDS_IO_15_12_Input();               // 811C [5]  else Output
  bool Is_LVDS_IO_11_8_Input();                // 811C [4]  else Output
  bool Is_LVDS_IO_7_4_Input();                 // 811C [3]  else Output
  bool Is_LVDS_IO_3_0_Input();                 // 811C [2]  else Output
  bool Is_PanelOutSignal_in_high_Impedance();  // 811C [1]   else only enabled
  bool Is_TRG_CLK_NIM();                       // 811C [0]  else TTL

  bool Is_Ch_Enabled(int channel);             // 8120 [7:0]

  unsigned int Get_Monitor_DAC();              // 8138 [11:0]

  bool Is_MonitorMode_Majority();              // 8144 [2:0]
  bool Is_MonitorMode_WaveformGenerator();     // 8144 [2:0]
  bool Is_MonitorMode_BufferOccupancy();       // 8144 [2:0]
  bool Is_MonitorMode_VoltageLevel();          // 8144 [2:0]

  bool Is_IRQMode_RORA();                      // EF00 [7] else ROAK
  bool Is_RELOC_Enabled();                     // EF00 [6]
  bool Is_ALIGN64_Enabled();                   // EF00 [5]
  bool Is_BERR_Enabled();                      // EF00 [4]
  bool Is_OpticalLinkIRQ_Enabled();            // EF00 [3]
  unsigned int Get_IRQ_Level();                // EF00 [2:0]

  unsigned int Get_BoardID();                  // EF08 [4:0]

  unsigned int Get_MCST_CBLT_Addr();           // EF0C [7:0]
  bool Is_Board_disabled();                    // EF0C [9:8]
  bool Is_Board_last();                        // EF0C [9:8]
  bool Is_Board_first();                       // EF0C [9:8]
  bool Is_Board_intermediate();                // EF0C [9:8]

  unsigned int Get_Reloc_Address();            // EF10 [15:0]

  unsigned long Get_IRQ_StatusID();            // EF14 [31:0]

  unsigned int Get_IRQ_Event_Number();         // EF18 [9:0]

  unsigned int Get_BLT_Event_Number();         // EF1C [7:0]

  bool Is_Flash_Enabled();                     // EF2C [0]
  


  /* Current Status */
  void Print_VME_Status();                     // EF04
  void Print_Acquisition_Status();             // 8104
  void Print_Ch_n_Status();                    // 1n88

  unsigned int Get_BufferOccupancy(int channel); // 1n94 [10:0]

  unsigned long Get_Number_StoredEvents();     // 812C [31:0]

  unsigned long Get_Event_Size();              // 814C [31:0]
  
  bool Is_Calibration_done(int channel);       // 1n88 [6]
  bool Is_BufferFreeError(int channel);        // 1n88 [5]
  bool Is_DC_Offset_Updated(int channel);      // 1n88 [2]
  bool Is_MemoryEmpty(int channel);            // 1n88 [1]
  bool Is_MemoryFull(int channel);             // 1n88 [0]

  bool Is_Board_Ready_For_Acquisition();       // 8104 [8]
  bool Is_PLL_Locked();                        // 8104 [7]
  bool Is_EventFULL();                         // 8104 [4]
  bool Is_EventREADY();                        // 8104 [3]
  bool Is_ACQ_Running();                       // 8104 [2]

  bool Is_VME_BERR();                          // EF04 [2]
  bool Is_VME_OutBuffer_Full();                // EF04 [1]
  bool Is_VME_EventReady();                    // EF04 [0]


  /* ReadOut Functions */
  int Read_Event(unsigned long * rdata);                      // one Data word
  int DAQ_To_File(std::string filename, int NbrOfEvt);        // D32
  int DAQ_To_File_MBLT64(std::string filename, int NbrOfEvt); // MBLT64
  int Read_MBLT64(unsigned long * BLTData, unsigned long Nread); // MBLT64
                        // long statt int !!
  /* CAEN IRQ Commands */
  int Enable_IRQ_PCI(unsigned long Mask);
  int Disable_IRQ_PCI(unsigned long Mask);
  int Wait_IRQ_PCI(unsigned long Mask, unsigned long Timeout); // Timeout in Millisec
  int Wait_4_Event(bool * stop, unsigned long Timeout); // Timeout in Millisec

private:

  Handle m_handle;                              // Handle to hardware device
  InputData m_input_data;

  std::string ADCName;                          // arbitrary name
  unsigned long BaseAddr;                       // VME base address

  std::string BoardType;

  FW AMC_FPGA_FW[NCH];
  FW ROC_FPGA_FW;

  unsigned long BoardInfo;
  unsigned long ConfigRom[22];
};

#endif
