#include "ModuleV1190Data.h"
#include <iomanip>
#include <iostream>


ModuleV1190Data::ModuleV1190Data():_data( 0 ){

}

ModuleV1190Data::ModuleV1190Data( unsigned long aData ){
  SetData( aData );
}

ModuleV1190Data::~ModuleV1190Data(){

}

unsigned long ModuleV1190Data::GetDataType(){
  return ( ( ( _data ) >> 27 ) & 0x1f );
}

bool ModuleV1190Data::IsMeasurement(){
  V1190DataType type = static_cast< V1190DataType >( this->GetDataType() );
  //std::cout << "Type = " << type << std::endl;
  return ( type == V1190_Measurement );
}

bool ModuleV1190Data::IsTdcHeader(){
  V1190DataType type = static_cast< V1190DataType >( this->GetDataType() );
  //std::cout << "Type = " << type << std::endl;
  return ( type == V1190_TdcHeader );
}

void ModuleV1190Data::Decode(){
  unsigned long Type = this->GetDataType();
  switch( Type ){
    case static_cast< unsigned long >( V1190_Measurement ) :
      this->_DecodeDatum();
      //std::cout << "Channel: " << _channel << " Data: " << _measurement << std::endl; 
      break;
    case static_cast< unsigned long>( V1190_GlobalHeader) :
      this->_DecodeGlobalHeader();
      //std::cout << "Events count: " << _eventCount << " Geo: " << _geo << std::endl; 
      break;
    case static_cast< unsigned long>( V1190_GlobalTrailer ) :
      this->_DecodeGlobalTrailer();
      //std::cout << "Words count: " << _wordCount << " Geo: " << _geo << std::endl; 
      break;
      
    case static_cast< unsigned long>( V1190_TdcTrailer ) :
      this->_DecodeTdcTrailer();
      //std::cout << "Words count: " << _wordCount << " Tdc: " << _tdc << " Event Id: " << _eventId << std::endl; 
      break;
      
    case static_cast< unsigned long>( V1190_TdcHeader ) :
      this->_DecodeTdcHeader();
      //std::cout << "Bunch Id: " << _bunchId << " Tdc: " << _tdc << " Event Id: " << _eventId << std::endl; 
      break;    
      
    default :
      //std::cerr << "Uknown type " << std::hex << Type << std::dec << std::endl;
      break;
  }
}

void ModuleV1190Data::SetData( unsigned long aData ){ 
  _data = aData; 
}

void ModuleV1190Data::_DecodeDatum(){
  if( this->GetDataType() == V1190_Measurement ){
    _measurement = ( (unsigned long )( _data ) & 0x7FFFF );
    _channel = ( ( ( unsigned long )( _data ) >> 19 ) & 0x1F );
  }
}

void ModuleV1190Data::_DecodeGlobalHeader(){
  if( this->GetDataType() == V1190_GlobalHeader ){
    _eventCount = ( ( ( unsigned long )( _data ) >> 5 ) & 0x3FFFFF );
    _geo = ( ( unsigned long )( _data ) & 0x1F );
  }
}

void ModuleV1190Data::_DecodeGlobalTrailer(){
  if( this->GetDataType() == V1190_GlobalTrailer ){
    _wordCount = ( ( (unsigned long)(_data)>>5 ) & 0xFFFF );
    _geo = ( ( unsigned long )( _data ) & 0x1F );
  }
}

void ModuleV1190Data::_DecodeTdcTrailer(){
  if( this->GetDataType() == V1190_TdcTrailer ){
    _tdc = ( ( (unsigned long)(_data)>>24 ) & 0x3 );
    _eventId = ( ( (unsigned long)(_data)>>12 ) & 0x0FFF );
    _wordCount = ( (unsigned long)(_data) & 0x0FFF );
  }
}

void ModuleV1190Data::_DecodeTdcHeader(){
  if( this->GetDataType() == V1190_TdcHeader ){
    _tdc = ( ( (unsigned long)(_data)>>24 ) & 0x3 );
    _eventId = ( ( (unsigned long)(_data)>>12 ) & 0x0FFF );
    _bunchId = ( (unsigned long)(_data) & 0x0FFF );
  }
}


