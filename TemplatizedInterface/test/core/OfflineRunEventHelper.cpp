/*
 * OfflineRunEventHelper.cpp
 *
 *  Created on: Aug 8, 2018
 *      Author: raman
 */
#include "RunHelper.h"
#include "EventHelper.h"
#include "Voxelator_Evolution.h"
#include "G4SystemOfUnits.hh"


int main(){
	Tomography::evolution::Voxelator::Create(50*cm,50*cm,50*cm,2.5*cm,2.5*cm,2.5*cm);
	{
		Tomography::EventHelper u("TrackExact.txt","EventHelperTrackExact.txt");
		Tomography::RunHelper r("Exact");
		r.PrintMaxMinSDAndRL();
	}


	{
		Tomography::EventHelper u("TrackFitted.txt","EventHelperTrackFitted.txt");
		Tomography::RunHelper r("Fitted");
	}


	{
		//Tomography::EventHelper u("TrackSampledFitted.txt","EventHelperTrackSampledFitted.txt");
		//Tomography::RunHelper r("SampledFitted");
	}

	return 0;
}



