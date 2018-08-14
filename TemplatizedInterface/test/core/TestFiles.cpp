/*
 * TestFiles.cpp
 *
 *  Created on: Aug 14, 2018
 *      Author: raman
 */

#include "Files.h"
#include "base/Global.h"

int main(){
	//std::cout << Tomography::Files::instance() << std::endl;
    Tomography::Files::instance()->Open("Hello.txt",Tomography::operation::write);

    Tomography::Files::instance()->Close();

	return 0;
}





