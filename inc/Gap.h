/*
 * Gap.h
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */

#ifndef INC_GAP_H_
#define INC_GAP_H_

#include <string>
#include "base/Global.h"
#include "ostream"

namespace Tracking{
class Gap{
private:
	std::string gapName;
	Precision current;
	Precision vset;
	Precision vmon;
	int station;
public:
	Gap(): gapName("testGap"), current(0.), vset(0.),vmon(0.), station(0){}
	Gap(std::string gapNm,
		Precision curr,
		Precision volt,
		Precision voltMon,
		int stat):
			gapName(gapNm), current(curr), vset(volt), vmon(voltMon),station(stat){}
	~Gap(){}
	TRACKING_INLINE
	std::string GetName(){return gapName;}
	TRACKING_INLINE
	Precision GetCurrent(){return current;}
	TRACKING_INLINE
	Precision GetVSet(){return vset;}
	TRACKING_INLINE
	Precision GetVMon(){return vmon;}
	TRACKING_INLINE
	int GetStation(){return station;}

	TRACKING_INLINE
	void SetName(std::string name){gapName = name;}
	TRACKING_INLINE
	void SetCurrent(Precision curr){current = curr;}
	TRACKING_INLINE
	void SetVoltage(Precision volt){vset = volt;}
	TRACKING_INLINE
	void Set(std::string name, Precision curr, Precision volt,Precision voltMon, int stat){
		gapName=name;
		current=curr;
		vset=volt;
		vmon = voltMon;
		station = stat;
	}
	TRACKING_INLINE
	friend std::ostream& operator<<(std::ostream& os, Gap const &gap) {
	os << "{"<<std::endl<<
			"GapName : " << gap.gapName<<std::endl<<
			"Current : " << gap.current << std::endl<<
			"Voltage : " << gap.vset << std::endl <<
			"}"<<std::endl;
	  return os;
	}

};//end of Gap Class


}// end of Tracking namespace


#endif /* INC_GAP_H_ */
