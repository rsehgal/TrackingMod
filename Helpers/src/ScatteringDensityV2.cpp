/*
 * ScatteringDensityV2.cpp
 *
 *  Created on: 02-Jan-2019
 *      Author: rsehgal
 */

#include "ScatteringDensityV2.h"

namespace Tomography {
ScatteringDensityV2 *ScatteringDensityV2::s_instance = 0;
bool ScatteringDensityV2::iniflag = false;

ScatteringDensityV2* ScatteringDensityV2::instance() {
        if (!s_instance)
          s_instance = new ScatteringDensityV2;

        return s_instance;
    }

ScatteringDensityV2* ScatteringDensityV2::Create() {
	return instance();
}
ScatteringDensityV2::ScatteringDensityV2() {
	// TODO Auto-generated constructor stub
	sNumOfVoxels = Tomography::evolution::Voxelator::instance()->GetTotalNumberOfVoxels();
	iniflag = false;

}

ScatteringDensityV2::~ScatteringDensityV2() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
