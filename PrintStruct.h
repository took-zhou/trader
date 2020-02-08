#ifndef __PRINTSTRUCT__
#define __PRINTSTRUCT__

#include <iostream>
#include "utils.h"
#include "ThostFtdcUserApiStruct.h"

void printCThostFtdcInputOrderField(CThostFtdcInputOrderField *order);
void printContract(Contract *order);
void printGDF_STRUCT(GDF_STRUCT *gdf);

#endif
