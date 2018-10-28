/*
 * TestFit.cpp
 *
 *  Created on: 16-Oct-2018
 *      Author: rsehgal
 */
#include "ReadOneEvent.h"
#include "base/Global.h"
#include "Files.h"
#include "Fit2DLinear.h"
#include <iostream>
#include <fstream>
#include "base/Vector3D.h"
#include "CommonFunc.h"
#include "Imaging.h"
using Vec_t = Tracking::Vector3D<double>;
int main() {
	//Try to get fitted Hit point vector from pixel center points
	Tomography::Fit2DLinear fitter;
	std::string filename = "ActualAndFittedHits.txt";
	//Opening the required file
	Tomography::Files::instance()->Open(filename, Tomography::operation::read);
	//Creating object of Event Reader
	Tomography::ReadOneEvent r(filename);


	Tracking::ImageReconstruction Im;
	std::vector<Vec_t> incomingHitPtVector, outgoingHitPtVector;
	Tomography::Track incoming, outgoing;
	//Accessing 10 events, but only one at a time
	for (int i = 0; i < 10; i++) {
		r.Fill();
		std::vector<Vec_t> actualHitPointVector = r.GetActualHitPointVector();
		std::vector<Vec_t> fittedHitPointVector = r.GetFittedHitPointVector();
		incomingHitPtVector.clear();
		outgoingHitPtVector.clear();
		std::cout << "==================================" << std::endl;

		for (int j = 0; j < actualHitPointVector.size() / 2; j++) {
			incomingHitPtVector.push_back(actualHitPointVector[j]);
		}
		incoming =
				CommonFunc::Functions::instance()->CreateTrackFromHitPointVector(
						incomingHitPtVector);
		std::cout << "Actual Incoming  Track : ";
		incoming.Print();

		for (int j = actualHitPointVector.size() / 2;
				j < actualHitPointVector.size(); j++) {
//		for(int j = actualHitPointVector.size()-1 ; j >= actualHitPointVector.size()/2 ; j--){
			outgoingHitPtVector.push_back(actualHitPointVector[j]);
		}
		outgoing =
				CommonFunc::Functions::instance()->CreateTrackFromHitPointVector(
						outgoingHitPtVector);
		std::cout << "Actual Outgoing Track : ";
		outgoing.Print();
		Vec_t pocaPtActual = Im.POCA_V4(incoming, outgoing);
		std::cout << "Actual POCA : ";
		pocaPtActual.Print();

		std::cout << "*********************************" << std::endl;
		incomingHitPtVector.clear();
		outgoingHitPtVector.clear();
		for (int j = 0; j < fittedHitPointVector.size() / 2; j++) {
			incomingHitPtVector.push_back(fittedHitPointVector[j]);
		}

		incoming =
				CommonFunc::Functions::instance()->CreateTrackFromHitPointVector(
						incomingHitPtVector);
		std::cout << "Incoming Fitted Track : ";
		incoming.Print();

		for (int j = fittedHitPointVector.size() / 2;
				j < fittedHitPointVector.size(); j++) {
//		for(int j = fittedHitPointVector.size()-1 ; j >= fittedHitPointVector.size()/2 ; j--){
			outgoingHitPtVector.push_back(fittedHitPointVector[j]);
		}
		outgoing =
				CommonFunc::Functions::instance()->CreateTrackFromHitPointVector(
						outgoingHitPtVector);
		std::cout << "Outgoing Fitted Track : ";
		outgoing.Print();
		Vec_t pocaPtFitted = Im.POCA_V4(incoming, outgoing);
		std::cout << "Fitted POCA : ";
		pocaPtFitted.Print();

	}

	return 0;
}

