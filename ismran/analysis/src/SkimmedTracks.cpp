/*
 * SkimmedTracks.cpp
 *
 *  Created on: 18-Sep-2020
 *      Author: rsehgal
 */

#include "SkimmedTracks.h"
std::vector<SkimmedMuonTrack*> vecOfSkimmedMuonTracks;

void ResetVectorOfSkimmedMuonTracks()
{
	ResetVector<SkimmedMuonTrack>(vecOfSkimmedMuonTracks);
}



