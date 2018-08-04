/*
 * Database.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: rsehgal
 */

#ifdef USE_MYSQL
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

	fVerbose = false;
	mysql_init(&mysql);

}

Database::~Database() {
	// TODO Auto-generated destructor stub
	//mysql_free_result(res);
	  // mysql_close(conn);
}

int Database::Connect(){
	conn=mysql_real_connect(&mysql, "127.0.0.1", fUsername.c_str(), fPasswd.c_str(), fDbName.c_str(), 0, 0, 0);
	if(conn == NULL){
		std::cout<<mysql_error(&mysql)<< std::endl << std::endl;
		return 1;
	}


}

int Database::Delete(std::string tablename){
	std::string query = "DELETE FROM "+tablename;
	return Query(query);
}

int Database::Insert(std::string query){
	return Query(query);
}

int Database::Select(std::string query){
	return Query<true>(query);
}

int Database::GetNumOfRows(std::string tablename){
	std::string query = "SELECT count(*) FROM "+tablename;
	return Count(query);
}

int Database::GetNumOfColumns(std::string tablename){
	std::string query = "SELECT count(*) FROM INFORMATION_SCHEMA.Columns where TABLE_NAME = '"+tablename+"'";
	return Count(query);
}

int Database::Count(std::string query){
	int queryReturnCode = Query<true>(query);
	row=mysql_fetch_row(res);
	return atoi(row[0]);
}


/*
template<bool select>
int Database::Query(std::string query){

//	fQueryState = mysql_query(conn,fQuery.c_str());
	fQueryState = mysql_query(conn,query.c_str());
	if(fQueryState!=0)
	   {
	      std::cout << mysql_error(conn) << std::endl << std::endl;
	      return 1;
	   }
	if(select){
		res = mysql_store_result(conn);

		if(fVerbose)
			PrintQueryOutput();
	}
}
*/

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

#endif
