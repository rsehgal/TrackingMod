/*
 * Scintillator.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: rsehgal
 */

#include "Hodoscope.h"
#include "sstream"
#include "string"

/*
* There should a Hodoscope configuration file, 
* we will call it TDML (Tomography Description Markup Language),
* which is basically a XML file, describing the full HodoScope
* setup.
* like 1) Number of Scintillator
*      2) Number of Rpc
*      3) Module at which Scintillator is connected
*      4) Module at which Rpc is connected
*      5) Channel information associated Scintillator and Rpc
*/

namespace Tracking{
Hodoscope::Hodoscope():fNumOfScintillatorPlane(0), fNumOfRpc(0){}
Hodoscope::Hodoscope(int numOfScintillatorPlane, int numOfRpc):
    fNumOfScintillatorPlane(numOfScintillatorPlane),
    fNumOfRpc(numOfRpc){
        //std::stringstream ss;

        //Inserting ScintillatorPlanes in the Hodoscope
        for(int i = 0 ; i < fNumOfScintillatorPlane ; i++){
            std::stringstream ss;
            ss << "Plane_" << i ; 
            fScintPlaneVector.push_back(new ScintillatorPlane(2, 8, ss.str() ));
        }

        //Inserting RPCs in the Hodoscope
        for(int i = 0 ; i < fNumOfRpc ; i++){
            std::stringstream ss;
            ss << "RPC_" << i;
            fRpcVector.push_back(new RPC(i+2, 96, ss.str() ));
            //fRpcVector.push_back(new RPC(2, 6, ss.str(),23));
        }
    }

Hodoscope::Hodoscope(std::string hodoName, int numOfScintillatorPlane, int numOfRpc):
Hodoscope(numOfScintillatorPlane,numOfRpc){
    fName = hodoName;
}

Hodoscope::~Hodoscope(){}

}//end of Tracking namespace
