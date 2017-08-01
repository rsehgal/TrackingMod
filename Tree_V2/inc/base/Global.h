/*
 * Global.h
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */

#ifndef BASE_GLOBAL_H_
#define BASE_GLOBAL_H_

#include <vector>
#include <algorithm>

namespace Tracking {

typedef std::vector<unsigned int> HitsPerChannel_v; //Multihit information for channel
typedef HitsPerChannel_v Channel;
typedef std::vector<Channel*> Channel_v; //Basically hitsPerChannel for all the events
typedef Channel_v Module;
typedef std::vector<Module> ModuleVector;

#define scintMax 500

#define scint_multiplicity_max 2

#define Precision double
#define TRACKING_INLINE inline

#define PI 3.14159265359

class Global{
public:
	Global(){}
static double GenRandomDet(double min, double max) {
  return (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (max - min) + min;
}
};

}//end of Tracking namespace



#endif /* BASE_GLOBAL_H_ */
