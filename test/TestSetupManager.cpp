/*
 * Scintillator.h
 *
 *  Created on: Mar 9, 2016
 *      Author: rsehgal
 *
 *  This test is for testing of SetupManager
 */
#include <iostream>
#include "Scintillator.h"
#include "RPC.h"
#include "SetupManager.h"
#include "Component.h"

using namespace Tracking;

int main(){
  ScintillatorPlane *s1 = new ScintillatorPlane(2,8,-105.,false,"TopPlane");
  ScintillatorPlane *s2 = new ScintillatorPlane(2,8,105.,false,"BottomPlane");
  RPC *rpc1 = new RPC(2,32,"FirstRpc",-75.);
  RPC *rpc2 = new RPC(3,32,"SecondRpc",-45.);
  RPC *rpc3 = new RPC(4,32,"ThirdRpc",-15.);
  RPC *rpc4 = new RPC(5, 32, "ThirdRpc", 15.);
  RPC *rpc5 = new RPC(6, 32, "SecondRpc", 45.);
  RPC *rpc6 = new RPC(7, 32, "FirstRpc", 75.);


  //std::cout<<"Name : "<< s1->GetName();
  
  SetupManager s;
  s.RegisterScintillatorPlane(s1);
  s.RegisterScintillatorPlane(s2);
  s.RegisterRpc(rpc1);
  s.RegisterRpc(rpc2);
  s.RegisterRpc(rpc3);
  s.RegisterRpc(rpc4);
  s.RegisterRpc(rpc5);
  s.RegisterRpc(rpc6);

  // s.Register<ScintillatorPlane,false>(s1);
  // s.Register<ScintillatorPlane,false>(s2);
  // s.Register<RPC,true>(rpc1);
  // s.Register<RPC,true>(rpc2);
  // s.Register(rpc3);
  // s.Register(rpc4);
  // s.Register(rpc5);
  // s.Register(rpc6);

  //s.GetComponentsName();
  //std::cout<<"SetupManager Size  : "<< s.GetSize() << std::endl;
  s.GetComponentsName();

}