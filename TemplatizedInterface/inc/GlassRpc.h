/*
 * GlassRpc.h
 *
 *  Created on: May 12, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_GLASSRPC_H_
#define TEMPLATIZEDINTERFACE_INC_GLASSRPC_H_

#include <cstring>
#include "base/Global.h"
#include "Properties.h"

namespace Tomography {

class GlassRpc : public Properties {
public:
  GlassRpc();
  GlassRpc(std::string name, std::vector<int> channelDimVector);
  virtual ~GlassRpc();


};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_GLASSRPC_H_ */
