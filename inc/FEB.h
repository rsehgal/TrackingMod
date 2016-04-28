/*
 * FEB.h
 *
 *  Created on: Mar 8, 2016
 *      Author: rsehgal
 */

#ifndef INC_FEB_H_
#define INC_FEB_H_

#include "base/Global.h"
#include <vector>
#include <algorithm>

namespace Tracking{
/*
*
* Each FEB is having 4 Chips, and each Chip is having 8 Channels
* So each FEB contains 32 channels,
* Hence for CMS Rpcs, which are having 96 channels, we need 3 FEB
* But for glass RPC, which are having 32X32 strips we needs only
* 2 FEB's'
*
* So here we will make two class, one for FEB, and one for CHIP
*/
//class Chip;

class Chip{
private:
	Precision threshold;

public:
	Chip(): threshold(0.){}
	Chip(Precision thr): threshold(thr) {}
	~Chip(){}
	void SetThreshold(Precision thr){threshold=thr;}
	Precision GetThreshold(){return threshold;}
	TRACKING_INLINE
	friend std::ostream& operator<<(std::ostream& os, Chip const &chip) {
		os << "{"<<std::endl<<
				"Threshold : " << chip.threshold << std::endl<<
				"}"<<std::endl;
		  return os;
		}

};


class FEB{
private:
	int numOfChips;
	std::vector<Chip*> feb;



public:
	FEB():numOfChips(4){InitializeFeb();}
	~FEB(){}
	TRACKING_INLINE
	void SetNumOfChips(int numChips){
		numOfChips = numChips;
		InitializeFeb();
	}
	TRACKING_INLINE
	int GetNumOfChips(){return numOfChips;}

	void InitializeFeb(){
		for(int i=0; i < numOfChips ; i++)
			feb.push_back(new Chip());
	}

	void Set(Precision th1, Precision th2, Precision th3, Precision th4){
		feb[0]->SetThreshold(th1);
		feb[1]->SetThreshold(th2);
		feb[2]->SetThreshold(th3);
		feb[3]->SetThreshold(th4);
	}

	int CalculateNumOfChips(){

		return feb.size();
	}

	Chip GetChip(int i){
		return *feb[i];
	}
	TRACKING_INLINE
	friend std::ostream& operator<<(std::ostream& os, FEB  &feb) {

		for(int i = 0 ; i < feb.CalculateNumOfChips() ; i++ ){
			os << "{"<<std::endl<<
					"chip no : "<<i<<std::endl<<
					feb.GetChip(i)<<
					"}"<<std::endl;
		}
		return os;
	}


};


}//end of Tracking namespace



#endif /* INC_FEB_H_ */
