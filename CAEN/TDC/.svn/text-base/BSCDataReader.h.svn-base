/*
DataReader.h
Reads data from specified module and stores it in ROOT Tree

*/
#ifndef _DATAREADER_H_
#define _DATAREADER_H_

#include <map>
#include <vector>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include "BSCModule.h"
#include "ModuleV2718.h"
#include "ModuleV1190.h"

using std::map;
using std::vector;
using std::string;

//const unsigned ChannelsPerModules = 8;

typedef std::map< std::string, BSCModule * > ModulesMap;
typedef std::map< std::string, BSCModule * >::iterator ModulesMapIter;
typedef std::vector< BSCModule * > ModulesContainer;
typedef std::vector< BSCModule * >::iterator ModulesContainerIter;

class BSCDataReader{
 public:
  BSCDataReader();
  virtual ~BSCDataReader();
  void Initialize( string aIniFileName );
  void AddModule( BSCModule * aModule );
  unsigned GetNumberOfModules(){ return fModules.size(); };
  void SetNumberOfSpills( unsigned aSpills );//{ fSpillNum = aSpills; };
  void Run();

 private:
  vector< BSCModule * > fModules; 
  vector< TBranch* > fBranchVector;
  TFile * fDataFile;
  TTree * fDataTree;
  unsigned fSpillNum;
  bool fStopFlag;
  int  fModuleCount;
  //TODO: Dynamically allocate the needed modules
  ModuleVmeInterface * _vmeInt;
  ModulesMap _modMap;
  ModulesContainer _modules;
};

#endif
