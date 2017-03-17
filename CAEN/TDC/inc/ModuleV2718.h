#ifndef __ModuleV2718_HH_
#define __ModuleV2718_HH_

#include <iostream>
#include <vector>
#include "BSCModule.h"
#include "ModuleVmeInterface.h"

class ModuleV2718 : public BSCModule, public ModuleVmeInterface {
public:
        ModuleV2718();
        virtual ~ModuleV2718();
        void Start();  
        int Initialize();
//      void Calibrate();
        bool Read();
        bool Write();
        void Stop();
        BSCModule * Clone();

        unsigned char ReadC( unsigned long aAddress );
        unsigned short ReadW( unsigned long aAddress );
        unsigned long ReadL( unsigned long aAddress );

        void WriteC( unsigned long aAddress, unsigned char aData );
        void WriteW( unsigned long aAddress, unsigned short aData );
        void WriteL( unsigned long aAddress, unsigned long aData );
       
        void SetPulserParameters( unsigned char aPulser, unsigned char aPeriod, unsigned char aWidth, unsigned char aOutput,unsigned char NumberOfPulses );
        void StartPulser( unsigned char aPulser );
        void StopPulser( unsigned char aPulser );

        CVErrorCodes GetStatus(){ return _status; };
        std::string GetError(){ return CAENVME_DecodeError( _status ); };

};

#endif
