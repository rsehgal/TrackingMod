/*
 * Database.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: rsehgal
 */

#include "Database.h"
#include <iostream>
#include <iomanip>

namespace Tomography {

Database::Database() {
	// TODO Auto-generated constructor stub
	fUsername = "raman";
	fPasswd = "sehgal";
	fDbName = "tomo";
	fQueryState = 0;

	fVerbose = true;
	mysql_init(&mysql);

}

Database::~Database() {
	// TODO Auto-generated destructor stub
	mysql_free_result(res);
	   mysql_close(conn);
}

int Database::Connect(){
	conn=mysql_real_connect(&mysql, "127.0.0.1", fUsername.c_str(), fPasswd.c_str(), fDbName.c_str(), 0, 0, 0);
	if(conn == NULL){
		std::cout<<mysql_error(&mysql)<< std::endl << std::endl;
		return 1;
	}


}

int Database::Query(std::string query){

//	fQueryState = mysql_query(conn,fQuery.c_str());
	fQueryState = mysql_query(conn,query.c_str());
	if(fQueryState!=0)
	   {
	      std::cout << mysql_error(conn) << std::endl << std::endl;
	      return 1;
	   }
	res = mysql_store_result(conn);

	if(fVerbose)
		PrintQueryOutput();
}

void Database::PrintQueryOutput(){
	std::cout<<"MySQL Tables in mysql database."<< std::endl << std::endl;
	   while((row=mysql_fetch_row(res))!=NULL)
	   {
	      //std::cout << left;
	      std::cout << std::setw(18) << row[0]
				    << std::setw(18) << row[1]
	                //<< std::setw(18) << row[2]
	                //<< std::setw(18) << row[3]
					<< std::endl;
	   }
}

} /* namespace Tomography */
