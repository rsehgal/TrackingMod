#include "BSCModule.h"
#include <iostream>

BSCModule::BSCModule(){

}

BSCModule::~BSCModule(){

}

std::string BSCModule::GetName(){ 
	return fName; 
}

void BSCModule::SetName( string aName ){ 
	fName = aName; 
}

void BSCModule::SetTree( TTree * aTree ){ fTree = aTree; };

long BSCModule::GetBaseAddress() { return fBaseAddr; };
void BSCModule::SetBaseAddress( long aBaseAddress ){ fBaseAddr = aBaseAddress; };
void BSCModule::Start(){};
bool BSCModule::Read(){ return true; };
bool BSCModule::Write(){ return true; };
void BSCModule::Stop(){};
/*long BSCModule::GetBoardId(){ return fBoardId; };
long BSCModule::GetVmeLink(){ return fVmeLink; };
void BSCModule::SetBoardId( long aBoardId ){ fBoardId = aBoardId; };
void BSCModule::SetVmeLink( long aVmeLink ){ fVmeLink = aVmeLink; };
int32_t BSCModule::GetVmeHandle(){ return _handle; };
*/
//BSCModule * BSCModule::Clone(){ std::cout << "Abstract module class" << std::endl; return NULL; };

