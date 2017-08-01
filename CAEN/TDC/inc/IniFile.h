#ifndef __INIFILE_H_
#define __INIFILE_H_

#include <fstream>
#include <map>
#include <string>
#include <sstream>

const int INI_OK = 0;
// File Errors
const int INI_ERROR_CANNOT_OPEN_READ_FILE = 10;
// Format Errors
const int INI_ERROR_WRONG_FORMAT = 20;
const int INI_ERROR_WRONG_GROUP_FORMAT = 21;
const int INI_ERROR_MISSING_VALUE = 22;

typedef  std::map< const std::string, std::string > IniFileData;
typedef  IniFileData::iterator IniFileDataIter;
class IniFile{
	public:
          IniFile();
		IniFile( std::string aFileName );
		
		virtual ~IniFile();
		
		// Basic file operations
		void SetFileName( const std::string aFileName ){ sFileName = aFileName; };
		int Read();
		int Write();
		
		// Data readout methods 
		long Int( const std::string aGroupName, const std::string aKeyName, const long aDefaultValue );
                long long Long( const std::string aGroupName, const std::string aKeyName, const long long aDefaultValue );
                std::string String( const std::string aGroupName, const std::string aKeyName, const std::string aDefaultValue );
		float Float( const std::string aGroupName, const std::string aKeyName, const float aDefaultValue );
		// vector<int> IntVector
		// vector<float> FloatVector
		// vector<double> DoubleVector
		// Error methods
		std::string GetErrorMsg(); 

	private:
		bool CheckIfGroup( std::string aLine, std::string& aGroup );
		bool CheckIfToken( std::string aLine, std::string& aKey, std::string& aValue );
		bool CheckIfComment( std::string aLine );
		std::string 	sFileName;
		IniFileData 	mData;
		int 					iError;
};

#endif
