/*
 * Factory.h
 *
 *  Created on: July 22, 2018
 *      Author: rsehgal
 */

#ifndef TEMPLATIZEDINTERFACE_INC_FACTORY_H_
#define TEMPLATIZEDINTERFACE_INC_FACTORY_H_

#include "Properties.h"
#include "Detectors.h"

namespace Tomography {

using Detector = Properties;

template <class detType>
class Factory {
    
public:   
    Factory(){}
    static Detector* MakeInstance(int module, std::string name, double zcoord, int start);
    virtual ~Factory(){}
};

template<class detType>
Detector* Factory<detType>::MakeInstance(int module, std::string name, double zcoord, int start){
    return new detType(module,name,zcoord,start);
}


class DetectorFactory{

public:
    DetectorFactory(){}
    static Detector* MakeInstance(int module, std::string name, double zcoord, int start, std::string detType);    
    virtual ~DetectorFactory(){}


};

Detector* DetectorFactory::MakeInstance(int module, std::string name, double zcoord, int start, std::string detType){
        if(detType == "GLASS")
            return Factory<GlassRpc>::MakeInstance(module,name,zcoord,start);
        if(detType == "TRG")
            return Factory<TriggeringPlane>::MakeInstance(module,name,zcoord,start);

}

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_FACTORY_H_ */
