/*
 * Event.h
 *
 *  Created on: May 16, 2017
 *      Author: rsehgal
 *
 *  This class will be used to store the information about event.
 *  1) Whether its genuine event or not
 *  2) Timing info at which the muon hit the top and bottom plane,
 *     this will be helpful, to check the direction of motion of muon.
 *  3)
 */

#ifndef TEMPLATIZEDINTERFACE_INC_EVENT_H_
#define TEMPLATIZEDINTERFACE_INC_EVENT_H_
#include <iostream>

namespace Tomography {

class Event {
	bool fValid;
	long fTopPlaneTiming;
	long fBottomPlaneTiming;
	bool fTopToBottom; // true implies muon is moving from top to bottom
	                   // false implies reverse direction i.e. bottom to top
	int fEventNum;

public:
	Event();
	Event(int eventnum,bool valid,long toptime,long bottomtime):fEventNum(eventnum),fValid(valid),fTopPlaneTiming(toptime),fBottomPlaneTiming(bottomtime){
		//std::cout<<"Event Num : " << fEventNum << " : Top : " << fTopPlaneTiming <<" :: Bottom : "<< fBottomPlaneTiming << std::endl;
		fTopToBottom = (fBottomPlaneTiming >= fTopPlaneTiming);
	}
	bool GetValidity(){return fValid;}
	bool IsMovingTopToBottom(){return fTopToBottom;}
	long GetTopPlaneTiming(){return fTopPlaneTiming;}
	long GetBottomPlaneTiming(){return fBottomPlaneTiming;}
	int GetEventNum(){return fEventNum;}
	void SetValidity(bool valid){fValid = valid;}
	void SetTopPlaneTiming(long time){fTopPlaneTiming=time;}
	void SetBottomPlaneTiming(long time){fBottomPlaneTiming=time;}
	void SetValidityAndTiming(bool valid, long toptime, long bottomtime){
		fValid = valid;
		fTopPlaneTiming = toptime;
		fBottomPlaneTiming = bottomtime;
	}
	void SetEventNum(int eventno){fEventNum = eventno;}


	~Event();
};

} /* namespace Tomography */

#endif /* TEMPLATIZEDINTERFACE_INC_EVENT_H_ */
