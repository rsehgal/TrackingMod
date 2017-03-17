#ifndef __MODULE_V1190_DATA_H_
#define __MODULE_V1190_DATA_H_

typedef enum V1190DataType {
  V1190_Measurement     = 0x00,
  V1190_TdcHeader       = 0x01,
  V1190_TdcTrailer      = 0x03,
  V1190_TdcError        = 0x04,
  V1190_GlobalHeader    = 0x08,
  V1190_GlobalTrailer   = 0x10,
  V1190_TriggerTimeTag  = 0x11,
  V1190_Filler          = 0x18,
  V1190_InvalidData     = 0x1F
} V1190DataType;

class ModuleV1190Data {
  public:
    ModuleV1190Data();
    ModuleV1190Data( unsigned long aData );
    virtual ~ModuleV1190Data();
    
    unsigned long GetDataType();
    void Decode();
    void SetData( unsigned long aData );
    
    
    
    bool IsMeasurement();
    bool IsTdcHeader();
    unsigned GetChannel(){ return _channel; };
    unsigned long GetMeasurement(){ return _measurement; };
    unsigned short GetTdc(){ return _tdc; };
  private:
    void _DecodeDatum();
    void _DecodeGlobalHeader();
    void _DecodeGlobalTrailer();
    void _DecodeTdcTrailer();
    void _DecodeTdcHeader();
    
    unsigned long _data;
    unsigned long _measurement;
    unsigned _channel;
    unsigned long _eventCount;
    unsigned _geo;
    unsigned long _wordCount;
    unsigned short _tdc;
    unsigned long _eventId;
    unsigned long _bunchId;
};



#endif



