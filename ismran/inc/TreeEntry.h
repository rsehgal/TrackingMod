/*
 * TreeEntry.h
 *
 *  Created on: 21-Aug-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_INC_TREEENTRY_H_
#define ISMRAN_INC_TREEENTRY_H_

#include "includes.hh"
#include "PsBar.h"

struct TreeEntry
{
  UShort_t  brch;    //! board #  and channel number ( its packed in as follows )
  //! board*16 + chno.
  UInt_t    qlong;   //! integrated charge in long gate 88 nsec
  ULong64_t tstamp;  //! time stamp in pico sec.
  UInt_t    time;    //! real computer time in sec
  Int_t     dt;      //! delt between near and far channels

  TreeEntry(UShort_t br, UInt_t ql, ULong64_t ts,UInt_t t, Int_t delT){
    brch=br;
    qlong=ql;
    tstamp=ts;
    time=t;
    dt=delT;

  }

  void Print(){
  	std::cout << brch <<" , " << qlong << " , " << tstamp <<" , "<< time << ", " << dt << std::endl;
  }

};

using TreeEntryVector=std::vector<TreeEntry>;

extern std::vector<TreeEntry> CheckPairs(std::vector<TreeEntry> treeEntVec);
extern void PrintEntryVector(TreeEntryVector treeEntVec);
extern std::vector<ScintillatorBar*> DetectMuonHits(TreeEntryVector treeEntVec);
extern std::vector< std::vector<ScintillatorBar*> >  DetectMuonTracks(std::vector<ScintillatorBar*> muonHitVector);
extern bool CompareTimestampScintillator(ScintillatorBar *i1, ScintillatorBar *i2);
extern bool CompareBarIndexInScintillator(ScintillatorBar *i1, ScintillatorBar *i2);
extern void PrintScintillatorVector(std::vector<ScintillatorBar*> scintBarVector);
extern void PrintMuonTrackVector(std::vector< std::vector<ScintillatorBar*> > muonTrackVec);
extern void PrintMuonTrackVectorAllLayers(std::vector< std::vector<ScintillatorBar*> > muonTrackVec);
#endif /* ISMRAN_INC_TREEENTRY_H_ */
