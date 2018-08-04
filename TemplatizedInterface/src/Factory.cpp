/*#include "Factory.h"

typedef Tomography::Properties Detector;

namespace Tomography{

template<class detType>
Properties *Factory<detType>::MakeInstance(int module, std::string name, double zcoord, int start){
    return new detType(module,name,zcoord,start);
}

}*/