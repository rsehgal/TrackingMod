#ifndef _BSCMODULE_H_
#define _BSCMODULE_H_

#include "TTree.h"
#include "TBranch.h"
#include <string>



#include "IniFile.h"
#include "ModuleVmeInterface.h"

using std::string;

class BSCModule{
	public:
		BSCModule();
		virtual ~BSCModule();

		std::string GetName();
		void SetName( string aName );

		void SetTree( TTree * aTree );

		long GetBaseAddress();
		void SetBaseAddress( long aBaseAddress );
                virtual void SetVmeInterface( ModuleVmeInterface * aVmeInt ){};
		virtual unsigned GetChannelNumber(){ return 0; };		
                virtual void SetIniFile( IniFile * aIniFile ){ _iniFile = aIniFile; }
		virtual int Initialize()=0;
		virtual void Start();
		virtual bool Read();
		virtual bool Write();
		virtual void Stop();
                virtual BSCModule * Clone()=0;
		
		virtual void SetReadoutTimeout( unsigned aTimeout ){ fReadoutTimeout = aTimeout; };
		virtual unsigned long GetReadoutTimeout(){ return fReadoutTimeout; };
	protected:
                IniFile * _iniFile;
		TTree * fTree;
		string fName;
		long fBaseAddr;
		unsigned long fReadoutTimeout;
};


#endif

