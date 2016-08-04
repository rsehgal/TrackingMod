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
//#include "Plane.h"
#include <vector>

namespace Tomography {

class GlassRpc : public Properties {

	//static int fClusterSize;
public:
  GlassRpc();

  /*
   * @input : 1) Detector Name
   *          2) std::vector of Channels in each plane
   */
  GlassRpc(int moduleId,std::string rpcName,double xPos, double yPos, double zPos, double theta, int startId=31);
  GlassRpc(int moduleId,std::string rpcName,double zPos, int startId=31);
  GlassRpc(std::string name, std::vector<int> channelDimVector);
/*
  static void SetClusterSize(int clusterSize){fClusterSize = clusterSize;

  }
  static int GetClusterSize(){return fClusterSize;}
*/
  virtual ~GlassRpc();



};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_GLASSRPC_H_ */
