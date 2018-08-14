/*
 * TestReadOneEvent.cpp
 *
 *  Created on: Aug 12, 2018
 *      Author: raman
 */

#include "ReadOneEvent.h"
#include "base/Global.h"
#include "Files.h"

int main(){
	std::string filename = "ActualAndFittedHits.txt";

	//Opening the required file
	Tomography::Files::instance()->Open(filename,Tomography::operation::read);

	//Creating object of Event Reader
	Tomography::ReadOneEvent r(filename);

	//Accessing 10 events, but only one at a time
	for(int i = 0 ;  i < 10 ; i++){
		std::cout << "==================================" << std::endl;
		r.Read();
		r.Print();
	}

	//Closing the file
	Tomography::Files::instance()->Close(filename);

	/* Or below line,if one want to close all the open file,
	 * but be careful when using statement below,  this will
	 * forcibly close all the opened files, so use it when its
	 * really required.
	 *
	 * One good use case is calling it in the end of RunAction.
	 */
	//Tomography::Files::instance()->Close();

	return 0;
}

