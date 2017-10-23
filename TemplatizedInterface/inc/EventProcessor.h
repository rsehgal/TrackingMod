/*
 * EventProcessor.h
 *
 *  Created on: Aug 24, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_EVENTPROCESSOR_H_
#define HELPERS_INC_EVENTPROCESSOR_H_

#include "Coordinates.h"
#include <TGeoMatrix.h>
#include "base/Global.h"

#ifdef USE_EVE
#include "HittedPixel.h"
#endif

#include "TGeoBBox.h"
#include "visualizer/Eve/Singleton.h"
#include "VisualizationHelper.h"
#include "TThread.h"
#include "LinesAngle.h"
#include "TEveStraightLineSet.h"
#include "TRandom.h"
#include "Tree.h"
#include "base/Vector3D.h"
#include "Properties.h"
#include "SetupManager.h"
 #include "visualizer/Eve/Singleton.h"
#include "Track.h"

using Detector = Tomography::Properties;

namespace Tomography {

class EventProcessor {
	bool fGenuineEvent;
	Tomography::Track fIncoming;
	Tomography::Track fOutgoing;

public:
	EventProcessor();
	virtual ~EventProcessor();

	bool IsGenuineEvent(){return fGenuineEvent;}
	void ProcessEvent(int evNo);
	Tomography::Track GetIncomingTrack(){return fIncoming;}
	Tomography::Track GetOutgoingTrack(){return fOutgoing;}

};

} /* namespace Tomography */

#endif /* HELPERS_INC_EVENTPROCESSOR_H_ */
