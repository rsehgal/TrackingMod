/*
 * ScintillatorBar_V2.h
 *
 *  Created on: 08-Nov-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_V2_INC_PAIRFINDER_H_
#define ISMRAN_ANALYSIS_V2_INC_PAIRFINDER_H_

#include <string>
#include "TreeEntry.h"
#include <vector>


namespace lite_interface{
class PairFinder {
	std::string fDatafileName;
	std::vector<TreeEntry*> fVecOfTreeEntry;
	std::vector<TreeEntry*> fVecOfPairedTreeEntry;
public:
	PairFinder();
	PairFinder(std::string datafilename);
	void LoadDataAndSort();
	void CheckPairs();
	void ValidatePairs();
	std::vector<TreeEntry*> GetVectorOfPairedTreeEntries() const;
	virtual ~PairFinder();

};
}/* End of lite_interface namespace */

#endif
