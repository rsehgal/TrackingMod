/*
 * ExactHitDataTree.h
 *
 *  Created on: 01-Dec-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_INC_EXACTHITDATATREE_H_
#define ISMRAN_INC_EXACTHITDATATREE_H_

#include <vector>
#include <TFile.h>
#include <TTree.h>


namespace lite_interface {

class ExactHitDataTree {

	std::vector<double> fXVec;
	std::vector<double> fYVec;
	std::vector<double> fZVec;
	double fInitialEnergy;
	double fDepositedEnergy;
	unsigned int fEvNo;

	int fNumOfEntries;

	TFile *fp;
	TTree *fTree;

	/*
	 * Various angle that we want to store
	 */
	double fAngleCRY = 0.;
	double fAngleReconsLinear = 0.;
	double fAngleReconsParam = 0.;
	double fAngleReconsMean = 0.;
	double fAngleReconsExact = 0;

public:
	ExactHitDataTree();
	virtual ~ExactHitDataTree();
	void Fill(std::vector<double> xvec, std::vector<double> yvec, std::vector<double> zvec);
	void Fill(std::vector<double> xvec, std::vector<double> yvec, std::vector<double> zvec, double initialEnergy, double depositedEnergy);
	void Fill(std::vector<double> xvec, std::vector<double> yvec, std::vector<double> zvec, double initialEnergy, double depositedEnergy,unsigned int evNo);
	void Fill(std::vector<double> xvec, std::vector<double> yvec, std::vector<double> zvec, double initialEnergy, double depositedEnergy,unsigned int evNo
			  ,double anglecry, double anglereconsLinear, double anglereconsParam, double anglereconsMean, double anglereconsExact);
	void Write();
	void Close();
	void GetEntry(unsigned int entry);
	unsigned int GetEntries()const{return fNumOfEntries;}
	TTree* GetTree()const {return fTree;}
};

} /* namespace lite_interface */

#endif /* ISMRAN_INC_EXACTHITDATATREE_H_ */