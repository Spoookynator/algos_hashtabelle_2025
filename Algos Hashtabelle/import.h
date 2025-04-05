#pragma once
#include <string>

struct Data;

Data** parseLoad(std::string, double, double, double, double, int);

Data** parseCsv(std::string);