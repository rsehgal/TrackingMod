/*
 * TestVisualization.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: rsehgal
 */
#include "VisualizationHelper.h"


int main(){
  Tracking::VisualizationHelper v;
  ScintillatorPlane *s1 = new ScintillatorPlane(2,8,-105.,false,"TopPlane");
  ScintillatorPlane *s2 = new ScintillatorPlane(2,8,105.,false,"BottomPlane");
  RPC *rpc1 = new RPC(2,32,"FirstRpc",-75.);
  RPC *rpc2 = new RPC(3,32,"SecondRpc",-45.);
  RPC *rpc3 = new RPC(4,32,"ThirdRpc",-15.);
  RPC *rpc4 = new RPC(5, 32, "ThirdRpc", 15.);
  RPC *rpc5 = new RPC(6, 32, "SecondRpc", 45.);
  RPC *rpc6 = new RPC(7, 32, "FirstRpc", 75.);
  Target *t = new Target();

  v.Register(s1);
  v.Register(s2);
  v.Register(rpc1);
  v.Register(rpc2);
  v.Register(rpc3);
  v.Register(rpc4);
  v.Register(rpc5);
  v.Register(rpc6);
  v.Register(t);
  v.Show();

}

