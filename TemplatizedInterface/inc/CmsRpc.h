/*
 * CmsRpc.h
 *
 *  Created on: May 12, 2016
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_CMSRPC_H_
#define TEMPLATIZEDINTERFACE_INC_CMSRPC_H_

#include <cstring>
#include "base/Global.h"
#include "Properties.h"
//#include "Plane.h"
#include <vector>

namespace Tomography {

class CmsRpc : public Properties {

//	static int fClusterSize;
public:
  CmsRpc();

  /*
   * @input : 1) Detector Name
   *          2) std::vector of Channels in each plane
   */
  CmsRpc(int moduleId,std::string rpcName,double zPos, int startId=31);
  CmsRpc(std::string name, std::vector<int> channelDimVector);
  virtual ~CmsRpc();
/*
  static void SetClusterSize(int clusterSize){fClusterSize = clusterSize;

  }
  static int GetClusterSize(){return fClusterSize;}
*/



};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_CMSRPC_H_ */
