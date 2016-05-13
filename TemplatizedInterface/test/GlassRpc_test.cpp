/*
 * GlassRpc_test.cpp
 *
 *  Created on: May 12, 2016
 *      Author: rsehgal
 */

#include "GlassRpc.h"
#include <vector>
#include <iostream>
using namespace Tomography;

int main(){
  std::vector<int> channelVector;
  channelVector.push_back(32);
  channelVector.push_back(32);
  GlassRpc rpc1("FirstRpc",channelVector);
  std::cout<<"Name : "<< rpc1.GetName() << std::endl;
  std::cout<<"Total Num of Channels : "<< rpc1.GetTotalNumOfChannels() << std::endl;
}


