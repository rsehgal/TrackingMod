/*
 * PixelCombinationChecker.cpp
 *
 *  Created on: 19-Jul-2019
 *      Author: kavya
 */

#include "PixelCombinationChecker.h"

namespace Tomography {

unsigned int PixelCombinationChecker::fTotalNumOfRegTracks = 0;

/*
TreeNode* PixelCombinationChecker::Insert(TreeNode *node, PixelCombination data)
{
	std::cout << " Node : " << node << std::endl;
	if (node == NULL)
	{
		std::cout << "Creating New Node ......" << std::endl;
		TreeNode *temp = new TreeNode(data);

		temp->sleft = temp->sright = NULL;
		std::cout <<"Returning the created node.........." << std::endl;
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
		std::cout << "Going to Right Subtree......." << std::endl;
		node->sright = Insert(node->sright, data);
	  }

	    else if (data.sPixelCombinationSha < node->sPixComb.sPixelCombinationSha)
	   {
	    	std::cout << "Going to Left Subtree......." << std::endl;
	      	node->sleft = Insert(node->sleft, data);
	   }
}
*/

TreeNode* PixelCombinationChecker::Insert(TreeNode *node, PixelCombination *data, bool insert)
{
	//std::cout << " Node : " << node << std::endl;
	if (insert) {
		if (node == NULL) {
			//std::cout << "Creating New Node ......" << std::endl;
			TreeNode *temp = new TreeNode(data);
			temp->counter++;
			TreeNode::sNodeCounter++;

			temp->sleft = temp->sright = NULL;
			//std::cout <<"Returning the created node.........." << std::endl;
			return temp;
		}
	}
	else{
		if(node==NULL)
			return NULL;
	}

	TreeNode *returnPointer = NULL;

	if (data->sPixelCombinationSha == (node->sPixComb->sPixelCombinationSha))
	{
		returnPointer = node;
		std::cout << "Duplicate found......" << std::endl;
		node->counter++;

		if(insert)
			node->sCombDataVector.push_back(node->sPixComb->sCombData);

		return node;
		//return returnPointer;
	}

	if (data->sPixelCombinationSha > node->sPixComb->sPixelCombinationSha)
	  {
		//std::cout << "Going to Right Subtree......." << std::endl;
		if(insert)
			node->sright = Insert(node->sright, data, insert);
		else
			return Insert(node->sright, data, insert);
	  }

	    else if (data->sPixelCombinationSha < node->sPixComb->sPixelCombinationSha)
	   {
	    	//std::cout << "Going to Left Subtree......." << std::endl;
	    	if(insert)
	    		node->sleft = Insert(node->sleft, data, insert);
	    	else
	    		return Insert(node->sleft, data, insert);
	   }

	return node;
	//return returnPointer;
}

TreeNode* PixelCombinationChecker::Locate(TreeNode *node, std::string combStringSha){
	//return Insert(node,new PixelCombination(combStringSha,new CombData()),false);
	return Insert(node,new PixelCombination(combStringSha,NULL),false);
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


void PixelCombinationChecker::CalcTotalNumOfRegisteredTracks(TreeNode *node){
	if(node==NULL)
	{
	   return;
	}
	CalcTotalNumOfRegisteredTracks(node->sleft);
	fTotalNumOfRegTracks += node->counter;
	CalcTotalNumOfRegisteredTracks(node->sright);

	//return fTotalNumOfRegTracks;
}

void PixelCombinationChecker::InOrder(TreeNode *node)
{
    if(node==NULL)
    {
        return;
    }
    InOrder(node->sleft);

    std::cout<<node->sPixComb->sPixelCombinationSha<<" " <<std::endl;

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
