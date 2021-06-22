/*
**	Filename : CharacterizationFileReader.h
**	2021-06-19
**	username : raman
*/
#ifndef CharacterizationFileReader_h
#define CharacterizationFileReader_h

#include "includes.h"
#include <string>
#include <climits>
struct Event {
  UShort_t sBrch;    //! board #  and channel number ( its packed in as follows )     //! board*16 + chno.
  UInt_t sQlong;     //! integrated charge in long gate 88 nsec
  ULong64_t sTstamp; //! time stamp in pico sec.
  UInt_t sTime;      //! real computer time in sec
  int sDelt;
  int sActualSourcePos;

  Event(UShort_t brch, UInt_t qlong, ULong64_t tstamp, UInt_t tme, int delt)
  {
    sBrch            = brch;
    sQlong           = qlong;
    sTstamp          = tstamp;
    sTime            = tme;
    sDelt            = delt;
    sActualSourcePos = 100000;
  }
  Event(UShort_t brch, UInt_t qlong, ULong64_t tstamp, UInt_t tme, int delt, int actualSourcePos)
  {
    sBrch            = brch;
    sQlong           = qlong;
    sTstamp          = tstamp;
    sTime            = tme;
    sDelt            = delt;
    sActualSourcePos = actualSourcePos;
  }

  int GetDelT() const { return sDelt; }

  UInt_t GetQNear() { return (sQlong >> 16); }

  UInt_t GetQFar()
  {
    unsigned short int maxU_16bits = USHRT_MAX;
    UInt_t maskingVal              = maxU_16bits;
    return (sQlong & maskingVal);
  }

  float GetQMean() { return sqrt(GetQNear() * GetQFar()); }

  ULong64_t GetTNear() const { return sTstamp; }

  ULong64_t GetTFar() { return (sTstamp - sDelt); }

  int GetActualSourcePosition() const { return sActualSourcePos; }

  double GetExpectedGammaPos() {}

  double GetTempAttenuationCoeffForAnEvent()
  {
    return (1. / (2. * sActualSourcePos)) * log((1. * GetQFar()) / (1. * GetQNear()));
  }

  void Print()
  {
    std::cout << "Branch : " << sBrch << std::endl
              << "QNear : " << GetQNear() << std::endl
              << "QFar : " << GetQFar() << std::endl
              << "QLong : " << sQlong << std::endl
              << "TNear : " << GetTNear() << std::endl
              << "TFar : " << GetTFar() << std::endl
              << "DelT : " << GetDelT() << std::endl
              << "ActualSourcePos : " << GetActualSourcePosition() << std::endl;
  }

  /*void WriteToFile(std::string filename)
  {
    std::ofstream outfile(filename);
    outfile << GetQNear() << "," << GetQFar() << "," << GetDelT() << "," << GetActualSourcePosition() << std::endl;
  }*/
};

class CharacterizationFileReader {
private:
  TFile *fp;
  TTree *fTree;
  int fDelTOffset;

  std::string fFileName;
  UShort_t fBrch;    //! board #  and channel number ( its packed in as follows )     //! board*16 + chno.
  UInt_t fQlong;     //! integrated charge in long gate 88 nsec
  ULong64_t fTstamp; //! time stamp in pico sec.
  UInt_t fTime;      //! real computer time in sec
  int fDelt;
  std::vector<unsigned int> fEventNumVec;

public:
  CharacterizationFileReader();
  CharacterizationFileReader(std::string filename);
  void ConnectTree(std::string treeName = "ftree");

  /*Required Getters*/
  TTree *GetTree() const;
  TFile *GetFilePointer() const;
  Event *GetEvent(unsigned int evNo);
  int GetActualPosition();
  std::string GetBarName();
  unsigned int GetTotalNumOfEvents();
  void CalculateDelTOffset();
  int GetDelTOffset() const;
  double GetMeanAttenuationCoeff(unsigned int numOfEvents = 0);

  /*Functions related to Training and Testing*/
  float fTestProportion;
  void RandomizeIt();
  std::vector<Event *> GetTrainingData();
  std::vector<Event *> GetTestingData();
  std::vector<Event *> GetAllEvents(unsigned int numOfEvents = 0);
  /*Function to Print few (10) random event number */
  void PrintRandom();
};

#endif
