/*
 * Data.h
 *
 *  Created on: Jul 5, 2017
 *      Author: rsehgal
 */

#ifndef SIMPLEGEANT4PROGRAMS_PROG1_INC_DATA_H_
#define SIMPLEGEANT4PROGRAMS_PROG1_INC_DATA_H_

#include <iostream>
#include "Tree.h"
#include "base/Global.h"

void InitializeTree(){

	Tracking::Tree *tree = Tracking::Tree::instance();
	tree->SetTreeDefaultParameters();
	tree->InitializeTreeForWriting();
	//Tracking::Channel b;
	//tree->CreateBranch<decltype(b)>("TestBranch",b);


/*
	for(int i = 2 ; i < 6 ; i++){

	for(int brNum = 32 ; brNum < 96 ; brNum++){
		Tracking::Channel b;

		std::string bName = "Module"+std::to_string(i)+"_LE_CH"+std::to_string(brNum);
		tree->CreateBranch<decltype(b)>(bName.c_str(),b);
	}

	}
*/


}

void CreateBranch(std::string bName,Tracking::Channel b){
	Tracking::Tree::instance()->CreateBranch<decltype(b)>(bName.c_str(),b);
}


#endif /* SIMPLEGEANT4PROGRAMS_PROG1_INC_DATA_H_ */
