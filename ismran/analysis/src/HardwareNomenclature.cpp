/*
 * HardWareNomenclature.cpp
 *
 *  Created on: 09-Sep-2020
 *      Author: rsehgal
 */

#include "HardwareNomenclature.h"

std::string source = "Cs137";

std::vector<std::string> vecOfOnlyBarsNames={
  "PS71_SF874"   ,"PS72_SF889"   ,"PS73_SF878"  ,"PS74_SF891"   ,"PS75_SF887" ,
  "PS76_SF888"   ,"PS77_SF892"   ,"PS78_SF886"   ,"PS79_SF883"   ,"PS80_SF877" ,
  "PS81_SF879"   ,"PS82_SF885"   ,"PS83_SF890"   ,"PS84_SF882"   ,"PS85_SF881" ,
  "PS86_SF884"   ,"PS87_SF876"   ,"PS88_SF880"   ,"PS89_SF873"
  //,"PS90_SF875"
};

std::string mode = "Couples";

std::vector<int> vecOfNearVoltage={
	1802, 1680, 1778, 1770, 1761,
	1809, 1713, 1929, 1941, 1780,
	1750, 1808, 1700, 1856, 1820,
	1849, 1996, 2029, 1862, 1816
};

std::vector<int> vecOfNearThreshold={
	6, 10, 6, 12, 12,
	12, 20, 20, 13, 30,
	30, 12, 12, 30, 12,
	20, 12, 12, 12, 12
};

std::vector<int> vecOfFarVoltage={
	1939, 1745, 1833, 1740, 1719,
	1840, 1734, 1922, 1999, 1784,
	1804, 1793, 1750, 1888, 1830,
	1813, 1994, 1930, 1821,1844
};

std::vector<int> vecOfFarThreshold={
	6, 10, 6, 12, 12,
	12, 20, 20, 13, 30,
	30, 12, 12, 30, 12,
	20, 12, 12, 12, 12
};

std::vector<int> vecOfSourcePositions{
	-45, -40, -30, -20, -10, 0, 10, 20, 30, 40, 45
};

HardWareNomenclature::~HardWareNomenclature() {
	// TODO Auto-generated destructor stub
}

HardWareNomenclature::HardWareNomenclature() {
	// TODO Auto-generated constructor stub

}

