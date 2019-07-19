/*
 * PixelCombinationChecker.cpp
 *
 *  Created on: 19-Jul-2019
 *      Author: kavya
 */

#include "PixelCombinationChecker.h"

namespace Tomography {


TreeNode* PixelCombinationChecker::Insert(TreeNode *node, PixelCombination data)
{
	if (node == NULL)
	{
		TreeNode *temp = new TreeNode(data);

		temp->sleft = temp->sright = NULL;
		return temp;
	}

	if (data.sPixelCombinationSha == (node->sPixComb.sPixelCombinationSha))
	{
		node->counter++;
		node->sCombDataVector.push_back(node->sPixComb.sCombData);
		return node;
	}

	if (data.sPixelCombinationSha > node->sPixComb.sPixelCombinationSha)
	  {
		node->sright = Insert(node->sright, data);
	  }

	    else if (data.sPixelCombinationSha < node->sPixComb.sPixelCombinationSha)
	   {
	      	node->sleft = Insert(node->sleft, data);
	   }
}

/*

TreeNode* PixelCombinationChecker::Locate(TreeNode *node, PixelCombination data)
{

    if(node==NULL)
    {
        return NULL;
    }
    if(data.sPixelCombinationSha  > (node->sPixComb.sPixelCombinationSha))
    {

        return Locate(node->sright,data.sPixelCombinationSha); 				 // Search in the right sub tree.
    }
    else if(data.sPixelCombinationSha  < node->sPixComb.sPixelCombinationSha)
    {

        return Locate(node->sleft,data.sPixelCombinationSha );				 // Search in the left sub tree.
    }
    else
    {
        return node;
    }
}



*/

void PixelCombinationChecker::InOrder(TreeNode *node)
{
    if(node==NULL)
    {
        return;
    }
    InOrder(node->sleft);

    std::cout<<node->sPixComb.sPixelCombinationSha<<" " <<std::endl;

    InOrder(node->sright);
}

PixelCombinationChecker::PixelCombinationChecker() {
	// TODO Auto-generated constructor stub
	//fRoot = NULL;

}

PixelCombinationChecker::~PixelCombinationChecker() {
	// TODO Auto-generated destructor stub
}

} /* namespace Tomography */
