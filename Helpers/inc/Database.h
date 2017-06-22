/*
 * Database.h
 *
 *  Created on: Jun 22, 2017
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_DATABASE_H_
#define HELPERS_INC_DATABASE_H_

#include <string>
#include <mysql.h>

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
	int Query(std::string query);
	void PrintQueryOutput();
};

} /* namespace Tomography */

#endif /* HELPERS_INC_DATABASE_H_ */
