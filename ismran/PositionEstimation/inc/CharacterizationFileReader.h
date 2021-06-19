/*
**	Filename : CharacterizationFileReader.h
**	2021-06-19
**	username : raman
*/
#ifndef CharacterizationFileReader_h
#define CharacterizationFileReader_h

class CharacterizationFileReader{
		std::string fFileName;
		UShort_t fBrch; //! board #  and channel number ( its packed in as follows )     //! board*16 + chno.
                UInt_t fQlong;   //! integrated charge in long gate 88 nsec
                ULong64_t fTstamp;  //! time stamp in pico sec.
                UInt_t time;    //! real computer time in sec
                int delt;
}

#endif
