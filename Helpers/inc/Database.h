/*
 * Database.h
 *
 *  Created on: Jun 22, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_DATABASE_H_
#define HELPERS_INC_DATABASE_H_

#ifdef USE_MYSQL

#include <string>
#include <mysql.h>
#include <iostream>

namespace Tomography {

class Database {
	//MYSQL related data members
	MYSQL *conn, mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	int fQueryState;

	//MySql database details
	std::string fDbName;
	std::string fUsername;
	std::string fPasswd;
	std::string fQuery;

	bool fVerbose;

public:
	Database();
	virtual ~Database();
	int Connect();

	template<bool select=false>
	int Query(std::string query);

	int Insert(std::string query);
	int Delete(std::string tablename);
	int Select(std::string query);
	int Count(std::string query);
	int GetNumOfColumns(std::string tablename);
	int GetNumOfRows(std::string tablename);
	void PrintQueryOutput();
};

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


} /* namespace Tomography */
#endif //MYSQL ifdef
#endif /* HELPERS_INC_DATABASE_H_ */
