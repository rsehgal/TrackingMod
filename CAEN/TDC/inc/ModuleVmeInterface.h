#ifndef _MODULEVMEINT_H_
#define _MODULEVMEINT_H_

#include <string>

#define LINUX 1
#include "CAENVMElib.h"
#include "CAENVMEoslib.h"
#include "CAENVMEtypes.h"

class ModuleVmeInterface {
  public:
    ModuleVmeInterface(){};
    virtual ~ModuleVmeInterface(){};
    virtual unsigned char ReadC( unsigned long aAddress )=0;
    virtual unsigned short ReadW( unsigned long aAddress ){ return 0; };
    virtual unsigned long ReadL( unsigned long aAddress ){ return 0; };

    virtual void WriteC( unsigned long aAddress, unsigned char aData ){};
    virtual void WriteW( unsigned long aAddress, unsigned short aData ){};
    virtual void WriteL( unsigned long aAddress, unsigned long aData ){};
    long GetBoardId(){ return _boardId; };
    long GetVmeLink(){ return _vmeLink; };
    int32_t GetVmeHandle(){ return _handle; };
    void SetBoardId( long aBoardId ){ _boardId = aBoardId; };
    void SetVmeLink( long aVmeLink ){ _vmeLink = aVmeLink; };


    virtual CVErrorCodes GetStatus(){ return _status; };
    virtual std::string GetError(){ return CAENVME_DecodeError( _status ); };
    
  protected:
    CVErrorCodes _status;
    int32_t _handle;
    long _vmeLink;
    long _boardId;
    

};


#endif
