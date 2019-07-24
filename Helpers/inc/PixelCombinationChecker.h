/*
 * PixelCombinationChecker.h
 *
 *  Created on: 19-Jul-2019
 *      Author: kavya
 */

#ifndef HELPERS_INC_PIXELCOMBINATIONCHECKER_H_
#define HELPERS_INC_PIXELCOMBINATIONCHECKER_H_


#include "HelperStructs.h"
#include <iostream>
#include <cstring>

namespace Tomography {


class PixelCombinationChecker {
	//TreeNode *fRoot;

public:
	static unsigned int fTotalNumOfRegTracks;

	PixelCombinationChecker();

	//TreeNode* Insert(TreeNode *node, PixelCombination data);
	TreeNode* Insert(TreeNode *node, PixelCombination *data, bool insert=true);
	//TreeNode* Locate(TreeNode *node, PixelCombination data);
	TreeNode* Locate(TreeNode *node, std::string combStringSha);
	void InOrder(TreeNode *node);
	static void CalcTotalNumOfRegisteredTracks(TreeNode *node);

	virtual ~PixelCombinationChecker();



};

} /* namespace Tomography */

#endif /* HELPERS_INC_PIXELCOMBINATIONCHECKER_H_ */
