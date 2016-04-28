/*
 * Hodoscope.h
 *
 *  Created on: Mar 24, 2016
 *      Author: rsehgal
 */

#include "Scintillator.h"
#include "RPC.h"
#include "base/Global.h"
#include <string>
#include <vector>
#include <algorithm>

#ifndef INC_HODOSCOPE_H_
#define INC_HODOSCOPE_H_

namespace Tracking{
class Hodoscope{
private:
    int fNumOfScintillatorPlane;
    int fNumOfRpc;
    std::vector<ScintillatorPlane*> fScintPlaneVector;
    std::vector<RPC*> fRpcVector;
    std::string fName;
public:
    Hodoscope();
    Hodoscope(int numOfScintillatorPlane, int numOfRpc);
    Hodoscope(std::string hodoName, int numOfScintillatorPlane, int numOfRpc);
    ~Hodoscope();
    //Hodoscope(int numOfHodoScope);

    int GetNumOfScintillatorPlane(){return fNumOfScintillatorPlane;}
    int GetNumOfRpc(){return fNumOfRpc;}
    void SetNumOfRpc(int numOfRpc){fNumOfRpc = numOfRpc;}
    void SetNumOfScintillatorPlane(int numOfScintillatorPlane){
        fNumOfScintillatorPlane = numOfScintillatorPlane;
    }

    void SetHodoScopeName(std::string hodoName){fName = hodoName;}
    std::string GetHodoScopeName(){return fName;}

    std::vector<RPC*> GetRpcVector(){return fRpcVector;}
    std::vector<ScintillatorPlane*> GetScintPlaneVector(){return fScintPlaneVector;}

}; // end of Hodoscope Class
}//end of Tracking namespace

#endif /* INC_HODOSCOPE_H_ */

