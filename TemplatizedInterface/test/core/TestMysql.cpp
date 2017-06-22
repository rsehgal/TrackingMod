/*
 * TestMysql.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: rsehgal
 */

#include "Database.h"

int main(){

	Tomography::Database d;
	d.Connect();
	//d.Query("select * from test");

	return 0;

}


