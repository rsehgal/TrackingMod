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
	std::string query = "select * from scatteringdata";
	d.Select(query);

	int numOfRecords = d.GetNumOfRows("scatteringdata");
	std::cout<<"Num of Records : " << numOfRecords << std::endl;

	int numOfColumns = d.GetNumOfColumns("scatteringdata");
	std::cout<< "Num of Columns : " << numOfColumns << std::endl;

	double energy = 2.6;
	std::string material = "Fe";
	double thickness = 20.5;
	double fwhm = 30.6;
	query = "insert into scatteringdata values("+std::to_string(energy)+",'"+material+"',"+std::to_string(thickness)+","+std::to_string(fwhm)+")";
	std::cout << "Final Query : " << query << std::endl;

	d.Insert(query);

	//d.Delete("scatteringdata");
	return 0;

}


