#pragma once
#include "hashtable.h"
#include <fstream>

class SaveLoad
{
public:
	static bool save(const Hashtable& hashtable, const std::string& filename);
	static bool load(Hashtable& hashtable, const std::string& filename);
};

