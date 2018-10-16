/*
 * Files.h
 *
 *  Created on: July 15, 2018
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_FILES_H_
#define HELPERS_INC_FILES_H_

#include <fstream>
#include <sstream>
#include <map>
#include <iterator>
#include <iostream>
#include <cstdarg>
#include "base/Vector3D.h"
namespace Tomography{


/* A Singleton class that will to open any file anywhere
** and write to that file anytime, and close anywhere anytime
*/

struct FileMap {
	operation sOp;
	std::string sFileName;
	std::fstream *sFileStream;

	FileMap(std::string filename, operation op){
		sOp = op;
		sFileName = filename;
		if(sOp == operation::read){
			sFileStream = new std::fstream();
			sFileStream->open(sFileName,std::ios::in);
		}else if(sOp == operation::write){
			sFileStream = new std::fstream();
			sFileStream->open(sFileName,std::ios::out);
		}else if(sOp == operation::append){
			sFileStream = new std::fstream();
			sFileStream->open(sFileName,std::ios::app);
		}
	}

	FileMap(FileMap& ob){
		sOp = ob.sOp;
		sFileName = ob.sFileName;
		sFileStream = ob.sFileStream;

	}

};

class Files{
	static Files* fInstance;
	std::vector<FileMap*>  fFileMapVector;

	//Additional Stuff required to read one HIT
	double fActHitX , fActHitY , fActHitZ ;
	double fFittedHitX , fFittedHitY , fFittedHitZ ;

	Files(){}

public:
	static Files* instance();

	void Open(std::string filename, operation mode){
		for(int i = 0 ; i < fFileMapVector.size() ; i++){
			if(fFileMapVector[i]->sFileName == filename){
				if(fFileMapVector[i]->sOp == operation::read){
					std::cerr << "WARNING... File already opened in Read Mode..." << std::endl;
					return;
				}else if(fFileMapVector[i]->sOp == operation::write){
					std::cerr << "WARNING... Be careful !!!! Some Process has already opened the file in Write Mode..." << std::endl;
					return;
				}
			}
		}
		fFileMapVector.push_back(new FileMap(filename,mode));
	}

	void Close(){
		for (int i = 0 ; i < fFileMapVector.size() ; i++){
			fFileMapVector[i]->sFileStream->close();
			fFileMapVector.erase(fFileMapVector.begin() + i);
		}
	}

	//Closes specified file
	void Close(std::string filename){
		for (int i = 0 ; i < fFileMapVector.size() ; i++){
			if(fFileMapVector[i]->sFileName == filename){
				fFileMapVector[i]->sFileStream->close();
				fFileMapVector.erase(fFileMapVector.begin() + i);
				return;
			}
		}
	}

	//Writing specific string to file
	void Write(std::string filename, std::string data){

	}

	FileMap* GetFileMap(std::string filename){
		for (int i = 0 ; i < fFileMapVector.size() ; i++){
			if(fFileMapVector[i]->sFileName == filename){
			   	return fFileMapVector[i];
			}
		}
	}

	void Write(std::string filename, int fmt, ...){
		FileMap* fileMap = GetFileMap(filename);
		if(fileMap->sOp == operation::read){
	   		std::cout << "This file is NOT opened in WRITE mode.." << std::endl;
	    }else{
	    	va_list args;
	        va_start(args, fmt);
	        double data;
	        while (fmt) {
	        	data = va_arg(args, double);
	            fmt--;
	            if(!fmt)
	            	*(fileMap->sFileStream) << data ;
	            else
	            	*(fileMap->sFileStream) << data << " ";
	        }
	        *(fileMap->sFileStream) << std::endl;
	        va_end(args);
	    }
	}


	void NumOfOpenedFiles(){std::cout << "Num of Opened files : " <<  fFileMapVector.size() << std::endl; }

	void FillHit(std::string filename, Tracking::Vector3D<double> &actHit, Tracking::Vector3D<double> &fittedHit ){
		FileMap* fileMap = GetFileMap(filename);
		*(fileMap->sFileStream) >> fActHitX >> fActHitY >> fActHitZ >> fFittedHitX
						>> fFittedHitY >> fFittedHitZ;
		actHit.Set(fActHitX,fActHitY,fActHitZ);
		fittedHit.Set(fFittedHitX,fFittedHitY,fFittedHitZ);
	}

	template<bool exact>
	Tracking::Vector3D<double> ReadHit(){
		if(exact)
			return Tracking::Vector3D<double>(fActHitX, fActHitY, fActHitZ);
		else
			return Tracking::Vector3D<double>(fFittedHitX, fFittedHitY, fFittedHitZ);
	}

	template<bool exact>
	Tracking::Vector3D<double> ReadHit(std::string filename){
		//Tracking::Vector3D<double> hit;
		double actHitX = 0., actHitY = 0., actHitZ = 0.;
		double fittedHitX = 0., fittedHitY = 0., fittedHitZ = 0.;
		FileMap* fileMap = GetFileMap(filename);
		*(fileMap->sFileStream) >> actHitX >> actHitY >> actHitZ >> fittedHitX
				>> fittedHitY >> fittedHitZ;
		if(exact)
			return Tracking::Vector3D<double>(actHitX, actHitY, actHitZ);
		else
			return Tracking::Vector3D<double>(fittedHitX, fittedHitY, fittedHitZ);
	}

	std::string ReadLine(std::string filename){
		std::string line = "hello";

		return line;
	}

};


}//end of Tomography namespace

//Tester Program for above logic
/*int main(){

    Tomography::Files::instance()->Open("test.txt",Tomography::operation::write);
    Tomography::Files::instance()->Open("test2.txt",Tomography::operation::write);
    
    Tomography::Files::instance()->Write("test.txt","Hello Raman");
    
    Tomography::Files::instance()->NumOfOpenedFiles();
    Tomography::Files::instance()->Close("test.txt");

    Tomography::Files::instance()->NumOfOpenedFiles();

    Tomography::Files::instance()->Write("test2.txt","Hello Raman fdfsdf");
    Tomography::Files::instance()->Close("test2.txt");

    Tomography::Files::instance()->NumOfOpenedFiles();
    //Tomography::Files::instance()->Close();


    return 0;
}*/



#endif


