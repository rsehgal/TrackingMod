/*
 * MyHit.h
 *
 *  Created on: October 3, 2020
 *      Author: rsehgal
 */

#ifndef INC_DATATREE_H_
#define INC_DATATREE_H_

#include "includes.hh"

class TTree;
class TFile;

class DataTree{

	/*Various branches that we want to attach*/
	//std::string  scintName;    //! board #  and channel number ( its packed in as follows ) //! board*16 + chno.
	const char*  scintName;    //! board #  and channel number ( its packed in as follows ) //! board*16 + chno.

	//Charge info may not be required, but keeping it for the time being.
	UInt_t    qlongNear;   //! integrated charge in long gate 88 nsec
	UInt_t    qlongFar;   //! integrated charge in long gate 88 nsec
	//UInt_t    qlongMean;   //! integrated charge in long gate 88 nsec
	Double_t    qlongMean;   //! integrated charge in long gate 88 nsec
	Double_t    qlongMeanCorrected;   //! integrated charge in long gate 88 nsec


	ULong64_t tstampNear;  //! time stamp in pico sec.
	ULong64_t tstampFar;  //! time stamp in pico sec.
	ULong64_t tsmallTimeStamp;  //! time stamp in pico sec.
	Long64_t deltaTstamp;  //! time stamp in pico sec.
	Long64_t deltaTstampCorrected;

	unsigned short barIndex;
	unsigned short layerIndex;

	double hitX;
	double hitY;
	double hitZ;


	/*TTree object*/
	TTree *fTree;
	TFile *fp;
	unsigned int fNumOfEntries;

public:
	DataTree();
	DataTree(std::string filename);
	void Fill(const char *scintname,UInt_t qlongnear,UInt_t qlongfar, Double_t qlongmean,
					Double_t qlongmeancorrected, ULong64_t tstampnear, ULong64_t tstampfar,
					ULong64_t tsmalltimestamp, Long64_t deltatstamp, Long64_t deltatstampcorrected,
					unsigned short barindex, unsigned short layerindex);
	void Fill(const char *scintname,UInt_t qlongnear,UInt_t qlongfar, Double_t qlongmean,
					Double_t qlongmeancorrected, ULong64_t tstampnear, ULong64_t tstampfar,
					ULong64_t tsmalltimestamp, Long64_t deltatstamp, Long64_t deltatstampcorrected,
					unsigned short barindex, unsigned short layerindex,double hitx, double hity, double hitz);
	void Write();
	void Close();
	void GetEntry(unsigned int entry);
	unsigned int GetEntries()const{return fNumOfEntries;}

	void Print() const {
		std::cout <<"======================================================================" << std::endl;
		std::cout << //scintName <<" , " << 
				  qlongNear <<" , " << qlongFar << " , " <<qlongMean
		          << " , " << qlongMeanCorrected << " , " << tstampNear <<" , " << tstampFar
		          << " , " << tsmallTimeStamp << " , " << deltaTstamp << " , " << deltaTstampCorrected
		          << " , " << barIndex << " , " << layerIndex << std::endl; 
	}

	TTree* GetTree()const {return fTree;}
	~DataTree();
};


#endif /* INC_DATATREE_H_ */
