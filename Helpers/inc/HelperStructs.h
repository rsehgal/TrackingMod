/*
 * HelperStructs.h
 *
 *  Created on: 19-Jul-2019
 *      Author: kavya
 */

#ifndef HELPERS_INC_HELPERSTRUCTS_H_
#define HELPERS_INC_HELPERSTRUCTS_H_

#include "Track.h"
#include "base/Vector3D.h"

namespace Tomography{

using HitPointVector = std::vector<Tracking::Vector3D<double>>;

struct CombData{
	Track sTrack;
	HitPointVector sHitPointVector;
	CombData(){}
	CombData(Track tr, HitPointVector hitPtVector){
		sTrack = tr;
		sHitPointVector = hitPtVector;
	}

	void Set(Track tr, HitPointVector hitPtVector){
		sTrack = tr;
		sHitPointVector = hitPtVector;
	}
};

struct PixelCombination{
	std::string sPixelCombinationSha;
	CombData sCombData;
	PixelCombination(){}
	PixelCombination(std::string pixCombSha,CombData combData){
		sPixelCombinationSha = pixCombSha;
		sCombData = combData;
	}

	std::string GetCombinationName()const {return sPixelCombinationSha;}

};

struct TreeNode{
	int counter;
	TreeNode *sleft;
	TreeNode *sright;
	PixelCombination sPixComb;
	TreeNode(){
		counter = 0;
	}

	TreeNode(PixelCombination pixComb){
		sPixComb = pixComb;
	}
	std::vector<CombData> sCombDataVector;
};
}



#endif /* HELPERS_INC_HELPERSTRUCTS_H_ */
