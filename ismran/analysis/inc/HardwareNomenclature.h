/*
 * HardWareNomenclature.h
 *
 *  Created on: 09-Sep-2020
 *      Author: rsehgal
 */

#ifndef ISMRAN_ANALYSIS_INC_HARDWARENOMENCLATURE_H_
#define ISMRAN_ANALYSIS_INC_HARDWARENOMENCLATURE_H_

#include <vector>
#include <string>

extern std::string source;
extern std::vector<std::string> vecOfOnlyBarsNames;
extern std::string mode;
extern std::vector<int> vecOfNearVoltage;
extern std::vector<int> vecOfFarVoltage;
extern std::vector<int> vecOfNearThreshold;
extern std::vector<int> vecOfFarThreshold;
extern std::vector<int> vecOfSourcePositions;

class HardWareNomenclature {
public:
	virtual ~HardWareNomenclature();
	HardWareNomenclature();
};

#endif /* ISMRAN_ANALYSIS_INC_HARDWARENOMENCLATURE_H_ */
