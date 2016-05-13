/*
 * GlassRpc.h
 *
 *  Created on: May 12, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_PROPERTIES_H_
#define TEMPLATIZEDINTERFACE_INC_PROPERTIES_H_

#include <cstring>
#include "base/Global.h"
#include <vector>
#include <algorithm>

namespace Tomography {

class Properties {
private:
  int fTotalNumOfChannels;
  std::string fName;
  int fDim;
  std::vector<int> fChannelsInDim; //where vector index represent dimension number


public:
  Properties();
  Properties(std::string name,std::vector<int> channelsInDim);
  virtual ~Properties();

  void SetName(std::string name){fName = name;}
  void SetChannelDimVector(std::vector<int> channelsInDim ) {fChannelsInDim = channelsInDim;}

  std::string GetName(){return fName;}
  int GetTotalNumOfChannels(){return fTotalNumOfChannels;}
  int GetReadOutDimension(){return fDim;} //1, 2, or 3
  int GetNumOfChannelsInDimension(int dim){return fChannelsInDim[dim];}

};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_GLASSRPC_H_ */
