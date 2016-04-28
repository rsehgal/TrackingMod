/*
 * InputFiles.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */
#include <cstring>
#include "InputFiles.h"
//class InputFiles;
namespace Tracking{
InputFiles::InputFiles():
      mappingFile("mapping.txt"),
      daqInfoFile("daqinfo"),
      runFile("run"),
      offlineCfgFile("offline.cfg"),
      iniFile("rpc.ini"),
      rootFile("test.root"),
      etaPartitionFile("trg_eta.txt"),
      largeAcceptanceFile("large.cfg"),
      smallAcceptanceFile("small.cfg"),
      twoRpcFile("tworpc.cfg"){}

InputFiles::InputFiles(const std::string root):
        mappingFile("mapping.txt"),
        daqInfoFile("daqinfo"),
        runFile("run"),
        offlineCfgFile("offline.cfg"),
		iniFile("rpc.ini"),
		rootFile(root),
		etaPartitionFile("trg_eta.txt"),
		largeAcceptanceFile("large.cfg"),
		smallAcceptanceFile("small.cfg"),
		twoRpcFile("tworpc.cfg"){}

InputFiles::InputFiles(const std::string mapping, const std::string daqInfo, const std::string run,
                       const std::string offlineCfg, const std::string ini, const std::string root,
                       const std::string etapartitionfile, const std::string largeacceptance,
                       const std::string smallacceptance, const std::string tworpc):
      mappingFile(mapping),
      daqInfoFile(daqInfo),
      runFile(run),
      offlineCfgFile(offlineCfg),
      iniFile(ini),
      rootFile(root),
      etaPartitionFile(etapartitionfile),
      largeAcceptanceFile(largeacceptance),
      smallAcceptanceFile(smallacceptance),
      twoRpcFile(tworpc){}

} // end of Tracking namespace
