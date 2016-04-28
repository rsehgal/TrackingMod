#include "IniFile.h"
#include <iostream>
#include <sstream>
#include "TDC.h"

namespace Tracking{

IniFile::IniFile(){

}

IniFile::IniFile( std::string aFileName ){
	SetFileName( aFileName );
}

IniFile::~IniFile(){

}

int IniFile::Read(){
	std::ifstream ini( sFileName.c_str() );
	std::stringstream parser;
	std::string token, value, line, group;
	iError = INI_OK;
	// Loading the file into the parser
	if( ini ){
		parser << ini.rdbuf();
		ini.close();
	} else {
		iError = INI_ERROR_CANNOT_OPEN_READ_FILE;
		return iError;
	}
	group = "";
	
	while( std::getline( parser, line ) && ( iError == INI_OK ) ){
		// Check if the line is comment
		if( !CheckIfComment( line ) ){
			// Check for group
			if( !CheckIfGroup( line, group ) ){
				// Check for token
				if( CheckIfToken( line, token, value ) ){
					// Make the key in format group.key if the group is not empty
					if( group.size() > 1 ) token = group + "." + token;
					mData[ token ] = value;
				}	else {
					iError = INI_ERROR_WRONG_FORMAT;
					return iError;
				}
			}	
		}			
	}
	for( IniFileDataIter Iter = mData.begin(); Iter != mData.end(); Iter++ ) std::cout << "INI: " << Iter->first << " = " << Iter->second << std::endl;
	return iError;
}


int IniFile::ReadAndFillTDC(){
    std::ifstream ini( sFileName.c_str() );
    std::stringstream parser;
    std::string token, value, line, group;
    iError = INI_OK;
    // Loading the file into the parser
    if( ini ){
        parser << ini.rdbuf();
        ini.close();
    } else {
        iError = INI_ERROR_CANNOT_OPEN_READ_FILE;
        return iError;
    }
    group = "";
    TdcMap mapTDC;
    MapVector mapVector;
    while( std::getline( parser, line ) && ( iError == INI_OK ) ){
            // Check if the line is comment
            if( !CheckIfComment( line ) ){
                // Check for group
                if( !CheckIfGroup( line, group ) ){
                    // Check for token
                    if( CheckIfToken( line, token, value ) ){
                        // Make the key in format group.key if the group is not empty
                        //if( group.size() > 1 ) token = group + "." + token;
                        mData[ token ] = value;
                    }   else {
                        iError = INI_ERROR_WRONG_FORMAT;
                        return iError;
                    }
                }
                else{
                  mapVector.push_back(mData);
                  mData.clear();
                }
            }
        }
    mapVector.push_back(mData);



    for(int i=3 ; i<mapVector.size() ; i++){
      TDC tempTDC;
      tempTDC.SetName(mapVector[i]["Name"]);
      //std::cout<<"TriggerWindowWidth : "<<mapVector[i]["TriggerWindowWidth"]<<std::endl;
      tempTDC.SetTriggerWindowWidth(std::stoi(mapVector[i]["TriggerWindowWidth"], nullptr,10 ));
      tempTDC.SetTriggerWindowOffset(std::stoi(mapVector[i]["TriggerWindowOffset"], nullptr,10 ));
      tempTDC.SetTriggerExtraSearchMargin(std::stoi(mapVector[i]["TriggerExtraSearchMargin"], nullptr,10 ));
      tempTDC.SetTriggerRejectMargin(std::stoi(mapVector[i]["TriggerRejectMargin"], nullptr,10 ));
      tempTDC.SetEnableTriggerTimeSubstraction(std::stoi(mapVector[i]["EnableTriggerTimeSubstraction"], nullptr,10 ));
      tempTDC.SetIndividualLSB(std::stoi(mapVector[i]["IndividualLSB"], nullptr,10 ));

      fTdcVector.push_back(tempTDC);
      //std::cout<<"Tdc-NName : "<<mapVector[i]["Name"]<<std::endl;
      //std::cout<<"TDC-NAME : "<<mapVector[i]["Name"]<<std::endl;
    }

    std::cout<<"Num of Groups in INI file : "<<mapVector.size()<<std::endl;

}

bool IniFile::CheckIfGroup( std::string aLine, std::string& aGroup ){
	if( aLine[ 0 ] == '[' ){
		if( aLine[ aLine.length() - 1 ] == ']' ){ // The format is right
				aGroup = aLine.substr( 1, aLine.length()-2 );
				return true;
		} else { 
			iError = INI_ERROR_WRONG_GROUP_FORMAT; 
			return true;
		} 
	}
	return false;
}

bool IniFile::CheckIfToken( std::string aLine, std::string& aKey, std::string& aValue ){
	unsigned int p0, p1;
	p0 = 0;
	p1 = std::string::npos;
	p1 = aLine.find_first_of( '=', p0 );
	if( p0 != p1 ){
		aKey = aLine.substr( p0, p1 - p0 );
		p0 = aLine.find_first_not_of( '=', p1 );
		if( p0 != std::string::npos ){
			aValue = aLine.substr( p0, aLine.size() - p0 );
		} else {
			iError = INI_ERROR_MISSING_VALUE;
			return true;
		}
	} else {
		iError = INI_ERROR_WRONG_FORMAT;
		return false;
	}
	return true;
}

bool IniFile::CheckIfComment( std::string aLine ){
	return ( aLine[ 0 ] == '#' );
}

int IniFile::Write(){
	iError = INI_OK;
	
	return iError;
}


long	IniFile::Int( const std::string aGroupName, const std::string aKeyName, const long aDefaultValue ){
	std::stringstream sstr;
	std::string key;
	long res = aDefaultValue;
	IniFileDataIter Iter;
	if( aGroupName.size() > 0 ) key = aGroupName + "." + aKeyName;
	Iter = mData.find( key );
	if( Iter != mData.end() ){
          sstr << Iter->second;	
          sstr >> res;
	}
	return res;
}

long long IniFile::Long( const std::string aGroupName, const std::string aKeyName, const long long aDefaultValue ){
  std::stringstream sstr;
  std::string key;
  long long result = aDefaultValue;
  IniFileDataIter Iter;
  if( aGroupName.size() > 0 ) key = aGroupName + "." + aKeyName;
  Iter = mData.find( key );
  if( Iter != mData.end() ){
    sstr << Iter->second;	
    sstr >> result;
  }
  return result;
}

std::string IniFile::String( const std::string aGroupName, const std::string aKeyName, const std::string aDefaultValue ){
	std::string key;
	std::string res = aDefaultValue;
	IniFileDataIter Iter;
	if( aGroupName.size() > 0 ) key = aGroupName + "." + aKeyName;
	Iter = mData.find( key );
	if( Iter != mData.end() ){
		res = Iter->second;	
	}
	return res;
}

float IniFile::Float( const std::string aGroupName, const std::string aKeyName, const float aDefaultValue ){
	std::stringstream ss;
	std::string key;
	float res = aDefaultValue;
	IniFileDataIter Iter;
	if( aGroupName.size() > 0 ) key = aGroupName + "." + aKeyName;
	Iter = mData.find( key );
	if( Iter != mData.end() ){
		ss << Iter->second;	
		ss >> res;
	}
	return res;
}

std::string IniFile::GetErrorMsg(){
	return "";
}
}// end of Tracking namespace

