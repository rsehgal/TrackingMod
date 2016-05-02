/*
 * TestCmsRpc.cpp
 *
 *  Created on: May 2, 2016
 *      Author: rsehgal
 */

#include <iostream>
#include <CmsRpc.h>

using namespace Tracking;

int main(){
  CmsRpc *rpc1 = new CmsRpc(2,"FirstCMSRpc",0);
  std::cout<<"Name : "<< rpc1->GetName() << std::endl;
  rpc1->PrintStrips();

  return 0;
}



