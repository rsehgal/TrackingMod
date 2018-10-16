/*
 * ReadOneEvent.h
 *
 *  Created on: Aug 12, 2018
 *      Author: raman
 */

#ifndef READONEEVENT_H_
#define READONEEVENT_H_

#include <vector>
#include <cstring>
#include "base/Vector3D.h"
#include <iostream>
#include "Files.h"
namespace Tomography{

using Vec_t = Tracking::Vector3D<double>;

class ReadOneEvent {
	std::string fFileName;
	int fNumOfDetectors;
	std::vector<Vec_t> fHitPointVector;

	//Two new vector to hold Actual and Fitted hit point Vectors
	std::vector<Vec_t> fActualHitPointVector;
	std::vector<Vec_t> fFittedHitPointVector;

public:
	ReadOneEvent();
	ReadOneEvent(std::string filename);
	ReadOneEvent(std::string filename, int numOfDetectors);
	virtual ~ReadOneEvent();
	template<bool exact>
	void Read(){
		fHitPointVector.clear();
			for(int i = 0 ; i < fNumOfDetectors ; i++){
				Files *filePtr = Files::instance();
				//filePtr->FillHit(fFileName);
				//fHitPointVector.push_back(filePtr->ReadHit<exact>());
				fHitPointVector.push_back(filePtr->ReadHit<exact>(fFileName));

			}

	}

	void Fill();
	std::vector<Vec_t> GetActualHitPointVector() const {return fActualHitPointVector;}
	std::vector<Vec_t> GetFittedHitPointVector() const {return fFittedHitPointVector;}

	//Some Required Getters
	std::vector<Vec_t> GetHitPointVector() const {
		return fHitPointVector;
	}

	friend std::ostream &operator<<( std::ostream &output,  ReadOneEvent &D ) {
		for(int i = 0 ; i < D.fHitPointVector.size() ; i++){
	         output << D.fHitPointVector[i].x() << " , "
	        		<< D.fHitPointVector[i].y() << " , "
	        		<< D.fHitPointVector[i].z() << std::endl;
		}
	}

	void Print(){
		for(int i = 0 ; i < fHitPointVector.size() ; i++){
			         std::cout << fHitPointVector[i].x() << " , "
			        		<< fHitPointVector[i].y() << " , "
			        		<< fHitPointVector[i].z() << std::endl;
				}
	}

};

}

#endif /* READONEEVENT_H_ */
