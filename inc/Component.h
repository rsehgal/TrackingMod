/*
 * Scintillator.h
 *
 *  Created on: Mar 30, 2016
 *      Author: rsehgal
 *
 *
 * This is basically a blank class which will be inherited by all other
 * components like Scintillator, ScintillatorPlane, and RPC
 * 
 * This is written for SetupManager, so that it can register any type
 * of component by receiving that in the reference of Component class
 */

#ifndef INC_COMPONENT_H_
#define INC_COMPONENT_H_
#include <TEveGeoShape.h>

class Tree;
class Scintillator;

namespace Tracking{
    class Component{
    public:
        Component(){}
        virtual std::vector<Scintillator*> GetScintillatorPlane()  {}
        
        /*virtual TEveGeoShape* GetScintillatorEveGeoShape(){}
        virtual int GetNumOfScintillators(){return 0;}
        virtual bool GetScintHit(){return false;}

        template<bool ForRpc>
        void DetectAndSetHit(Tree&t, int evNo){}*/

        virtual std::string GetName(){}

    };
}//end of Tracking namespace
#endif